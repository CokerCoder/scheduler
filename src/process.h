#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include "deque.h"


typedef struct process Process;
struct process
{
    int arrival_time;
    int pid;
    int mem_size;
    int job_time;
    int remaining_time;
    int finish_time;
};

Process* new_process(int arrival_time, int pid, int mem_size, int job_time);

void move_to_last(Deque* processes, int pid, int clock);

void print_processes(Deque *processes);

void print_processes_reversed(Deque *processes);

int proc_remaining(Deque* processes, int clock);

Node* next_running_process(Deque* processes);


#endif 