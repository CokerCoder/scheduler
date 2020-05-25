#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "deque.h"


void unlimited_allocator(Deque* processes, int* clock, const char* sa, int quantum);
void swapping_allocator(Deque* processes, Deque* ram_list, int* clock, const char* sa, int quantum);
void virtual_allocator(Deque* processes, Deque* pages, int* clock, const char* sa, int quantum);



#endif