#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 256

//queue node structure
typedef struct Node {
    char name[NAME_MAX];
    int priority;
    int pid;
    int address;
    int memory;
    int runtime;
    bool suspended;
    struct Node* next;
} Node;

//queue structure
typedef struct {
    Node* head;
    Node* tail;
} Queue;

//create a new queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

//check if the queue is empty
int isEmpty(Queue* queue) {
    return queue->head == NULL;
}

//queue an element
void enqueue(Queue* queue, char name[], int priority, int memory, int runtime) {
    Node* newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->priority = priority;
    newNode->memory = memory;
    newNode->runtime = runtime;
    newNode->pid = 0;
    newNode->address = 0;
    newNode->suspended = 0;
    newNode->next = NULL;
    if (isEmpty(queue)) {
        queue->head = newNode;
        queue->tail = newNode;
    } else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
}

void insert(Queue *queue,Node *node){
    if(isEmpty(queue)){
        queue->head = node;
        queue->tail = node;
    }else{
        node->next = NULL;
        queue->tail->next = node;
        queue->tail = node;
    }
}

// void insertHead(Queue *queue,Node *node){
//     if(isEmpty(queue)){
//         queue->head = node;
//         queue->tail = node;
//     }else{
//         node->next = queue->head;
//         queue->head = node;
//     }
// }

//dequeue an element
char *dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty!\n");
        exit(1);
    }
    Node* temp = queue->head;
    char *name = temp->name;
    queue->head = queue->head->next;
    free(temp);
    return name;
}

//move node from head of queue to tail
void rotate(Queue *queue) {
    if (isEmpty(queue) || queue->head == queue->tail) {
        // If the queue is empty or has only one element, no rotation needed
        return;
    }

    Node *temp = queue->head;
    queue->head = queue->head->next;
    temp->next = NULL; 

    queue->tail->next = temp; 
    queue->tail = temp; 
}

//get the head element of the queue
Node *head(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty!\n");
        exit(1);
    }
    return queue->head;
}

//display the queue
void displayQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Queue elements: ");
    Node* current = queue->head;
    while (current != NULL) {
        printf("%s \n", current->name);
        current = current->next;
    }
    printf("\n");
}

//free memory allocated to the queue
void destroyQueue(Queue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void emptyQueue(Queue *queue){
    if(isEmpty(queue)){
        return;
    }
    // Node* current = queue->head;
    // while (current != NULL) {
    //     Node* temp = current;
    //     current = current->next;
    //     free(temp);
    // }
    queue->head = NULL;
    queue->tail = NULL;
}