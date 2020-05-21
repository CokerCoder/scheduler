#ifndef UNTITLED_PROCESS_H
#define UNTITLED_PROCESS_H


typedef struct process Process;
struct process
{
    int arrival_time;
    int pid;
    int mem_size;
    int job_time;
    int remaining_time;
};

Process new_process(int arrival_time, int pid, int mem_size, int job_time, int remaining_time);

void print_process(Process p);


#endif 