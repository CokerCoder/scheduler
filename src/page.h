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



#endif