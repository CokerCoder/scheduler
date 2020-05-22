#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"



Process* new_process(int arrival_time, int pid, int mem_size, int job_time, int remaining_time) {
    
    Process* process = (Process *) malloc(sizeof(Process));
    assert(process);

    process->arrival_time = arrival_time;
    process->pid = pid;
    process->mem_size = mem_size;
    process->job_time = job_time;
    process->remaining_time = job_time;

    return process;
}


// Move the top process to the last available position (only if the data is type of process)
void move_to_last(Deque* processes, int clock) {

    assert(processes!=NULL);
    assert(processes->size>0);

    // If there is only one node ledt, nothing can be done
    if (processes->size==1) {
        return;
    }

    Node* first = processes->head;
    Node* last = processes->tail;

    Node* curr = processes->head;


    // After this loop, curr will be the next arriving process, so we will insert the first process before curr
    while (curr!=NULL && ((Process *)curr->data)->arrival_time <= clock) {
        curr = curr->next;
    }

    // If no processes arrives, simply return
    if (curr==first) {
        return;
    }

    // If curr is NULL, means all processes have arrived and simply insert the first process to the end
    if (curr==NULL) {
        processes->head = first->next;
        first->next = NULL;
        last->next = first;
        processes->tail = last->next;
        return;
  }

    // Otherwise, move the first process before the curr (next arriving) process
    // Change the head pointer to point to second node now
    processes->head = first->next;

    first->prev = curr->prev;
    curr->prev = first;
    
    // Set the next of first as curr
    first->next = curr;
    first->prev->next = first;
}


// Helper function to print the deque when debugging
void print_processes(Deque *processes) {
    assert(processes!=NULL);

    Node *curr = processes->head;
    
    printf("pid      arrival      memory      jobtime      timeleft\n");
    while (curr != NULL) {
        printf("aa");
        Process* curr_process = (Process *) curr->data;
        printf("%-12d%-12d%-12d%-12d%-12d\n", \
        curr_process->pid, curr_process->arrival_time, curr_process->mem_size, curr_process->job_time, curr_process->remaining_time); 
        curr = curr->next; 
    } 
    printf("\n");
}