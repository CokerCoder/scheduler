#ifndef UNTITLED_PAGE_H
#define UNTITLED_PAGE_H

#include "deque.h"

typedef struct page Page;
struct page
{
    long long int n;
    long long int pid;
    long long int last_access;
};

// Initialize the pages linked list
void init_pages(Deque* pages, long long int mem_size);
void print_pages(Deque* pages);
long long int free_pages(Deque* pages);

// Number of pages of process pid that already loaded into the pages
long long int already_loaded(Deque* pages, long long int pid);
// Load the process with pid to the page
long long int load_process_topage(Deque* pages, long long int pid, long long int required, long long int clock);
// evict a single page of process with pid and return the evicted page position
long long int evict_page(Deque* pages, long long int pid);
// check if process with pid in the ram, if so return the position, otherwise return -1
long long int has_process(Deque* pages, long long int pid);
// print process address
void print_process_addr(Deque* pages, long long int pid);
// Memory used percentage
long long int mem_usage_page(Deque* pages);
// Helper function to print out the evicted pages
void print_evicted(long long int evicted_pages[], long long int clock, long long int n);
// Compare function used in the qsort
int cmpfunc (const void * a, const void * b);


#endif