#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "algo.h"
#include "process.h"
#include "deque.h"


void read_inputs(const char* filename, Deque* deque) {

    int arrival_time = 0;
    int pid = 0;
    int mem_size = 0;
    int job_time = 0;

    FILE* file = fopen(filename, "r");
    if (file==NULL) {
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d %d %d %d", &arrival_time, &pid, &mem_size, &job_time)==4) {
        Process process = new_process(arrival_time, pid, mem_size, job_time, job_time);
        deque_insert(deque, process);
    }
    fclose(file);
}
  

int main(int argc, char *argv[])  
{ 

    char filename[50] = "";
    char scheduling_algo[5] = "";
    char memory_allo[5] = "";
    int memory_size = 0;
    int quantum = 10;

    // Initialize empty doubly linked list
    Deque* deque = new_deque();
    if (deque == NULL) {
        fprintf(stderr, "Error: new_deque() returned NULL\n");
        exit(EXIT_FAILURE);
    }

    // Read the command line arguments
    int opt;   
    while((opt = getopt(argc, argv, ":f:a:m:s:q:")) != -1)  
    {  
        switch(opt)  
        {   
            case 'f':
                strcpy(filename, optarg);
                break;  
            case 'a':
                strcpy(scheduling_algo, optarg);
                break;
            case 'm':
                strcpy(memory_allo, optarg);  
                break;
            case 's':
                memory_size = atoi(optarg);
                break;
            case 'q':
                quantum = atoi(optarg);
                break;
            case '?':
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    for(; optind < argc; optind++){      
        printf("extra arguments: %s\n", argv[optind]);  
    } 

    read_inputs(filename, deque);

    if (strncmp(scheduling_algo, "ff", 2)==0) {
        ff(deque, memory_allo, memory_size, quantum);
    }
    else if (strncmp(scheduling_algo, "rr", 2)==0) {
        rr(deque, memory_allo, memory_size, quantum);
    }

    // print_deque(deque);

    return 0; 
} 