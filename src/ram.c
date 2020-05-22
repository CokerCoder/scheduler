#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "ram.h"
#include "deque.h"


void init_ram(Deque* ram_list, int mem_size) {
    Ram* ram_block = new_ram('H', 0, mem_size, -1, -1);
    deque_insert(ram_list, ram_block);
}


Ram* new_ram(char status, int starting, int length, int last_access, int pid) {
    Ram* ram_block = (Ram *) malloc(sizeof(Ram));
    assert(ram_block);

    ram_block->status = status;
    ram_block->starting = starting;
    ram_block->length = length;
    ram_block->last_access = last_access;
    ram_block->pid = pid;

    return ram_block;
}


void print_ram(Deque* ram_list) {

    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        printf("%c    starting: %-5d length: %-5d last access: %-5d pid: %-5d\n", \
        curr_block->status, curr_block->starting, curr_block->length, curr_block->last_access, curr_block->pid);
        curr = curr->next;
    }

}


int find_process(Deque* ram_list, int pid) {
    
    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->pid == pid) {
            return curr_block->starting;
        }
        curr=curr->next;
    }
    return -1;
}


int available_space(Deque* ram_list, int required) {
    
    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->status == 'H' && curr_block->length >= required) {
            return curr_block->starting;
        }
        curr=curr->next;
    }
    return -1;
}


void load_process(Deque* ram_list, Process* process, int starting) {
    assert(ram_list!=NULL);

    Ram* newblock = new_ram('P', starting, process->mem_size, 0, process->pid);
    Node* new_block = new_node(newblock);

    // Find the previous and next block of memory
    Node* curr = ram_list->head;
    Node* target = NULL;
    Node* prev = NULL;
    Node* next = NULL;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->starting == starting) {
            target = curr;
            prev = curr->prev;
            next = curr->next;
        }
        curr = curr->next;
    }

    new_block->prev = prev;
    if (prev!=NULL) {
        prev->next = new_block; 
    } else {
        ram_list->head = new_block;
    }

    new_block->next = next;
    if (next!=NULL) {
        next->prev = new_block;
    } else {
        ram_list->tail = new_block;
    }

    // Calculate how much free space left on that block
    int left = ((Ram *) target->data)->length - process->mem_size;
    // If there is space left over, make a new block
    if (left > 0) {
        Ram* leftblock = new_ram('H', starting+process->mem_size, left, -1, -1);
        Node* left_block = new_node(leftblock);

        new_block->next = left_block;
        left_block->prev = new_block;

        if (next!=NULL) {
            next->prev = left_block;
        } else {
            ram_list->tail = left_block;
        }

    }
    free_node(target);
}


void evict_space(Deque* ram_list, int pid) {
    assert(ram_list!=NULL);

    Node* curr = ram_list->head;
    Node* target = NULL;
    Node* prev = NULL;
    Node* next = NULL;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->pid == pid) {
            target = curr;
            prev = curr->prev;
            next = curr->next;
        }
        curr = curr->next;
    }

    
}
