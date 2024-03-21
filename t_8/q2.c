#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "queue.h"

#define MEMORY 1024

int avail_mem[MEMORY] = {0};

Queue *running;
int runcount = 0;

// Remove a node with the corresponding PID
Node *removePID(int pid, Queue *queue) {
    Node *current = queue->head;
    Node *parent = NULL;
    Node *temp;

    while (current != NULL && current->pid != pid) {
        parent = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Process with PID %d not found\n", pid);
        return NULL;
    }

    temp = current;

    if(queue->head == current && current->next == NULL){
        queue->head = NULL;
        queue->tail = NULL;
    }

    if (parent == NULL) {
        queue->head = current->next;
    } else {
        parent->next = current->next;
    }

    if (current == queue->tail && parent != NULL) {
        queue->tail = parent;
    }

    // Free memory of the removed node
    current->next = NULL;
    return temp;
}

// Check if there is a chunk of memory of specific size
int checkMEM(int size) {
    int index = 0;
    int check = 0;
    while (index < MEMORY) {
        check = 0;
        if (avail_mem[index] == 0) {
            int i;
            for (i = index; i < (index + size); i++) {
                if (avail_mem[(i%MEMORY)] == 1)
                    break;
            }
            check = i-index;
            if(check < 0){
                check += MEMORY;
            }
            if (check >= size)
                return index;
            else
                index = i;
        }
        index++;
    }
    return -1;
}

// Allocate a chunk of memory of a specific size at a specific address
void allocate(int address, int size) {
    int end = (size + address);
    for (int i = address; i < end; i++) {
        avail_mem[(i%MEMORY)] = 1;
    }
}

// Deallocate memory at an address of specific size
void deallocate(int address, int size) {
    int end = (size + address);
    for (int i = address; i < end; i++) {
        avail_mem[(i%MEMORY)] = 0;
    }
}

// Wait for all children to finish
void finishWait() {
    while (runcount > 0) {
        sleep(1);
    }
}

// Print contents of a node
void printProcess(Node *process, int type) {
    if(type == 0){
        printf("Process Name: %s | Priority: %d | PID: %d | MEM Size: %d | Runtime: %d | EXECUTING\n",
             process->name, process->priority, process->pid, process->memory, process->runtime);
    }else{
        printf("Name: %s | MEM Size: %d | Runtime: %d | END!!!\n\n", process->name, process->memory, process->runtime);
    }
}

//deallocate all memory
void clearMEM(){
    for(int i=0; i<MEMORY; i++){
        avail_mem[i] = 0;
    }
}

#ifdef SHOULD_NOT_BE_HERE
// Utilize interrupts when a child finishes executing:
// Remove the node and deallocate memory
void child_handler(int signum) {
    int pid;
    Node *node;
    printf("Child finished\n");
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        node = removePID(pid, running);
        printProcess(node, 1);
        runcount--;
        printf("runcount is %d\n",runcount);
        deallocate(node->address, node->memory);
        free(node);
    }
}
#endif

int main() {
    Queue *priority = createQueue();
    Queue *secondary = createQueue();
    running = createQueue();

    FILE *file;

    char name[NAME_MAX] = "\0";
    int prio;
    int mem;
    int runTime;

    file = fopen("processes_q2.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (1) {
        //read name, priority, memory, runtime from the comma delimited file
        fscanf(file, "%[^,], %d, %d, %d\n", name, &prio, &mem, &runTime);
        printf("name=%s, prio=%d, mem=%d, runtime=%d len=%lu\n", name, prio,mem,runTime,strlen(name));
        if (prio == 0) {
            enqueue(priority, name, prio, mem, runTime);
        } else {
            enqueue(secondary, name, prio, mem, runTime);
        }
        if(feof(file)){
            break;
        }
    }

    fclose(file);

#ifdef SHOULD_NOT_BE_HERE
    signal(SIGCHLD, child_handler);
#endif
    int memIndex = 0;
    int pid = 0;
    Node *current;
    Node *node;
    while (!isEmpty(priority)) {
        current = head(priority);
        if (current != NULL) {
            memIndex = checkMEM(current->memory);

            if (memIndex >= 0) {
                runcount++;
                dequeue(priority);
                allocate(memIndex, current->memory);

                current->address = memIndex;

                pid = fork();
                if (pid < 0) {
                    perror("Fork Error");
                    exit(1);
                }

                if (pid == 0) {
                    char runtime_str[20];
                    snprintf(runtime_str, sizeof(runtime_str), "%d", current->runtime);
                    execl("./process", "process", NULL);
                    perror("Process Exec Error (primary)");
                    exit(1);
                }

                current->pid = pid;
                printProcess(current, 0);
                insert(running, current);
            }
        }
    }
    while (!isEmpty(running)) {
#ifdef DEBUGGING
        printf("runcount is %d\n",runcount);
#endif
        current = head(running);
        while (current)
        {
            if (current->runtime > 1)
            {
                current->runtime--;
                current = current->next;
            }
            else
            {
                kill(current->pid, SIGTERM);
                waitpid(current->pid, NULL, 0);
                node = removePID(current->pid, running);
                printProcess(node, 1);
                runcount--;
#ifdef DEBUGGING
                printf("runcount is %d\n", runcount);
#endif
                deallocate(node->address, node->memory);
                free(node);
                current = current->next;
            }
        }
        sleep(1);
    }

    //primary (real time) queue is empty - continue with all secondary tasks
    bool lastdbg = false;
    while(!isEmpty(secondary) || !isEmpty(running)){
        // printf("------------------------------------------------------------1\n");
        // displayQueue(secondary);
        // printf("------------------------------------------------------------1\n");
        current = head(secondary);

        if (current != NULL) {
            memIndex = checkMEM(current->memory);

            if (memIndex >= 0) {
                runcount++;
                dequeue(secondary);
                // printf("------------------------------------------------------------2\n");
                // displayQueue(secondary);
                // printf("------------------------------------------------------------2\n");
                allocate(memIndex, current->memory);

                current->address = memIndex;

                pid = fork();
                if (pid < 0) {
                    perror("Fork Error Secondary");
                    exit(1);
                }

                if (pid == 0) {
                    execl("./process", "process", NULL);
                    perror("Process Exec Error (secondary)");
                    exit(1);
                }

                current->pid = pid;
                printProcess(current, 0);
#ifdef DEBUGGING
                printf("adding process to running queue\n");
                printf("queue before addition is\n");
                displayQueue(running);
#endif
                insert(running, current);
#ifdef DEBUGGING
                printf("queue after addition is\n");
                displayQueue(running);
                if (current->memory == 1024)  //debugging
                {
                    printf("adding last process to running queue\n");
                    lastdbg = true;
                }
#endif
                current->suspended = 0;
            }
        }
        if (!isEmpty(running))
        {
#ifdef DEBUGGING
            printf("current running queue is\n");
displayQueue(running);
#endif
            current = head(running);
            if(current->suspended == 1)
                kill(current->pid, SIGCONT);
            sleep(1);
            if (current->runtime >1) kill(current->pid, SIGTSTP);
#ifdef DEBUGGING
            if (lastdbg) printf("sent stop to last process\n");
#endif
            current->suspended = 1;
            current->runtime--;
#ifdef DEBUGGING
            if (lastdbg)
            {
                printf("current->runtime is %d\n", current->runtime);
            }
#endif
            if (current->runtime<=0){
                kill(current->pid, SIGINT);
                waitpid(current->pid, NULL, 0);
                node = removePID(current->pid, running);
                printProcess(node, 1);
                runcount--;
                deallocate(node->address, node->memory);
                free(node);
            }
            rotate(running);
        }       
    }

    return 0;
}

