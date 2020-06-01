#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "process.h"
#include "deque.h"



Process* new_process(__int64_t arrival_time, __int64_t pid, __int64_t mem_size, __int64_t job_time) {
    
    Process* process = (Process *) malloc(sizeof(Process));
    assert(process);

    process->arrival_time = arrival_time;
    process->pid = pid;
    process->mem_size = mem_size;
    process->job_time = job_time;
    process->remaining_time = job_time;
    process->finish_time = -1; // some arbitrary finish time
    process->last_access = -1;

    return process;
}


// Move the running process to the last available position (only if the data is type of process)
void move_to_last(Deque* processes, __int64_t pid, __int64_t clock) {

    assert(processes!=NULL);
    assert(processes->size>0);

    // If there is only one node ledt, nothing can be done
    if (processes->size==1) {
        return;
    }

    Node* first = NULL;
    Node* last = processes->tail;

    Node* curr = processes->head;


    while (curr!=NULL) {
        Process* curr_process = ((Process *)curr->data);
        if (curr_process->pid == pid) {
            first = curr;
            break;
        }
        curr = curr->next;
    }

    Node* before = first->prev;

    // After this loop, curr will be the next arriving process, so we will insert the running process before curr
    while (curr!=NULL && (((Process *)curr->data)->arrival_time <= clock)) {
        curr = curr->next;
    }

    if (curr == first->next) {
        return;
    }

    // If curr is NULL, means all processes have arrived and simply insert the running process to the end
    if (curr==NULL) {
        if (processes->head == first) {
            processes->head = first->next;
        } else {
            first->prev->next = first->next;
            first->next->prev = first->prev;
        }
        first->next = NULL;
        last->next = first;
        last->next->prev = last;
        processes->tail = last->next;
        return;
    }
    // printf("first: %ld, curr: %ld, first->next: %ld, curr->prev: %ld\n", ((Process*)first->data)->pid, ((Process*)curr->data)->pid, ((Process*)first->next->data)->pid, ((Process*)curr->prev->data)->pid);
    // Otherwise, move the running process before the curr (next arriving) process
    // Change the head pointer to point to second node now
    if (processes->head == first) {
        processes->head = first->next;
    } else {
        before->next = first->next;
    }

    
    first->next->prev = before;

    curr->prev->next = first;
    first->prev = curr->prev;

    curr->prev = first;

    first->next = curr;
 
}


__int64_t proc_remaining(Deque* processes, __int64_t clock) {
    assert(processes!=NULL);
    Node *curr = processes->head;

    __int64_t num = 0;
    while (curr!=NULL) {
        Process* curr_process = (Process *) curr->data;
        if (curr_process->arrival_time < clock && curr_process->finish_time == -1) {
            num++;
        }
        curr = curr->next;
    }
    return num;
}


// Helper function to print the deque when debugging
void print_processes(Deque *processes) {
    assert(processes!=NULL);

    Node *curr = processes->head;
    
    printf("\npid      arrival      memory      jobtime      timeleft      finished      lastaccess\n");
    while (curr != NULL) {
        Process* curr_process = (Process *) curr->data;
        printf("%-12ld%-12ld%-12ld%-12ld%-12ld%-12ld%-12ld\n", \
        curr_process->pid, curr_process->arrival_time, curr_process->mem_size, curr_process->job_time, curr_process->remaining_time, curr_process->finish_time, curr_process->last_access); 
        curr = curr->next; 
    } 
    printf("\n");
}

void print_processes_reversed(Deque *processes) {
    assert(processes!=NULL);

    Node *curr = processes->tail;
    
    printf("\npid      arrival      memory      jobtime      timeleft      finished\n");
    while (curr != NULL) {
        Process* curr_process = (Process *) curr->data;
        printf("%-12ld%-12ld%-12ld%-12ld%-12ld%-12ld\n", \
        curr_process->pid, curr_process->arrival_time, curr_process->mem_size, curr_process->job_time, curr_process->remaining_time, curr_process->finish_time); 
        curr = curr->prev; 
    } 
    printf("\n");
}

Node* next_running_process(Deque* processes) {
    assert(processes!=NULL);

    Node *curr = processes->head;
    while (curr != NULL) {
        Process* curr_process = (Process *) curr->data;
        if (curr_process->remaining_time > 0) {
            return curr;
        }
        curr = curr->next;
    }

    return curr;
}


void print_stats(Deque* processes) {
    assert(processes!=NULL);

    __int64_t total_processes = 0;
    __int64_t interval = 60;
    __int64_t curr_throughput = 0;
    __int64_t total_throughput = 0;
    __int64_t min_throughput = 99999;
    __int64_t max_throughput = 0;
    __int64_t total_turnaround = 0;
    double total_overhead = 0;
    double max_overhead = 0;

    Node *curr = processes->head;

    while (curr != NULL) {
        Process* curr_process = (Process *) curr->data;

        while (curr_process->finish_time > interval) {
            if (curr_throughput < min_throughput) {
                min_throughput = curr_throughput;
            }
            if (curr_throughput > max_throughput) {
                max_throughput = curr_throughput;
            }
            total_throughput++;
            curr_throughput = 0;
            interval += 60;
        }
        
        if (curr_process->finish_time <= interval) {
            curr_throughput++;
        }

        if (curr->next == NULL) {
            if (curr_throughput < min_throughput) {
                min_throughput = curr_throughput;
            }
            if (curr_throughput > max_throughput) {
                max_throughput = curr_throughput;
            }
            total_throughput++;
        }

        
        __int64_t curr_turnaround = curr_process->finish_time - curr_process->arrival_time;
        total_turnaround += curr_turnaround;
        
        double curr_overhead = (double) curr_turnaround / (double) curr_process->job_time;
        total_overhead += curr_overhead;
        if (curr_overhead > max_overhead) {
            max_overhead = curr_overhead;
        }

        total_processes++;
        curr = curr->next;
    }

    printf("Throughput %ld, %ld, %ld\n", (total_processes + total_throughput - 1)/ (total_throughput), min_throughput, max_throughput);
    printf("Turnaround time %ld\n", (total_turnaround + total_processes - 1) / (total_processes));
    printf("Time overhead %.2f %.2f\n", max_overhead, total_overhead/total_processes);
    printf("Makespan %ld\n", ((Process*)processes->tail->data)->finish_time);
        
}


void swap(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}


void sjf(Deque* processes) {
    assert(processes);

    Node* head = processes->head;
    Node* ptr = NULL;
    Node* lptr = NULL;

    __int64_t swapped = 0;

    if (head == NULL) {
        return;
    }

    do
    {
        swapped = 0;
        ptr = head;

        while (ptr->next != lptr) {
            if (((Process *)ptr->data)->arrival_time == ((Process *)ptr->next->data)->arrival_time) {
                if (((Process *)ptr->data)->job_time > ((Process *)ptr->next->data)->job_time) {
                    swap(&(ptr->data), &(ptr->next->data));
                    swapped = 1;
                }
            }
            ptr = ptr->next;
        }
        lptr = ptr;

    } while (swapped);

}

// Sort the process based on the pid when arrives the same time
void sort_pid(Deque* processes) {
    assert(processes);

    Node* head = processes->head;
    Node* ptr = NULL;
    Node* lptr = NULL;

    __int64_t swapped = 0;

    if (head == NULL) {
        return;
    }

    do
    {
        swapped = 0;
        ptr = head;

        while (ptr->next != lptr) {
            if (((Process *)ptr->data)->arrival_time == ((Process *)ptr->next->data)->arrival_time) {
                if (((Process *)ptr->data)->pid > ((Process *)ptr->next->data)->pid) {
                    swap(&(ptr->data), &(ptr->next->data));
                    swapped = 1;
                }
            }
            ptr = ptr->next;
        }
        lptr = ptr;

    } while (swapped);

}


__int64_t least_used_id(Deque* processes, __int64_t running_id) {

    assert(processes!=NULL);
    Node* curr = processes->head;

    __int64_t least_time = 999999999;
    __int64_t least_pid = 0;

    while (curr!=NULL) {
        Process* curr_process = ((Process *)curr->data);
        if (curr_process->last_access >= 0 && curr_process->last_access < least_time && curr_process->pid != running_id) {
            least_time = curr_process->last_access;
            least_pid = curr_process->pid;
        }
        curr = curr->next;
    }

    return least_pid;
}

__int64_t recent_used_id(Deque* processes, __int64_t running_id) {

    assert(processes!=NULL);
    Node* curr = processes->head;

    __int64_t most_time = 0;
    __int64_t most_pid = 0;

    while (curr!=NULL) {
        Process* curr_process = ((Process *)curr->data);
        if (curr_process->last_access >= 0 && curr_process->last_access > most_time && curr_process->pid != running_id) {
            most_time = curr_process->last_access;
            most_pid = curr_process->pid;
        }
        curr = curr->next;
    }

    return most_pid;
}


void update_access(Deque* processes, __int64_t evicted_id) {
    assert(processes!=NULL);
    Node* curr = processes->head;

    while (curr!=NULL) {
        Process* curr_process = ((Process *)curr->data);
        if (curr_process->pid == evicted_id) {
            curr_process->last_access = -1;
        }
        curr = curr->next;
    }
}