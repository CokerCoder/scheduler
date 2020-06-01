#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include "deque.h"


typedef struct process Process;
struct process
{
    __int64_t arrival_time;
    __int64_t pid;
    __int64_t mem_size;
    __int64_t job_time;
    __int64_t remaining_time;
    __int64_t finish_time;
    __int64_t last_access;
};

Process* new_process(__int64_t arrival_time, __int64_t pid, __int64_t mem_size, __int64_t job_time);

// Move the process with pid to the last possible position (after the process that has arrived the most recently)
void move_to_last(Deque* processes, __int64_t pid, __int64_t clock);

void print_processes(Deque *processes);
void print_processes_reversed(Deque *processes);

// Number of processes remaining before a given time
__int64_t proc_remaining(Deque* processes, __int64_t clock);
// Next process should be running
Node* next_running_process(Deque* processes);

void print_stats(Deque* processes);

void swap(void** a, void** b);

// "Shortest job first": sort the processes by job time if two or more processes arrived in the same time
void sjf(Deque* processes);
// Sort the processes by pid if two or more processes arrived in the same time
void sort_pid(Deque* processes);

__int64_t least_used_id(Deque* processes, __int64_t running_id);
__int64_t recent_used_id(Deque* processes, __int64_t running_id);

// Update the access time to -1 when evicted all of its pages or finished 
void update_access(Deque* processes, __int64_t evicted_id);


#endif 