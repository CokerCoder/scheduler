#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "deque.h"


void unlimited_allocator(Deque* processes, __int64_t* clock, const char* sa, __int64_t quantum);
void swapping_allocator(Deque* processes, Deque* ram_list, __int64_t* clock, const char* sa, __int64_t quantum);
void virtual_allocator(Deque* processes, Deque* pages, __int64_t* clock, const char* sa, __int64_t quantum);
void custom_allocator(Deque* processes, Deque* pages, __int64_t* clock, const char* sa, __int64_t quantum);



#endif