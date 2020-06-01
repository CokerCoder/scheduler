#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "page.h"

#define PAGE_SIZE 4


void init_pages(Deque* pages, __int64_t mem_size) {
    assert(pages!=NULL);
    __int64_t i = 0;
    for (;i<mem_size/PAGE_SIZE;i++) {
        Page* new_page = (Page *) malloc(sizeof(Page));
        assert(new_page);
        new_page->n = i;
        new_page->pid = -1;
        new_page->last_access = -1;
        deque_insert(pages, new_page);
    }
}

void print_pages(Deque* pages) {

    assert(pages!=NULL);
    Node* curr = pages->head;

    printf("\n----------------------------------------------------------------\n");

    while (curr!=NULL) {
        Page* curr_page = (Page *) curr->data;
        printf("%-5ld last access: %-5ld pid: %-5ld\n", \
        curr_page->n, curr_page->last_access, curr_page->pid);
        curr = curr->next;
    }

    printf("----------------------------------------------------------------\n\n");

}

__int64_t free_pages(Deque* pages) {

    assert(pages!=NULL);
    Node* curr = pages->head;

    __int64_t free = 0;
    while(curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == -1) {
            free++;
        }
        curr = curr->next;
    }

    return free;
}

__int64_t already_loaded(Deque* pages, __int64_t pid) {
    
    assert(pages!=NULL);
    Node* curr = pages->head;

    __int64_t loaded = 0;
    while(curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == pid) {
            loaded++;
        }
        curr = curr->next;
    }

    return loaded;
}


__int64_t load_process_topage(Deque* pages, __int64_t pid, __int64_t required, __int64_t clock) {
    
    assert(pages!=NULL);
    Node* curr = pages->head;

    __int64_t loaded = 0;
    while(curr!=NULL) {
        if (loaded == required) {
            break;
        }
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == -1) {
            curr_page->last_access = clock;
            curr_page->pid = pid;
            loaded++;
        }
        curr = curr->next;
    }

    return loaded;
}


// Evict pages one at a time and return the page number that evicted
__int64_t evict_page(Deque* pages, __int64_t pid) {

    assert(pages!=NULL);
    Node* curr = pages->head;

    while (curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == pid) {
            curr_page->last_access = -1;
            curr_page->pid = -1;
            return curr_page->n;
        }
        curr = curr->next;
    }
    return -1;
}


__int64_t has_process(Deque* pages, __int64_t pid) {
    
    assert(pages!=NULL);
    Node* curr = pages->head;

    while (curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == pid) {
            return 1;
        }
        curr = curr->next;
    }
    return -1;
}


void print_process_addr(Deque* pages, __int64_t pid) {
    
    assert(pages!=NULL);
    Node* curr = pages->head;

    __int64_t count = 0;
    
    printf(", mem-addresses=[");

    while (curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid == pid) {
            if (count!=0) {
                printf(",");
            }
            printf("%ld", curr_page->n);
            count++;
        }
        curr = curr->next;
    }
    printf("]\n");
}


__int64_t mem_usage_page(Deque* pages) {
    assert(pages!=NULL);
    Node* curr = pages->head;

    double used = 0;
    double total = 0;

    while (curr!=NULL) {
        Page* curr_page = ((Page *)curr->data);
        if (curr_page->pid != -1) {
            used++;
        }
        total++;
        curr = curr->next;
    }

    __int64_t percentage = used / total * 100;

    return percentage;
}


void print_evicted(__int64_t evicted_pages[], __int64_t clock, __int64_t n) {
    printf("%ld, EVICTED, mem-addresses=[", clock);
    for (__int64_t j=0;j<n;j++) {
        if (j!=0) {
            printf(",");
        }
        printf("%ld", evicted_pages[j]);
    }
    printf("]\n");
}

int cmpfunc (const void * a, const void * b) {
   return ( *(__int64_t*)a - *(__int64_t*)b );
}

