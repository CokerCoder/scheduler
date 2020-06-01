#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "deque.h"


void unlimited_allocator(Deque* processes, long* clock, const char* sa, long quantum);
void swapping_allocator(Deque* processes, Deque* ram_list, long* clock, const char* sa, long quantum);
void virtual_allocator(Deque* processes, Deque* pages, long* clock, const char* sa, long quantum);
void custom_allocator(Deque* processes, Deque* pages, long* clock, const char* sa, long quantum);



#endif