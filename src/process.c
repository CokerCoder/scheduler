#include <stdio.h>

#include "process.h"


Process new_process(int arrival_time, int pid, int mem_size, int job_time, int remaining_time) {
    Process process = {arrival_time, pid, mem_size, job_time, remaining_time};
    return process;
}


void print_process(Process p) {
    printf("printed");
}