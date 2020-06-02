#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "deque.h"


void unlimited_allocator(Deque* processes, long long int* clock, const char* sa, long long int quantum);
void swapping_allocator(Deque* processes, Deque* ram_list, long long int* clock, const char* sa, long long int quantum);
void virtual_allocator(Deque* processes, Deque* pages, long long int* clock, const char* sa, long long int quantum);
void custom_allocator(Deque* processes, Deque* pages, long long int* clock, const char* sa, long long int quantum);



#endif