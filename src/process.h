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
    int last_access;
};

Process* new_process(int arrival_time, int pid, int mem_size, int job_time);

void move_to_last(Deque* processes, int pid, int clock);

void print_processes(Deque *processes);

void print_processes_reversed(Deque *processes);

int proc_remaining(Deque* processes, int clock);

Node* next_running_process(Deque* processes);

void print_stats(Deque* processes);

void swap(void** a, void** b);
void sjf(Deque* processes);

int least_used_id(Deque* processes, int running_id);
int recent_used_id(Deque* processes, int running_id);

void update_access(Deque* processes, int evicted_id);


#endif 