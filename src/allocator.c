#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"
#include "ram.h"
#include "page.h"


void unlimited_allocator(Deque* processes, long long int* clock, const char* sa, long long int quantum) {
    
    Node* curr = processes->head;
    
    while (curr!=NULL) {
        long long int elapsed = 0;
        Process* curr_process = (Process *) curr->data;
        
        if (*clock >= curr_process->arrival_time) {
            printf("%lld, RUNNING, id=%lld, remaining-time=%lld\n", *clock, curr_process->pid, curr_process->remaining_time);
            while (curr_process->remaining_time > 0) {

                if (strncmp(sa, "rr", 2) == 0) {
                    if (elapsed == quantum) {
                        move_to_last(processes, curr_process->pid, *clock);
                        curr = next_running_process(processes);
                        break;
                    }
                }

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    printf("%lld, FINISHED, id=%lld, proc-remaining=%lld\n", *clock, curr_process->pid, proc_remaining(processes, *clock));
                    curr = curr->next;     
                    break;
                }
            }
            
        } else {
            (*clock)++;
        }
    }
}


void swapping_allocator(Deque* processes, Deque* ram_list, long long int* clock, const char* sa, long long int quantum) {
    Node* curr = processes->head;
    long long int ram_size = ((Ram*)ram_list->head->data)->length;
    while (curr!=NULL) {
        // print_ram(ram_list);
        long long int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            long long int exist_in_ram = find_process(ram_list, curr_process->pid);
            long long int load_time = 0;

            if (exist_in_ram == -1) {
                long long int available_pos = 0;

                long long int evicted_pages[ram_size];
                long long int i = 0;

                while (1) {
                    available_pos = available_space(ram_list, curr_process->mem_size);
                    if (available_pos < 0) {
                        long long int least_used_process = least_used_id(processes, curr_process->pid);
                        // printf("least id: %lld\n", least_used_process);
                        
                        Node* curr = ram_list->head;
                        long long int count = 0;

                        while (curr!=NULL) {
                            Ram* curr_block = ((Ram *)curr->data);
                            if (curr_block->pid == least_used_process) {
                                while (count < curr_block->length/4) {
                                    evicted_pages[i++] = curr_block->starting/4+count;
                                    count++;
                                }
                            }
                            curr = curr->next;
                        }

                        evict_space(ram_list, least_used_process);
                        update_access(processes, least_used_process);
                    } else {
                        break;
                    }
                }

                if (i>0) print_evicted(evicted_pages, *clock, i);
                
                // print_ram(ram_list);
                // printf("load: %lld\n", curr_process->pid);
                load_process(ram_list, curr_process, available_pos, *clock);
                // print_ram(ram_list);
                load_time = 2 * curr_process->mem_size / 4;
            } else {
                update_time(ram_list, curr_process->pid, *clock);
            }

            curr_process->last_access = *clock;
            printf("%lld, RUNNING, id=%lld, remaining-time=%lld, load-time=%lld, mem-usage=%lld%%", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_uasge(ram_list));
            process_addr(ram_list, curr_process->pid);
            
            // print_ram(ram_list);

            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    printf("%lld, EVICTED", *clock);
                    process_addr(ram_list, curr_process->pid);
                    evict_space(ram_list, curr_process->pid);
                    printf("%lld, FINISHED, id=%lld, proc-remaining=%lld\n", *clock, curr_process->pid, proc_remaining(processes, *clock));
                    update_access(processes, curr_process->pid);
                    // print_ram(ram_list);
                    curr = curr->next;
                    break;
                }

                if (strcmp(sa, "rr") == 0) {
                    if (elapsed == quantum) {
                        move_to_last(processes, curr_process->pid, *clock);
                        curr = next_running_process(processes);
                        break;
                    }
                }
            }
        } else {
            (*clock)++;
        }
    }
    // print_ram(ram_list);
}


void virtual_allocator(Deque* processes, Deque* pages, long long int* clock, const char* sa, long long int quantum) {
    Node* curr = processes->head;
    // print_pages(pages);

    while (curr!=NULL) {
        long long int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            long long int required = curr_process->mem_size/4 - already_loaded(pages, curr_process->pid); // how many more pages needed
            long long int load_time = 0;

            long long int evicted_pages[pages->size];
            long long int i = 0;

            long long int more = 0;
            if (required < 4 && (curr_process->mem_size/4 < 4)) {
                more = curr_process->mem_size/4;
            } else {
                more = 4;
            }

            while (already_loaded(pages, curr_process->pid)+free_pages(pages)<more) {

                long long int least_used_process = least_used_id(processes, curr_process->pid);
                // print_pages(pages);
                // print_processes(processes);
                // printf("evict %lld\n", least_used_process);
                long long int evicted = evict_page(pages, least_used_process);
                evicted_pages[i++] = evicted;
                // Check if there are still this process in the memory
                if (has_process(pages, least_used_process) == -1) {
                    update_access(processes, least_used_process);
                }
                
            }
            if (i > 0) {
                qsort(evicted_pages, i, sizeof(long long int), cmpfunc);
                print_evicted(evicted_pages, *clock, i);
            }

            long long int loaded = load_process_topage(pages, curr_process->pid, required, *clock);
            load_time = 2 * loaded;
            // page fault penalty
            curr_process->remaining_time += curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            curr_process->last_access = *clock;
            printf("%lld, RUNNING, id=%lld, remaining-time=%lld, load-time=%lld, mem-usage=%lld%%", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_usage_page(pages));
            print_process_addr(pages, curr_process->pid);
            
            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    
                    printf("%lld, EVICTED", *clock);
                    print_process_addr(pages, curr_process->pid);
                    while (has_process(pages, curr_process->pid)==1) {
                        evict_page(pages, curr_process->pid);
                    }
                    curr_process->last_access = -1;
                    printf("%lld, FINISHED, id=%lld, proc-remaining=%lld\n", *clock, curr_process->pid, proc_remaining(processes, *clock));

                    curr = curr->next;
                    break;
                }

                if (strcmp(sa, "rr") == 0) {
                    if (elapsed == quantum) {
                        move_to_last(processes, curr_process->pid, *clock);
                        curr = next_running_process(processes);
                        break;
                    }
                }
            }
        } else {
            (*clock)++;
        } 
    }
} 


void custom_allocator(Deque* processes, Deque* pages, long long int* clock, const char* sa, long long int quantum) {
    Node* curr = processes->head;
    // print_pages(pages);

    while (curr!=NULL) {
        long long int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            long long int required = curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            long long int load_time = 0;

            long long int evicted_pages[pages->size];
            long long int i = 0;

            long long int more = 0;
            if (required < 4 && (curr_process->mem_size/4 < 4)) {
                more = curr_process->mem_size/4;
            } else {
                more = 4;
            }

            while (already_loaded(pages, curr_process->pid)+free_pages(pages)<more) {

                long long int recent_used_process = recent_used_id(processes, curr_process->pid);
                long long int evicted = evict_page(pages, recent_used_process);
                evicted_pages[i++] = evicted;
                if (has_process(pages, recent_used_process) == -1) {
                    update_access(processes, recent_used_process);
                }
            }
            if (i > 0) {
                qsort(evicted_pages, i, sizeof(long long int), cmpfunc);
                print_evicted(evicted_pages, *clock, i);
            }

            long long int loaded = load_process_topage(pages, curr_process->pid, required, *clock);
            load_time = 2 * loaded;
            // page fault penalty
            curr_process->remaining_time += curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            curr_process->last_access = *clock;
            printf("%lld, RUNNING, id=%lld, remaining-time=%lld, load-time=%lld, mem-usage=%lld%%", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_usage_page(pages));
            print_process_addr(pages, curr_process->pid);
            
            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    
                    printf("%lld, EVICTED", *clock);
                    print_process_addr(pages, curr_process->pid);
                    while (has_process(pages, curr_process->pid)==1) {
                        evict_page(pages, curr_process->pid);
                    }
                    curr_process->last_access = -1;
                    printf("%lld, FINISHED, id=%lld, proc-remaining=%lld\n", *clock, curr_process->pid, proc_remaining(processes, *clock));

                    curr = curr->next;
                    break;
                }

                if (strcmp(sa, "rr") == 0) {
                    if (elapsed == quantum) {
                        move_to_last(processes, curr_process->pid, *clock);
                        curr = next_running_process(processes);
                        break;
                    }
                }
            }
        } else {
            (*clock)++;
        } 
    }
} 