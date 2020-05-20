#ifndef UNTITLED_LIST_H
#define UNTITLED_LIST_H


typedef struct Process
{
    int arrival_time;
    int pid;
    int mem_size;
    int job_time;
    int remaining_time;
} process_t;

typedef struct Node node_t;

typedef struct Node { 
    process_t process;
    node_t* next; 
    node_t* prev; 
} node_t;

void push(node_t** head_ref, process_t new_process);
void insertBefore(node_t** head_ref, node_t* next_node, process_t new_process);
void insertAfter(node_t* prev_node, process_t new_process);
void append(node_t** head_ref, process_t new_process);
void deleteNode(node_t** head_ref, node_t* del);
void printList(node_t* node);


#endif 