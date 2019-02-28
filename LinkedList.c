#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedList{
    
    struct Node* head;
    struct Node* tail;

} LinkedList;


typedef struct Node {

    struct Node* next;
    unsigned pageNumber;
} Node;

void enqueue(LinkedList* linkedList, unsigned pageNum){
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    
    if(newNode == 0){
        fprintf(stderr, "Insufficient memory for new node.\n");
        exit(1);
    }
    
    newNode->pageNumber = pageNum;
    newNode->next = 0;
    if(linkedList->head == 0){
        linkedList->head = newNode;
        linkedList->tail = newNode;
    } else {
        linkedList->tail->next = newNode;
        linkedList->tail = newNode;
    }
}

unsigned dequeue(LinkedList* linkedList){
   
     unsigned pageNumber = linkedList->head->pageNumber;
     Node* oldHead = linkedList->head;
     linkedList->head = oldHead->next;
     free(oldHead);
    
} 

LinkedList* create(){

    LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    return linkedList;    

}

void printList(Node* current){


   if(current->next == 0){
        
        printf("%d\n", current->pageNumber);
        printf("\n");
        return;
    } else {
        
        printf("%d\n", current->pageNumber);
        printList(current->next);
    
    }


}/*
int main(int argc, char* argv[]){
    
    LinkedList* linkedList = create();
    enqueue(linkedList, 11111);
    enqueue(linkedList, 22222);
    enqueue(linkedList, 33333);
    enqueue(linkedList, 44444);
    printList(linkedList->head);
    dequeue(linkedList);
    printList(linkedList->head);
    return 0;
}*/
