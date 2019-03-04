// Cedric Nicolas and Qua'on Thomas
// Operating Systems - Project 2
// Due: 4 March 2019
#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"


// Adds a a memory address to the queue
// linkedList - pass the linked list/ queue that should be enqueued
// memAddressToAdd - the memory address that is being queued
void enqueue(LinkedList* linkedList, unsigned memAddressToAdd){
    
    // Creates new node to hold the info being queued
    Node* newNode = (Node*)malloc(sizeof(Node));
    
    //Checks if malloc above failed
    if(newNode == 0){
        fprintf(stderr, "Insufficient memory for new node.\n");
        exit(1);
    }
    
    newNode->memAddress = memAddressToAdd;
    newNode->next = 0;
    //Checks if list is empty, if so make the new node the head and the tail
    //else - add the new node to the existing list
    if(linkedList->head == 0){
        linkedList->head = newNode;
        linkedList->tail = newNode;
    } else {
        linkedList->tail->next = newNode;
        linkedList->tail = newNode;
    }
    
     linkedList->size += 1;
}

// Removes an element from the queue
// linkedList - the queue that we want to remove the element from
unsigned dequeue(LinkedList* linkedList){
   
     unsigned memAddress = linkedList->head->memAddress;
     Node* oldHead = linkedList->head;
     linkedList->head = oldHead->next;
     free(oldHead);
      linkedList->size -= 1;
     return memAddress;    
} 

// Initallized an empty linked list
LinkedList* create(){

    LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    linkedList->size = 0;
    return linkedList;    

}

