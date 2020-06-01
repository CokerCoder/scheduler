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

// Move the process with pid to the last possible position (after the process that has arrived the most recently)
void move_to_last(Deque* processes, long pid, long clock);

void print_processes(Deque *processes);
void print_processes_reversed(Deque *processes);

// Number of processes remaining before a given time
long proc_remaining(Deque* processes, long clock);
// Next process should be running
Node* next_running_process(Deque* processes);

void print_stats(Deque* processes);

void swap(void** a, void** b);

// "Shortest job first": sort the processes by job time if two or more processes arrived in the same time
void sjf(Deque* processes);
// Sort the processes by pid if two or more processes arrived in the same time
void sort_pid(Deque* processes);

long least_used_id(Deque* processes, long running_id);
long recent_used_id(Deque* processes, long running_id);

// Update the access time to -1 when evicted all of its pages or finished 
void update_access(Deque* processes, long evicted_id);


#endif 