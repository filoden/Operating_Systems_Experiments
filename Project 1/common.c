// use fscan/
// identify common words that appear in both files with a specified frequency
// words are seperated by spaces tabs and newlines

// *PER FILE*:
// build a seperate input linked list sorted in ascending order based on the word field
// Each node to contain a unique word and its count
// douubly linked and immediately sorted
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int THRESH =0;

typedef struct Node {
    char* word;
    int count;
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
    newNode->count = 0;
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

void addItem(LLS* list, char* word, int count){
    Node* newNode = createNode();
    newNode->count = count;
    newNode->word = word;
    if (list->cursor == NULL){
        list->head = newNode;
        list->cursor = newNode;
        return;
    }
    else{
        //printf("adding item\n");
        list->cursor->next = newNode;
        newNode->prev = list->cursor;
        list->cursor = newNode;
        return;
    }
}

void countItem(LLS* list, char* str){ // add to count if word is in list, otherwise create new node.
    Node * curNode = NULL;
    Node * prevNode = NULL;
    curNode = list->head;
    while (curNode != NULL){   //iterate through entire list
        if ((strcmp(str, curNode->word)) == 0 ){  // case: word is in list.
            curNode->count++;
            list->items++;
            return;
        }
        else if ((strcmp(str, curNode->word)) < 0 ){ // case: word is NOT in list
            break;
        }
        else{ // case: word not seen yet
            prevNode = curNode;
            curNode = curNode->next;
            continue;
        }
    }
    if (curNode == list->head){
        Node* newNode = createNode();
        if (list->head == NULL){
            list->head = newNode;
            newNode->word = malloc(strlen(str) + 1);
            strcpy(newNode->word, (char*)str);
            newNode->count = 1;
        }
        else{
            newNode->next = list->head;
            list->head = newNode;
            newNode->word = malloc(strlen(str) + 1);
            strcpy(newNode->word, (char*)str);
            newNode->count = 1;
            
        }
        list->items++;
        return;
    }

    if (curNode == NULL){ // add node to end of list
        curNode = prevNode;
        Node* newNode = createNode();
        curNode->next = newNode;
        newNode->word = malloc(strlen(str) + 1);
        strcpy(newNode->word, (char*)str);
        newNode->count = 1;
        list->items++;
        return;
    }
    Node* newNode = createNode();
    newNode->next = curNode;
    newNode->prev = prevNode;
    curNode->prev = newNode;
    prevNode->next = newNode;
    newNode->word = malloc(strlen(str) + 1);
    strcpy(newNode->word, (char*)str);
    newNode->count = 1;
    list->items++;
    return;
}

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
    int chk = fscanf(fptr, "%ms", &buffptr);
    while ((chk!= EOF) && (chk != 0)){
        if (buffptr == NULL){
            continue;
        }
        countItem(list, buffptr);
        free(buffptr);
        buffptr = NULL;
        chk = fscanf(fptr, "%ms", &buffptr);
    }
    free(buffptr);
    fclose(fptr);
    return list;
}

void printList(LLS* list, FILE* fptr){
    //printf("Printing List: \n");
    Node* curNode = NULL;
    curNode = list->head;
    while (curNode != NULL){
        fprintf(fptr, "%s,%d\n", curNode->word, curNode->count);
        curNode = curNode->next;
    }
    return;
}

LLS* compareFiles(LLS* l1, LLS* l2){
    Node* curNode1 = l1->head;
    Node* curNode2 = l2->head;
    LLS* list = createList();
    while ( (curNode1!= NULL) && (curNode2!= NULL) ) {
        int cmp = strcmp(curNode1->word, curNode2->word);
        if (cmp < 0){
            curNode1 = curNode1->next;
        }
        else if ( (cmp == 0) && (curNode1->count >= THRESH) && (curNode2->count >= THRESH)){
            addItem(list, (curNode1->word), (curNode1->count + curNode2->count));
            curNode1 = curNode1->next;
            curNode2 = curNode2->next;
            continue;
        }
        else{
            curNode2 = curNode2->next;
        }
    }
    return list;
}

void swap(Node* node){ // swaps a given node with the one preceeding it
    if (node->prev != NULL){
        Node* prevNode = node->prev;
        prevNode->next = node->next;  
        node->next->prev = node->prev;
        prevNode->prev->next = node;
        node->prev = prevNode->prev;
        node->next = prevNode;
        prevNode->prev = node;
    }
    else{
        perror("\nAttempted swap of header node.\n");
        exit(1);
    }
}

void swap2(Node* n1, Node* n2){
    Node* temp1p = n1->prev;
    Node* temp2p = n2->next;
    Node* temp1n = n1->next;
    Node* temp2n = n2->prev;
    n1->next = temp2n;
    n1->prev = temp2p;
    n2->next = temp1n;
    n2->prev = temp1p;
    n2->prev->next = n2;
    n2->next->prev = n2;
    n1->prev->next = n1;
    n1->next->prev = n1;
    return;
}

void insert(Node* n1, Node* n2, LLS* list){ // node to be inserted (n1) and the node it is to be inserted before (n2)
    // if (n1->next == n2){
    //     perror("attempting insert on alread sorted items");
    //     exit(0);
    // }
    n1->prev->next = n1->next;
    if (n1->next != NULL){
        n1->next->prev = n1->prev;
    }
    if (n2->prev == NULL){ // case: 1st node
        list->head = n1;
        n1->next = n2;
        n1->prev = NULL;
        n2->prev = n1;
        return;
    }
    else{ // case: middle node
        n1->next = n2;
        n1->prev = n2->prev;
        n2->prev = n1;
        n1->prev->next = n1;
    }
}
void insertionSort(LLS* list){
    Node* cur = NULL;
    Node* cmp = NULL;
    cur = list->head->next;
    cmp = list->head;
    while (cur != NULL){
        while (cmp != NULL){
            if (cmp->count < cur->count){
                insert(cur, cmp, list);
                break;
            }
            else{
                cmp = cmp->next;
                if (cmp == cur){
                    break;
                }
            }
        }
        cur = cur->next;
        cmp = list->head;
    }
}

int main(int argc, char* argv[]){
    const char* fn1 = argv[1];
    const char* fn2 = argv[2];
    THRESH = atoi(argv[3]);
    //printf("THRESH is %d", THRESH);
    //printf("Hello\n");
    LLS* l1 = NULL;
    LLS* l2 = NULL;
    l1 = fileparse(fn1);
    l2 = fileparse(fn2);
    LLS* l3 = compareFiles(l1, l2);
    insertionSort(l3);
    const char* f_out = argv[4];
    FILE * fptr = fopen(f_out, "w");
    printList(l3, fptr);
    fclose(fptr);
    delList(l1);
    delList(l2);
    delList(l3);
    return 1;
}


