// Operating Systems - Project 2
// Due: 4 March 2019
#ifndef LINKEDLIST
#define LINKEDLIST


// Data structure representing a linked list
typedef struct LinkedList{
    
    struct Node* head;
    struct Node* tail;
    int size;

} LinkedList;


//Data structure representing a node in a linkedList
typedef struct Node {

    struct Node* next;
    unsigned memAddress;
} Node;

// Adds a a memory address to the queue
// linkedList - pass the linked list/ queue that should be enqueued
// memAddressToAdd - the memory address that is being queued
void enqueue(LinkedList* linkedList, unsigned pageNum);

// Removes an element from the queue
// linkedList - the queue that we want to remove the element from
unsigned dequeue(LinkedList* linkedList);

// Initallized an empty linked list
LinkedList* create();

#endif
