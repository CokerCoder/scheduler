#ifndef RAM_H
#define RAM_H

#include "deque.h"


typedef struct ram Ram;
struct ram
{
    char status; // 'H' for empty and 'P' for occupied
    long long int starting;
    long long int length;
    long long int last_access;
    long long int pid;
};


void init_ram(Deque* ram, long long int mem_size);
Ram* new_ram(char status, long long int starting, long long int length, long long int last_access, long long int pid);
void print_ram(Deque* ram_list);

// Check if a given process already in the RAM
long long int find_process(Deque* ram_list, long long int pid);

// Check if there is enough space in the RAM
// Return: Starting position of the available block, -1 for unavailable
long long int available_space(Deque* ram_list, long long int required);

// Load a process from disk to RAM to the given memory position
void load_process(Deque* ram_list, Process* process, long long int starting, long long int clock);

// Evict the block with the given pid process
void evict_space(Deque* ram_list, long long int pid);

// Memory usage percentage
long long int mem_uasge(Deque* ram_list);

// Print the process address
void process_addr(Deque* ram_list, long long int pid);

long long int least_used(Deque* ram_list);

void update_time(Deque* ram_list, long long int pid, long long int clock);


#endif