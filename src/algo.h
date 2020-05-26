#ifndef UNTITLED_ALGO_H
#define UNTITLED_ALGO_H

#include "deque.h"

void ff(Deque* processes, Deque* ram_list, char* memory_alloc, int quantum);
void rr(Deque* processes, Deque* ram_list, Deque* pages, char* memory_alloc, int quantum);
void cs(Deque* processes, Deque* ram_list, Deque* pages, char* memory_alloc, int quantum);

#endif 