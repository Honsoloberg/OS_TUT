#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"

#define MEMORY 1024

int avail_mem[MEMORY] = {0};

Queue *running;
int runcount = 0;

//remove a node with the corresponding PID
Node *removePID(int pid, Queue *queue){
    Node *current = queue->head;
    while(current->next != NULL){
        if(current->pid == pid){
            removeNode(queue, current);
            break;
        }
    }
    return current;
}

//check if there is a chunk of memory of specific size
int checkMEM(int size){
    int index = 0;
    while (index < MEMORY){
        
        if(avail_mem[index] == 0){

            for(int i=index; i<MEMORY; i++){

                if(avail_mem[i] == 1){
                    index = i;
                    break;
                }

                if(i>=size){
                    return index;
                }
            }
        }
        index++;
    }
    return -1;
}

//allocate a chunk of memory of a specific size at a specific address
void allocate(int address, int size){
    for(int i=address; i<(size+address); i++){
        avail_mem[i] = 1;
    }
    //TEST CODE
    for(int i=0; i<MEMORY; i++){
        printf("%d,", avail_mem[i]);
    }
    printf("\n");
}

//deallocate memory at an address of specific size
void deallocate(int address, int size){
    for(int i=address; i<(size+address); i++){
        avail_mem[i] = 0;
    }
    //TEST CODE
    for(int i=0; i<MEMORY; i++){
        printf("%d,", avail_mem[i]);
    }
    printf("\n");
}

//wait for all children to finish
void finishWait(){
    while(runcount != 0){
        sleep(0.5);
    }
}

//print contents of a node
void printProcess(Node *process){
    printf("Process:\n");
    printf("Name: %s | Priority: %d | PID: %d | MEM Size: %d | Runtime: %d |\n", process->name, process->priority, process->pid, process->memory, process->runtime);
}

//utilize interupts when a child finishes executing:
//remove the node and deallocate memory
void child_handler(int signum){
    int pid;
    Node *node;

    while((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        node = removePID(pid, running);
        runcount--;
        deallocate(node->address, node->memory);
    }
}


int main(){
    Queue *priority = createQueue();
    Queue *secondary = createQueue();
    running = createQueue();

    FILE *file;

    char name[256];
    int prio;
    int mem;
    int runTime;

    while(fscanf(file, "%255s, %d, %d, %d", &name, &prio, &mem, &runTime)){
        if(priority == 0){
            enqueue(priority, name, prio, mem, runTime);
        }else{
            enqueue(secondary, name, prio, mem, runTime);
        }
    }

    fclose(file);

    signal(SIGCHLD, child_handler);


    int memIndex = 0;
    int pid = 0;
    int status;
    Node *current;
    while(!isEmpty(priority)){
        
        current = head(priority);
        if(current != NULL){
            memIndex = checkMEM(current->memory);

            if(memIndex >= 0){
                runcount++;
                dequeue(priority);
                allocate(memIndex, current->memory);
                
                current->address = memIndex;
                
                printProcess(current);
                pid = fork();
                if(pid < 0){
                    perror("Fork Error");
                    exit(1);
                }

                if(pid == 0){
                    execv("sigtrap.exe", (char *)current->runtime);
                }

                current->pid = pid;
                insert(running, current);
            }
        }
    }

    finishWait();
}