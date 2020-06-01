#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "algo.h"
#include "process.h"
#include "ram.h"
#include "page.h"
#include "deque.h"


void read_inputs(const char* filename, Deque* processes) {

    long arrival_time = 0;
    long pid = 0;
    long mem_size = 0;
    long job_time = 0;

    FILE* file = fopen(filename, "r");
    if (file==NULL) {
        printf("No such file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%ld %ld %ld %ld", &arrival_time, &pid, &mem_size, &job_time)==4) {
        Process* process = new_process(arrival_time, pid, mem_size, job_time);
        deque_insert(processes, process);
    }
    fclose(file);
}
  

int main(int argc, char *argv[])  
{ 

    char filename[50] = "";
    char scheduling_algo[5] = "";
    char memory_allo[5] = "";
    long memory_size = 0;
    long quantum = 10;


    Deque* processes = new_deque();
    if (processes == NULL) {
        fprintf(stderr, "Error: Failed to initialize new processes deque\n");
        exit(EXIT_FAILURE);
    }

    Deque* ram_list = new_deque();
    if (ram_list == NULL) {
        fprintf(stderr, "Error: Failed to initialize new ram deque\n");
        exit(EXIT_FAILURE);
    }

    Deque* pages = new_deque();
    if (pages == NULL) {
        fprintf(stderr, "Error: Failed to initialize new page deque\n");
        exit(EXIT_FAILURE);
    }
    

    // Read the command line arguments
    long opt;   
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

    read_inputs(filename, processes);
    init_ram(ram_list, memory_size);
    init_pages(pages, memory_size);

    sort_pid(processes);

    if (strcmp(scheduling_algo, "ff") == 0) {
        ff(processes, ram_list, memory_allo, quantum);
    }
    else if (strcmp(scheduling_algo, "rr") == 0) {
        rr(processes, ram_list, pages, memory_allo, quantum);
    }
    else if (strcmp(scheduling_algo, "cs") == 0) {
        cs(processes, ram_list, pages, memory_allo, quantum);
    }
    
    // print_processes(processes);
    print_stats(processes);
    // print_pages(pages);

    free_deque(processes);
    free_deque(ram_list);
    free_deque(pages);
    return 0; 
} 