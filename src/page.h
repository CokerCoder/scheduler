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

// Initialize the pages linked list
void init_pages(Deque* pages, long mem_size);
void print_pages(Deque* pages);
long free_pages(Deque* pages);

// Number of pages of process pid that already loaded into the pages
long already_loaded(Deque* pages, long pid);
// Load the process with pid to the page
long load_process_topage(Deque* pages, long pid, long required, long clock);
// evict a single page of process with pid and return the evicted page position
long evict_page(Deque* pages, long pid);
// check if process with pid in the ram, if so return the position, otherwise return -1
long has_process(Deque* pages, long pid);
// print process address
void print_process_addr(Deque* pages, long pid);
// Memory used percentage
long mem_usage_page(Deque* pages);
// Helper function to print out the evicted pages
void print_evicted(long evicted_pages[], long clock, long n);
// Compare function used in the qsort
int cmpfunc (const void * a, const void * b);


#endif