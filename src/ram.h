#ifndef RAM_H
#define RAM_H

#include "deque.h"


typedef struct ram Ram;
struct ram
{
    char status; // 'H' for empty and 'P' for occupied
    __int64_t starting;
    __int64_t length;
    __int64_t last_access;
    __int64_t pid;
};


void init_ram(Deque* ram, __int64_t mem_size);
Ram* new_ram(char status, __int64_t starting, __int64_t length, __int64_t last_access, __int64_t pid);
void print_ram(Deque* ram_list);

// Check if a given process already in the RAM
__int64_t find_process(Deque* ram_list, __int64_t pid);

// Check if there is enough space in the RAM
// Return: Starting position of the available block, -1 for unavailable
__int64_t available_space(Deque* ram_list, __int64_t required);

// Load a process from disk to RAM to the given memory position
void load_process(Deque* ram_list, Process* process, __int64_t starting, __int64_t clock);

// Evict the block with the given pid process
void evict_space(Deque* ram_list, __int64_t pid);

// Memory usage percentage
__int64_t mem_uasge(Deque* ram_list);

// Print the process address
void process_addr(Deque* ram_list, __int64_t pid);

__int64_t least_used(Deque* ram_list);

void update_time(Deque* ram_list, __int64_t pid, __int64_t clock);


#endif