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
#include <errno.h>

key_t key;
int num_senders = 9999;
int min_freq = 0;

static int comes_before(mNode *a, mNode *b) {
    if (a->count > b->count) return 1;
    if (a->count < b->count) return 0;

    // tie: alphabetical order
    return strcmp(a->word, b->word) < 0;
}

static void detachNode(mNode* node, mLLS* list) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->cursor = node->prev;
    }

    node->prev = NULL;
    node->next = NULL;
}

static void insertBefore(mNode* destNode, mNode* srcNode, mLLS* list) {
    srcNode->next = destNode;
    srcNode->prev = destNode->prev;

    if (destNode->prev != NULL) {
        destNode->prev->next = srcNode;
    } else {
        list->head = srcNode;
    }

    destNode->prev = srcNode;
}

static void insertAfter(mNode* destNode, mNode* srcNode, mLLS* list) {
    srcNode->prev = destNode;
    srcNode->next = destNode->next;

    if (destNode->next != NULL) {
        destNode->next->prev = srcNode;
    } else {
        list->cursor = srcNode;
    }

    destNode->next = srcNode;
}

void endTransaction(int qid){
    msgctl(qid, IPC_RMID, NULL);
    return;
}

int startTransaction(){
    int qid = msgget(key, 0666);
    if (qid == -1) {
        fprintf(stderr, "ERRNO: %d \n", errno);
        perror("msgget failure. Unable to start messaging. id:1\n");
        exit(0);
    }
    else{
        //fprintf(stderr, "Receiver is ready for messaging.\n");
    }
    return qid;
}

int recmsg(int qid, mLLS* list){
    if (num_senders == 9999){
        num_senders = 0;
    }
    message msg;
    if (msgrcv(qid, &msg, sizeof(msg) - sizeof(msg.mtype), 1, 0) == -1) {
        perror("msgrcv");
        return 1;
    }
    if (msg.cnt == end_msg_num){
        if (!(strncmp(msg.word, end_msg_str, 20))){
            //fprintf(stderr, "We got one endin'.\n");
            return 1;
        }
        
    }
    else if (msg.cnt == start_msg_num){
        if (!(strncmp(msg.word, start_msg_str, 20))){
            num_senders++;
            return 0;
        }
        
    }
    mNode* newNode = createmNode();
    newNode->word = malloc(sizeof(msg.word));
    strcpy(newNode->word, msg.word);
    newNode->count = msg.cnt;
    if (list->cursor == NULL){
        list->head = newNode;
        list->cursor = newNode;
        list->items++;
        return 0;
    }
    else{
        list->cursor->next = newNode;
        newNode->prev = list->cursor;
        list->cursor = newNode;
        list->items++;
        return 0;
    }
    return 0;
}

mLLS* RecmList(){
    int qid = startTransaction();
    mLLS *rec_list = createmList();
    int count = 0;
    while (count < num_senders){
        count += recmsg(qid, rec_list);
    }
    endTransaction(qid);
    return rec_list;

}

void delmNodeinlist(mNode* node, mLLS* list){
    if (node == NULL) return;

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->cursor = node->prev;
    }

    list->items--;
    delmNode(node);
}

void combine_duplicates(mLLS* list){
    mNode* curNode = list->head;
    while (curNode != NULL){
        mNode* tempNode = curNode->next;
        while (tempNode != NULL){
            mNode* nextNode = tempNode->next;
            if (strcmp(curNode->word, tempNode->word) == 0){
                curNode->count += tempNode->count;
                delmNodeinlist(tempNode, list);
            }
            tempNode = nextNode;
        }
        curNode = curNode->next;
    }
}


void insertmNode(mNode* destNode, mNode* srcNode, mLLS* list){
    if (destNode->prev == NULL){
        destNode->prev = srcNode;
        srcNode->next = destNode;
        srcNode->prev = NULL;
        list->head = srcNode;
        return;
    }
    else{
        mNode* prevNode = destNode->prev;
        destNode->prev = srcNode;
        srcNode->next = destNode;
        prevNode->next = srcNode;
        srcNode->prev = prevNode;
    }
}

void bubbleup(mNode* Node, mLLS* list){
    mNode* tmpNode = Node->prev;
    while ((Node->count > tmpNode->count)){
        if (tmpNode->prev == NULL){
            break;
        }
        tmpNode = tmpNode->prev;
    }
    insertmNode(tmpNode, Node, list);
}

void sortfreq(mLLS* list){
    if (list == NULL || list->head == NULL || list->head->next == NULL) {
        return;
    }

    mNode* cur = list->head->next;

    while (cur != NULL) {
        mNode* next = cur->next;

        if (comes_before(cur, cur->prev)) {
            mNode* scan = cur->prev;

            detachNode(cur, list);

            while (scan->prev != NULL && comes_before(cur, scan)) {
                scan = scan->prev;
            }

            if (comes_before(cur, scan)) {
                insertBefore(scan, cur, list);
            } else {
                insertAfter(scan, cur, list);
            }
        }

        cur = next;
    }
}

void printmListfreq(mLLS* list , FILE* f){
    mNode* curNode = list->head;
    while (curNode!= NULL){
        if (curNode->count >= min_freq){
            fprintf(f, "%s:%d\n", curNode->word, curNode->count);
        }
        curNode=curNode->next;
    }
}

int main(int argc, char *argv[]){
    IPC_init();
    min_freq = atoi(argv[2]);
    mLLS* final_list = RecmList();
    //printmList(final_list, stderr);
    combine_duplicates(final_list);
    sortfreq(final_list);
    FILE *f = fopen(argv[1], "w");
    printmListfreq(final_list, f);
    delmList(final_list);
    fclose(f);
    return 0;
}