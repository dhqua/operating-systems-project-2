#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"


void enqueue(LinkedList* linkedList, unsigned pageNum){
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    
    if(newNode == 0){
        fprintf(stderr, "Insufficient memory for new node.\n");
        exit(1);
    }
    
    newNode->memAddress = pageNum;
    newNode->next = 0;
    if(linkedList->head == 0){
        linkedList->head = newNode;
        linkedList->tail = newNode;
    } else {
        linkedList->tail->next = newNode;
        linkedList->tail = newNode;
    }
    
     linkedList->size += 1;
}

unsigned dequeue(LinkedList* linkedList){
   
     unsigned memAddress = linkedList->head->memAddress;
     Node* oldHead = linkedList->head;
     linkedList->head = oldHead->next;
     free(oldHead);
      linkedList->size -= 1;
     return memAddress;    
} 

LinkedList* create(){

    LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    linkedList->size = 0;
    return linkedList;    

}

void printList(Node* current){


   if(current->next == 0){
        
        printf("%d\n", current->memAddress);
        printf("\n");
        return;
    } else {
        
        printf("%d\n", current->memAddress);
        printList(current->next);
    
    }


}

/*
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
