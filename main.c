#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"


void read_inputs(const char* filename, node_t** processes) {

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
        process_t new_process;
        new_process.arrival_time = arrival_time;
        new_process.pid = pid;
        new_process.mem_size = mem_size;
        new_process.job_time = job_time;
        new_process.remaining_time = job_time;
        append(processes, new_process);
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
    struct Node* processes = NULL;

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
                printf("%d\n", atoi(optarg));
                break;
            case 'q':
                printf("%d\n", atoi(optarg));
                break;
            case '?':
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    for(; optind < argc; optind++){      
        printf("extra arguments: %s\n", argv[optind]);  
    } 

    read_inputs(filename, &processes);
    printList(processes);
    

    return 0; 
} 