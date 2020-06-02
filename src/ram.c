#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "process.h"
#include "ram.h"
#include "deque.h"


void init_ram(Deque* ram_list, long long int mem_size) {
    Ram* ram_block = new_ram('H', 0, mem_size, -1, -1);
    deque_insert(ram_list, ram_block);
}


Ram* new_ram(char status, long long int starting, long long int length, long long int last_access, long long int pid) {
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

    printf("----------------------------------------------------------------\n");

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        printf("%c    starting: %-5lld length: %-5lld last access: %-5lld pid: %-5lld\n", \
        curr_block->status, curr_block->starting, curr_block->length, curr_block->last_access, curr_block->pid);
        curr = curr->next;
    }

    printf("----------------------------------------------------------------\n\n");

}


long long int find_process(Deque* ram_list, long long int pid) {
    
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


long long int available_space(Deque* ram_list, long long int required) {
    
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


void load_process(Deque* ram_list, Process* process, long long int starting, long long int clock) {
    assert(ram_list!=NULL);

    Ram* newblock = new_ram('P', starting, process->mem_size, clock, process->pid);
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
    long long int left = ((Ram *) target->data)->length - process->mem_size;
    // If there is space left over, make a new block
    if (left > 0) {
        Ram* leftblock = new_ram('H', starting+process->mem_size, left, -1, -1);
        Node* left_block = new_node(leftblock);

        new_block->next = left_block;
        left_block->prev = new_block;
        left_block->next = next;

        if (next!=NULL) {
            next->prev = left_block;
        } else {
            ram_list->tail = left_block;
        }

    }
    free_node(target);
}


void evict_space(Deque* ram_list, long long int pid) {
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


    // If the block is between two occupied blocks, 
    // or the block is the only block, simply turn this into a free block
    if (((prev!=NULL && ((Ram *) prev->data)->status=='P') && (next!=NULL && ((Ram *) next->data)->status=='P'))\
        || (prev==NULL && next==NULL)
        || (prev==NULL && ((Ram *) next->data)->status=='P')
        || (next==NULL && ((Ram *) prev->data)->status=='P')
        ) {
        ((Ram *) target->data)->status = 'H';
        ((Ram *) target->data)->pid = -1;
        ((Ram *) target->data)->last_access = -1;
        return; // Return the process to avoid freeing the target node
    }

    // If both of the adj blocks are free as well, combine these blocks
    if ((prev!=NULL && ((Ram *) prev->data)->status=='H') && (next!=NULL && ((Ram *) next->data)->status=='H')) {
        ((Ram *) target->data)->status = 'H';
        ((Ram *) target->data)->pid = -1;
        ((Ram *) target->data)->last_access = -1;
        ((Ram *) target->data)->length = ((Ram *) prev->data)->length + ((Ram *) target->data)->length + ((Ram *) next->data)->length;
        ((Ram *) target->data)->starting = ((Ram *) prev->data)->starting;

        target->prev = prev->prev;
        if (prev->prev!=NULL) {
            prev->prev->next = target;
        } else {
            ram_list->head = target;
        }

        target->next = next->next;
        if (next->next!=NULL) {
            next->next->prev = target;
        } else {
            ram_list->tail = target;
        }

        free_node(prev);
        free_node(next);
        return;
    }

    // If the block after the target is free, combine the two
    if (next!=NULL && ((Ram *) next->data)->status=='H') {
        // Copy the information
        ((Ram *) next->data)->starting = ((Ram *) target->data)->starting;
        ((Ram *) next->data)->length = ((Ram *) target->data)->length + ((Ram *) next->data)->length;
        ((Ram *) next->data)->pid = -1;
        ((Ram *) next->data)->last_access = -1;

        next->prev = prev;
        if (prev!=NULL) {
            prev->next = next;
        } else {
            ram_list->head = next;
        }

        free_node(target);
        return;
    }

    // If the block before the target is free, combine the two
    if (prev!=NULL && ((Ram *) prev->data)->status=='H') {
        // Copy the information
        ((Ram *) prev->data)->length = ((Ram *) target->data)->length + ((Ram *) prev->data)->length;
        ((Ram *) prev->data)->pid = -1;
        ((Ram *) prev->data)->last_access = -1;

        prev->next = next;
        if (next!=NULL) {
            next->prev = prev;
        } else {
            ram_list->tail = prev;
        }

        free_node(target);
        return;
    }
    
}
 

long long int mem_uasge(Deque* ram_list) {
    
    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    double total = 0;
    double used = 0;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        long long int curr_size = curr_block->length;
        total += curr_size;
        if (curr_block->status == 'P') {
            used += curr_size;
        }
        curr = curr->next;
    }

    double percentage = used / total * 100;

    return ceil(percentage);
}


void process_addr(Deque* ram_list, long long int pid) {
    
    assert(ram_list!=NULL);
    Node* curr = ram_list->head;
    long long int count = 0;
    
    printf(", mem-addresses=[");

    while (curr!=NULL) {
        Ram* curr_block = ((Ram *)curr->data);
        if (curr_block->pid == pid) {
            while (count < curr_block->length/4) {
                if (count!=0) {
                    printf(",");
                }
                printf("%lld", curr_block->starting/4+count);
                count++;
            }
        }
        curr = curr->next;
    }
    printf("]\n");
    
}


long long int least_used(Deque* ram_list) {

    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    long long int least_time = __INT64_MAX__;
    long long int least_pid = 0;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->last_access >= 0 && curr_block->last_access < least_time) {
            least_time = curr_block->last_access;
            least_pid = curr_block->pid;
        }
        curr = curr->next;
    }

    return least_pid;
}

void update_time(Deque* ram_list, long long int pid, long long int clock) {

    assert(ram_list!=NULL);
    Node* curr = ram_list->head;

    while (curr!=NULL) {
        Ram* curr_block = (Ram *) curr->data;
        if (curr_block->pid == pid) {
            curr_block->last_access = clock;
        }
        curr = curr->next;
    }
    
}