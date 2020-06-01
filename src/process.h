#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include "deque.h"


typedef struct process Process;
struct process
{
    long arrival_time;
    long pid;
    long mem_size;
    long job_time;
    long remaining_time;
    long finish_time;
    long last_access;
};

Process* new_process(long arrival_time, long pid, long mem_size, long job_time);

void move_to_last(Deque* processes, long pid, long clock);

void print_processes(Deque *processes);

void print_processes_reversed(Deque *processes);

long proc_remaining(Deque* processes, long clock);

Node* next_running_process(Deque* processes);

void print_stats(Deque* processes);

void swap(void** a, void** b);
void sjf(Deque* processes);
void sort_pid(Deque* processes);

long least_used_id(Deque* processes, long running_id);
long recent_used_id(Deque* processes, long running_id);

void update_access(Deque* processes, long evicted_id);


#endif 