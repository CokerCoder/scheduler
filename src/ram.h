#ifndef RAM_H
#define RAM_H

#include "deque.h"


typedef struct ram Ram;
struct ram
{
    char status; // 'H' for empty and 'P' for occupied
    int starting;
    int length;
    int last_access;
    int pid;
};


void init_ram(Deque* ram, int mem_size);
Ram* new_ram(char status, int starting, int length, int last_access, int pid);
void print_ram(Deque* ram_list);

// Check if a given process already in the RAM
int find_process(Deque* ram_list, int pid);

// Check if there is enough space in the RAM
// Return: Starting position of the available block, -1 for unavailable
int available_space(Deque* ram_list, int required);

// Load a process from disk to RAM to the given memory position
void load_process(Deque* ram_list, Process* process, int starting, int clock);

// Evict the block with the given pid process
void evict_space(Deque* ram_list, int pid);

int mem_uasge(Deque* ram_list);

char* process_addr(Deque* ram_list, int pid);

int least_used(Deque* ram_list);

void update_time(Deque* ram_list, int pid, int clock);


#endif