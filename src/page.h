#ifndef UNTITLED_PAGE_H
#define UNTITLED_PAGE_H

#include "deque.h"

typedef struct page Page;
struct page
{
    long n;
    long pid;
    long last_access;
};


void init_pages(Deque* pages, long mem_size);
void print_pages(Deque* pages);
long free_pages(Deque* pages);
long already_loaded(Deque* pages, long pid);
long load_process_topage(Deque* pages, long pid, long required, long clock);
long evict_page(Deque* pages, long pid);
long has_process(Deque* pages, long pid);
void print_process_addr(Deque* pages, long pid);
long mem_usage_page(Deque* pages);
void print_evicted(long evicted_pages[], long clock, long n);
int cmpfunc (const void * a, const void * b);


#endif