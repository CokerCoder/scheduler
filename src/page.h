#ifndef UNTITLED_PAGE_H
#define UNTITLED_PAGE_H

#include "deque.h"

typedef struct page Page;
struct page
{
    int n;
    int pid;
    int last_access;
};


void init_pages(Deque* pages, int mem_size);
void print_pages(Deque* pages);
int free_pages(Deque* pages);
int already_loaded(Deque* pages, int pid);
int load_process_topage(Deque* pages, int pid, int required, int clock);
int least_used_id(Deque* pages, int running_id);
int evict_page(Deque* pages, int pid);
int has_process(Deque* pages, int pid);
void print_process_addr(Deque* pages, int pid);
int mem_usage_page(Deque* pages);
void print_evicted(int evicted_pages[], int clock, int n);
int cmpfunc (const void * a, const void * b);


#endif