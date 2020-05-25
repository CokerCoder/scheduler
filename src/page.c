#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "page.h"

void init_pages(Deque* pages, int mem_size) {
    assert(pages!=NULL);
    int i = 0;
    for (;i<mem_size/4;i++) {
        Page* new_page = (Page *) malloc(sizeof(Page));
        assert(new_page);
        new_page->n = i+1;
        new_page->pid = -1;
        new_page->last_access = -1;
        deque_insert(pages, new_page);
    }
}

void print_pages(Deque* pages) {

    assert(pages!=NULL);
    Node* curr = pages->head;

    printf("----------------------------------------------------------------\n");

    while (curr!=NULL) {
        Page* curr_page = (Page *) curr->data;
        printf("%d    last access: %-5d pid: %-5d\n", \
        curr_page->n, curr_page->last_access, curr_page->pid);
        curr = curr->next;
    }

    printf("----------------------------------------------------------------\n\n");

}