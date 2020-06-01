#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deque.h"
#include "algo.h"
#include "allocator.h"
#include "process.h"


// First come first serve algorithm
void ff(Deque* processes, Deque* ram_list, char* memory_alloc, int quantum) {

    assert(processes!=NULL);
    
    // Start the clock by 0
    int clock = 0;


    if (strcmp(memory_alloc, "u") == 0) {
        
        unlimited_allocator(processes, &clock, "ff", quantum);

    }
    
    else if (strcmp(memory_alloc, "p") == 0 || strcmp(memory_alloc, "v") == 0) {

        swapping_allocator(processes, ram_list, &clock, "ff", quantum);

    }
}



// Round-robin algorithm
void rr(Deque* processes, Deque* ram_list, Deque* pages, char* memory_alloc, int quantum) {

    assert(processes!=NULL);

    // Start the clock by 0
    int clock = 0;


    if (strcmp(memory_alloc, "u") == 0) {

        unlimited_allocator(processes, &clock, "rr", quantum);

    }


    else if (strcmp(memory_alloc, "p") == 0) {

        swapping_allocator(processes, ram_list, &clock, "rr", quantum);

    }

    else if (strcmp(memory_alloc, "v") == 0) {

        virtual_allocator(processes, pages, &clock, "rr", quantum);

    }

    else if (strcmp(memory_alloc, "cm") == 0) {

        custom_allocator(processes, pages, &clock, "rr", quantum);

    }
}



// Custom: Shortest job first
void cs(Deque* processes, Deque* ram_list, Deque* pages, char* memory_alloc, int quantum) {

    assert(processes!=NULL);

    // print_processes(processes);
    // Sort the processes queue based on their job time and pass it to first-come-first-serve algorithm
    sjf(processes);
    // printf("\nsort\n");
    // print_processes(processes);

    ff(processes, ram_list, memory_alloc, quantum);
}