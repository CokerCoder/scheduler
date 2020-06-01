#ifndef RAM_H
#define RAM_H

#include "deque.h"


typedef struct ram Ram;
struct ram
{
    char status; // 'H' for empty and 'P' for occupied
    long starting;
    long length;
    long last_access;
    long pid;
};


void init_ram(Deque* ram, long mem_size);
Ram* new_ram(char status, long starting, long length, long last_access, long pid);
void print_ram(Deque* ram_list);

// Check if a given process already in the RAM
long find_process(Deque* ram_list, long pid);

// Check if there is enough space in the RAM
// Return: Starting position of the available block, -1 for unavailable
long available_space(Deque* ram_list, long required);

// Load a process from disk to RAM to the given memory position
void load_process(Deque* ram_list, Process* process, long starting, long clock);

// Evict the block with the given pid process
void evict_space(Deque* ram_list, long pid);

long mem_uasge(Deque* ram_list);

void process_addr(Deque* ram_list, long pid);

long least_used(Deque* ram_list);

void update_time(Deque* ram_list, long pid, long clock);


#endif