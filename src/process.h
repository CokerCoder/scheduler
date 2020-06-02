#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H

#include "deque.h"


typedef struct process Process;
struct process
{
    long long int arrival_time;
    long long int pid;
    long long int mem_size;
    long long int job_time;
    long long int remaining_time;
    long long int finish_time;
    long long int last_access;
};

Process* new_process(long long int arrival_time, long long int pid, long long int mem_size, long long int job_time);

// Move the process with pid to the last possible position (after the process that has arrived the most recently)
void move_to_last(Deque* processes, long long int pid, long long int clock);

void print_processes(Deque *processes);
void print_processes_reversed(Deque *processes);

// Number of processes remaining before a given time
long long int proc_remaining(Deque* processes, long long int clock);
// Next process should be running
Node* next_running_process(Deque* processes);

void print_stats(Deque* processes);

void swap(void** a, void** b);

// "Shortest job first": sort the processes by job time if two or more processes arrived in the same time
void sjf(Deque* processes);
// Sort the processes by pid if two or more processes arrived in the same time
void sort_pid(Deque* processes);

long long int least_used_id(Deque* processes, long long int running_id);
long long int recent_used_id(Deque* processes, long long int running_id);

// Update the access time to -1 when evicted all of its pages or finished 
void update_access(Deque* processes, long long int evicted_id);


#endif 