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
};

Process* new_process(int arrival_time, int pid, int mem_size, int job_time, int remaining_time);
void move_to_last(Deque* processes, int clock);

void print_processes(Deque *processes);

int proc_remaining(Deque* processes, int clock);


#endif 