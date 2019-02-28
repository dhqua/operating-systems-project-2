#ifndef LINKEDLIST
#define LINKEDLIST


typedef struct LinkedList{
    
    struct Node* head;
    struct Node* tail;

} LinkedList;


typedef struct Node {

    struct Node* next;
    unsigned pageNumber;
} Node;

void enqueue(LinkedList* linkedList, unsigned pageNum);


unsigned dequeue(LinkedList* linkedList);

LinkedList* create();

void printList(Node* current);

#endif
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
