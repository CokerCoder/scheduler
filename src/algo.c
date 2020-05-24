#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deque.h"
#include "algo.h"
#include "allocator.h"


// First come first serve algorithm
void ff(Deque* processes, Deque* ram_list, char* memory_alloc, int quantum) {

    assert(processes!=NULL);
    
    // Start the clock by 0
    int clock = 0;


    if (strcmp(memory_alloc, "u") == 0) {
        
        unlimited_allocator(processes, &clock, "ff", quantum);

    }
    
    if (strcmp(memory_alloc, "p") == 0) {

        swapping_allocator(processes, ram_list, &clock, "ff", quantum);

    }
}



// Round-robin algorithm
void rr(Deque* processes, Deque* ram_list, char* memory_alloc, int quantum) {

    assert(processes!=NULL);

    // Start the clock by 0
    int clock = 0;


    if (strcmp(memory_alloc, "u") == 0) {

        unlimited_allocator(processes, &clock, "rr", quantum);

    }


    if (strcmp(memory_alloc, "p") == 0) {

        swapping_allocator(processes, ram_list, &clock, "rr", quantum);

    }
}