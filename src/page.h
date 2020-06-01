#ifndef UNTITLED_PAGE_H
#define UNTITLED_PAGE_H

#include "deque.h"

typedef struct page Page;
struct page
{
    __int64_t n;
    __int64_t pid;
    __int64_t last_access;
};

// Initialize the pages linked list
void init_pages(Deque* pages, __int64_t mem_size);
void print_pages(Deque* pages);
__int64_t free_pages(Deque* pages);

// Number of pages of process pid that already loaded into the pages
__int64_t already_loaded(Deque* pages, __int64_t pid);
// Load the process with pid to the page
__int64_t load_process_topage(Deque* pages, __int64_t pid, __int64_t required, __int64_t clock);
// evict a single page of process with pid and return the evicted page position
__int64_t evict_page(Deque* pages, __int64_t pid);
// check if process with pid in the ram, if so return the position, otherwise return -1
__int64_t has_process(Deque* pages, __int64_t pid);
// print process address
void print_process_addr(Deque* pages, __int64_t pid);
// Memory used percentage
__int64_t mem_usage_page(Deque* pages);
// Helper function to print out the evicted pages
void print_evicted(__int64_t evicted_pages[], __int64_t clock, __int64_t n);
// Compare function used in the qsort
int cmpfunc (const void * a, const void * b);


#endif