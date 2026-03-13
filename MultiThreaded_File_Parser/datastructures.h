#ifndef __DATASTRUCTURES_h__
#define __DATASTRUCTURES_h__
#include <pthread.h>
#include <sys/ipc.h>
#define MAXWORDSIZE 256

typedef struct mNode{
    char* word;
    int count;
    struct mNode* next;
    struct mNode* prev;
    bool valid;
} mNode;

typedef struct mLLS{
    struct mNode* cursor;
    struct mNode* head;
    int items;
} mLLS;

typedef struct message{
    long mtype;
    char word[MAXWORDSIZE];
    int cnt;
} message;

const char end_msg_str[] = "5Very4Specific3And2Unlikely1Word6";
const int end_msg_num = -1;
const char start_msg_str[] = "Even6More5Very4Specific3And2Unlikely1Word6";
const int start_msg_num = -33;
extern key_t key;

mNode* createmNode() {
    mNode* newNode = malloc(sizeof(mNode));
    if (newNode == NULL){
        perror("\nmalloc allocation failure.\n");
        exit(1);
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->valid = false;
    return newNode;
}

void delmNode(mNode* delNode){
    if (delNode != NULL){
        free(delNode->word);
        free(delNode);
    }
    return;
}

void delmList(mLLS* list){
    mNode* cursor = list->head;
    mNode* tempNode = NULL;
    if (list != NULL){
    while(cursor != NULL){
        tempNode = cursor;
        cursor = cursor->next;
        delmNode(tempNode);
    }
    free(list);
    }
    return;
}

mLLS* createmList(){
    mLLS* lls = malloc(sizeof(mLLS));
    if (lls == NULL){
        perror("\nmalloc allocation failure.\n");
        exit(1);
    }
    lls->head = NULL;
    lls->cursor = lls->head;
    lls->items = 0;
    return lls;
}

void addmItem(mLLS* list, char* word){
    mNode* newNode = createmNode();
    void *temp = malloc(strlen(word) + 1);
    if (temp == NULL){
        fprintf(stderr, "Malloc in void addmItem() failure.");
    }
    newNode->word = temp;
    newNode->count = 1;
    strcpy(newNode->word, (char*)word);
    if (list->cursor == NULL){
        list->head = newNode;
        list->cursor = newNode;
        list->items++;
        return;
    }
    else{
        list->cursor->next = newNode;
        newNode->prev = list->cursor;
        list->cursor = newNode;
        list->items++;
        return;
    }
}


typedef struct Node {
    char* word;
    bool valid;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LLS{
    Node* cursor;
    Node* head;
    int items;
} LLS;

Node* createNode() {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL){
        perror("\nmalloc allocation failure.\n");
        exit(1);
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->valid = false;
    return newNode;
}

void delNode(Node*delNode){
    if (delNode != NULL){
        free(delNode->word);
        free(delNode);
    }
    return;
}

void delList(LLS* list){
    Node* cursor = list->head;
    Node* tempNode = NULL;
    if (list != NULL){
    while(cursor != NULL){
        tempNode = cursor;
        cursor = cursor->next;
        delNode(tempNode);
    }
    free(list);
    }
    return;
}

LLS* createList(){
    LLS* lls = malloc(sizeof(LLS));
    if (lls == NULL){
        perror("\nmalloc allocation failure.\n");
        exit(1);
    }
    lls->head = NULL;
    lls->cursor = lls->head;
    lls->items = 0;
    return lls;
}

void addItem(LLS* list, char* word){
    Node* newNode = createNode();
    void *temp = malloc(strlen(word) + 1);
    if (temp == NULL){
        fprintf(stderr, "Malloc in void addItem() failure.");
    }
    newNode->word = temp;
    strcpy(newNode->word, (char*)word);
    if (list->cursor == NULL){
        list->head = newNode;
        list->cursor = newNode;
        return;
    }
    else{
        list->cursor->next = newNode;
        newNode->prev = list->cursor;
        list->cursor = newNode;
        return;
    }
}

void printmList(mLLS* list, FILE* fptr){
    mNode* curNode = NULL;
    curNode = list->head;
    while (curNode != NULL){
        
        fprintf(fptr, "%s ", curNode->word);
        curNode = curNode->next;
    }
    return;
}

void printList(LLS* list, FILE* fptr){
    Node* curNode = NULL;
    curNode = list->head;
    while (curNode != NULL){
        fprintf(fptr, "word: %s\n", curNode->word);
        curNode = curNode->next;
    }
    return;
}

void IPC_init(){
    key = ftok("mapper.c", (int)'X');
    if (key == -1){
        perror("ftok key generation failure.\n");
        exit(0);
    }
    return;
}


#endif