#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // string tools needed all over the place
#include <unistd.h> // POSIX API
#include <stdbool.h> // boolean data type
#include <dirent.h> // directory tools
#include <fnmatch.h> // wildcard matching for POSIX
#include <sys/types.h> // pid_t type
#include <pthread.h> // multithreading the file processing
#include <semaphore.h>
#include <assert.h>
#include "mythreads.h"
#include "datastructures.h"
#include <sys/ipc.h>
#include <sys/msg.h>

int THRESH = 0;
int max = 10;
int items;
key_t key;

int use  = 0;
int fill = 0;

sem_t empty;
sem_t full;
sem_t mutex;

#define CMAX (1)
int consumers = 10;

typedef struct buff{
    char* word;
    int count;
} buff;

buff *buffer[10];
pid_t id = 0;
char *self_pid;
//char *process_fn;

LLS* fileparse(const char *str){
    FILE *fptr;
    fptr = fopen(str, "r");
    LLS * list = createList();
    if (fptr == NULL){
        perror("Failure opening file");
        exit(1);
        return NULL;
    }
    char* buffptr;
    int chk = fscanf(fptr, "%m[^\n]\n", &buffptr);
    while ((chk!= EOF) && (chk != 0)){
        if (buffptr == NULL){
            continue;
        }
        addItem(list, buffptr);
        free(buffptr);
        buffptr = NULL;
        chk = fscanf(fptr, "%m[^\n]\n", &buffptr);
    }
    free(buffptr);
    fclose(fptr);
    return list;
}

int llsFormatDirs(LLS* list){
    if (list->head == NULL){
        return 0;
    }
    Node * curNode = list->head;
    int MAP_SIZE = 3;
    while (curNode != NULL){
        if (!(strncmp(curNode->word, "map", MAP_SIZE))){ // remove "map"
            char* temp = malloc(strlen(curNode->word)+1);
            char* start = curNode->word + MAP_SIZE;
            strcpy(temp, start);
            curNode->word = realloc(curNode->word, strlen(temp)+1);
            strcpy(curNode->word, temp);
            while (!(strncmp(curNode->word, " ", 1))){ // eliminate any leading whitespace which remains
                start = curNode->word + 1;
                strcpy(temp, start);
                strcpy(curNode->word, temp);
                free(temp);
            }
            curNode->valid = true;
        }
        else{
            fprintf(stderr, "Error, improperly formatted directory commands");
            curNode->valid = false;

        }
        curNode = curNode->next;
    }
    return 1;
}

LLS* getDirectories(int argc, char *argv[]){
    const char* commandFile= argv[1];
    //int* bufferSize = atoi(argv[2]);
    LLS* dir_cmds = fileparse(commandFile);
    llsFormatDirs(dir_cmds);
    return dir_cmds;
}

int findtxt(char * dir, LLS* list){
    const char *path = dir;
    DIR* d = opendir(path);
    if (!d){
        fprintf(stderr, "Error opening directory: %s.\n", dir);
        return 1;
    }
    struct dirent *entry;
    //FILE * f = fopen(process_fn, "a");
    //fprintf(f,"PID: %d. Found files are: \n", (int)id);
    while ((entry = readdir(d)) != NULL) {
        if ((fnmatch("*.txt", entry->d_name, 0) == 0)) { // add to list if match
            char* temp = malloc(strlen(dir)+strlen(entry->d_name)+2);
            strcpy(temp, dir);
            strcat(temp, "/");
            strcat(temp, entry->d_name);
            addItem(list, temp);
            free(temp);
            //fprintf(f, "PID: %d. Adding: '%s' to list\n", (int)id, entry->d_name);
        }
        //fprintf(stderr,"Found file: %s\n", entry->d_name);
    }
    //fclose(f);
    closedir(d);
    return 0;
}

mLLS* mfileparse(const char *str){
    FILE *fptr;
    fptr = fopen(str, "r");
    mLLS * list = createmList();
    if (fptr == NULL){
        perror("Failure opening file");
        exit(1);
        return NULL;
    }
    char* buffptr;
    int chk = fscanf(fptr, "%ms", &buffptr);
    while ((chk!= EOF) && (chk != 0)){
        if (buffptr == NULL){
            continue;
        }
        addmItem(list, buffptr);
        free(buffptr);
        buffptr = NULL;
        chk = fscanf(fptr, "%ms", &buffptr);
    }
    free(buffptr);
    fclose(fptr);
    return list;
}

void do_fill(char* word, int count) {
    strcpy(buffer[fill]->word, word);
    buffer[fill]->count = count;
    fill++;
    //fprintf(stderr, "FILL: %d ", fill);
    if (fill == max) {
        fill = 0;
    }
    return;
}

buff *do_get() {
    buff *item = malloc(sizeof(buff));
    if (item == NULL){
        fprintf(stderr, "FATAL ERROR: malloc() failure in do_get 1.\n");
        exit(0);
    }
    item->count = buffer[use]->count;
    item->word = malloc(strlen(buffer[use]->word)+1);
    if (item->word == NULL){
        fprintf(stderr, "FATAL ERROR: malloc() failure in do_get 2.\n");
        exit(0);
    }
    strcpy(item->word, buffer[use]->word);
    use++;
    //fprintf(stderr, " USE: %d", use);
    if (use == max) {
        use = 0;
    }
    return item;
}

/*
void set_proc_fn(){
    int temp = (int)getpid();
    char temparr[50];
    snprintf(temparr, sizeof(temparr), "%d", temp);
    process_fn = malloc(sizeof(char)*50);
    strcpy(process_fn, "log_consumer_");
    strcat(process_fn, temparr);
    strcat(process_fn, ".txt");
}
*/

void sendmsg(int qid, char* word, int count){
    message msg = {0};
    msg.mtype = 1;
    msg.cnt = count;
    snprintf(msg.word, sizeof(msg.word),"%s" ,word);
    int err = msgsnd(qid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);
    if (err == -1){
        perror("msgsnd failue.\n");
        exit(0);
    }
}

void endTransaction(int qid){
    sendmsg(qid, (char *)end_msg_str, end_msg_num);
    return;
}

int startTransaction(){
    int qid = msgget(key, IPC_CREAT | 0666);
    //fprintf(stderr, "Sender is ready for messaging.\n");
    sendmsg(qid, (char*)start_msg_str, start_msg_num);
    if (qid == -1) {
        perror("msgget failure. Unable to start messaging.\n");
        exit(0);
    }
    return qid;
}



void SendmList(mLLS* list){
    mNode* curNode = list->head;
    int qid = startTransaction();
    while (curNode!= NULL){
        sendmsg(qid, curNode->word, curNode->count);
        curNode = curNode->next;
    }
    endTransaction(qid);
    //fprintf(stderr, "This lil process is all tidyed up.\n");

}


void consumer(int numThreads) {
    buff *tmp = NULL;
    //FILE *f = fopen(process_fn, "a+");
    mLLS* list = createmList();
    for (int i = 0; i<numThreads;i++){
        while (true) {
            sem_wait(&full);
            sem_wait(&mutex);
            tmp = do_get();
            sem_post(&mutex);
            sem_post(&empty);
            if (tmp->count != -1) {
                //fprintf(f, "ID: %d  Consumer - Item: %s:%d is extracted.\n", id,tmp->word, tmp->count);
            }
            else {
                free(tmp->word);
                free(tmp);
                break;
            }
            addmItem(list, tmp->word);
            free(tmp->word);
            free(tmp);
        }
    }
    SendmList(list);
    delmList(list);
    //fclose(f);
  return;
}

void *mapper(void* arg){
    //char* logname = malloc(sizeof(char)*30);
    //strcpy(logname, "log_thread_");
    //char tid[30];
    //snprintf(tid, sizeof(tid), "%d", (int)pthread_self());
    //strcat(logname, tid);
    //strcat(logname, ".txt");
    //FILE * g = fopen(logname, "a+");
    //free(logname);
    mLLS* map_list = mfileparse((const char *)arg);
    //FILE * f = fopen("log.txt", "a");
    //fprintf(g, "\n--- ---\nPID: %d, TID: %d, File: %s\nPrinting found words:\n--- ---\n", (int)getpid(), (int)pthread_self(), (char*)arg);
    //printmList(map_list, g);
    //fprintf(g, "\n--- ---\nend of list:\n--- ---\n");
    mNode* curNode = map_list->head;
// Injection from sem.c
    while (curNode !=NULL){
        int i = curNode->count;
        sem_wait(&empty);
        sem_wait(&mutex);
        do_fill(curNode->word, curNode->count);
        sem_post(&mutex);
        sem_post(&full);
        //fprintf(g, "Thread - Item: %s:%d is inserted\n",curNode->word, i);
        curNode = curNode->next;
    
    }
        sem_wait(&empty);
        sem_wait(&mutex);
        do_fill("final", -1);
        sem_post(&mutex);
        sem_post(&full);
//end injection from sem.c
    //fclose(g);
    delmList(map_list);
    return NULL;
}

void semaphore_init(){
    int i;
    //int con_ids[CMAX];
    fill = 0;
    use = 0;
    for (i = 0; i < max; i++) {
        buffer[i] = malloc(sizeof(buff));
        buffer[i]->word = malloc(sizeof(char)*MAXWORDSIZE)  ;
    }
    sem_init(&empty, 0, max); // max are empty 
    sem_init(&full, 0, 0);    // 0 are full
    sem_init(&mutex, 0, 1);   // mutex
    return;
}

void command(char * word){
    //mLLS *boundedmList = createmList(); // List for bounded buffer 
    LLS* files = createList();
    findtxt(word, files);
    Node* cursor = files->head;
    pthread_t tid[100];
    int thread_cnt = 0;

    /*FILE *f = fopen(process_fn, "a");
    if (f == NULL){
        fprintf(stderr, "fopen error in void command(char * word).");
        exit(0);
    }*/
    while (cursor!= NULL && thread_cnt < 99){ // for each child process, generate thread for each file
        int err = pthread_create(&tid[thread_cnt], NULL, mapper, (void *)cursor->word);
        //fprintf(f, "Created: TID:%d, From: PID:%d to handle directory: %s\n", (int)tid[thread_cnt], (int)getpid(), cursor->word );
        thread_cnt++;
        if (err != 0) {
            thread_cnt--;
            fprintf(stderr, "Error: %d detected when creating thread.\n", err);
        }
        cursor = cursor->next;
    }
    //fprintf(f, "Created: %d threads before consuming.\n", thread_cnt);
    //fclose(f);
    for (int i = 0; i<thread_cnt;i++){
        consumer(thread_cnt);
    }
    // before joining, consume contents of buffer 


    for (int i = 0;i<thread_cnt;i++ ){ // join threads
        void *res;
        pthread_join(tid[i], &res);
    }
    //printList(files, stderr);
    delList(files);
}

void cleanup(){
    for (int i = 0;i<max;i++){
        free(buffer[i]->word);
        free(buffer[i]);
    }
    //free(process_fn);
    return;
}

int main(int argc, char *argv[]){
    LLS* dir_cmds = getDirectories(argc, argv);
    Node *curNode = dir_cmds->head;
    pid_t pid[10];
    FILE *f = fopen("logMain.txt", "w");
    fprintf(f, "Found directories from map command are:\n");
    //printList(dir_cmds, f);
    fprintf(f, "\n\n");
    fclose(f);
    semaphore_init();
    IPC_init();
    int proc_cnt = 0;
    while (curNode != NULL){
        if (curNode->valid == true){ // generate separate process for each map directory
            pid[proc_cnt] = fork();
            //set_proc_fn();
            proc_cnt++;
            if (pid[proc_cnt-1] == 0){
                id = proc_cnt;
                command(curNode->word);
                break;
            }
            else{curNode = curNode->next;continue;}
        }
        curNode = curNode->next;
    }
    delList(dir_cmds);
    cleanup();
    return 0;
}

