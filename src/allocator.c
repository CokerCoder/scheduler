#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"
#include "ram.h"
#include "page.h"


void unlimited_allocator(Deque* processes, int* clock, const char* sa, int quantum) {
    
    Node* curr = processes->head;
    
    while (curr!=NULL) {
        int elapsed = 0;
        Process* curr_process = (Process *) curr->data;
        if (*clock >= curr_process->arrival_time) {
            printf("%d, RUNNING, id=%d, remaining-time=%d\n", *clock, curr_process->pid, curr_process->remaining_time);
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
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", *clock, curr_process->pid, proc_remaining(processes, *clock));
                    curr = curr->next;     
                    break;
                }
            }
            
        } else {
            (*clock)++;
        }
    }
}


void swapping_allocator(Deque* processes, Deque* ram_list, int* clock, const char* sa, int quantum) {
    Node* curr = processes->head;
    
    while (curr!=NULL) {
        // print_ram(ram_list);
        int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            int exist_in_ram = find_process(ram_list, curr_process->pid);
            int load_time = 0;

            if (exist_in_ram == -1) {
                int available_pos = 0;

                int evicted_pages[1000];
                int i = 0;

                while (1) {
                    available_pos = available_space(ram_list, curr_process->mem_size);
                    if (available_pos < 0) {
                        int least_used_process = least_used_id(processes, curr_process->pid);
                        // printf("least id: %d\n", least_used_process);
                        
                        Node* curr = ram_list->head;
                        int count = 0;

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
                // printf("load: %d\n", curr_process->pid);
                load_process(ram_list, curr_process, available_pos, *clock);
                // print_ram(ram_list);
                load_time = 2 * curr_process->mem_size / 4;
            } else {
                update_time(ram_list, curr_process->pid, *clock);
            }

            curr_process->last_access = *clock;
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%", \
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
                    printf("%d, EVICTED", *clock);
                    process_addr(ram_list, curr_process->pid);
                    evict_space(ram_list, curr_process->pid);
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", *clock, curr_process->pid, proc_remaining(processes, *clock));
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


void virtual_allocator(Deque* processes, Deque* pages, int* clock, const char* sa, int quantum) {
    Node* curr = processes->head;
    // print_pages(pages);

    while (curr!=NULL) {
        int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            int required = curr_process->mem_size/4 - already_loaded(pages, curr_process->pid); // how many more pages needed
            int load_time = 0;

            int evicted_pages[pages->size];
            int i = 0;

            int more = 0;
            if (required < 4 && (curr_process->mem_size/4 < 4)) {
                more = curr_process->mem_size/4;
            } else {
                more = 4;
            }

            while (already_loaded(pages, curr_process->pid)+free_pages(pages)<more) {

                int least_used_process = least_used_id(processes, curr_process->pid);
                // print_pages(pages);
                // print_processes(processes);
                // printf("evict %d\n", least_used_process);
                int evicted = evict_page(pages, least_used_process);
                evicted_pages[i++] = evicted;
                // Check if there are still this process in the memory
                if (has_process(pages, least_used_process) == -1) {
                    update_access(processes, least_used_process);
                }
                
            }
            if (i > 0) {
                qsort(evicted_pages, i, sizeof(int), cmpfunc);
                print_evicted(evicted_pages, *clock, i);
            }

            int loaded = load_process_topage(pages, curr_process->pid, required, *clock);
            load_time = 2 * loaded;
            // page fault penalty
            curr_process->remaining_time += curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            curr_process->last_access = *clock;
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_usage_page(pages));
            print_process_addr(pages, curr_process->pid);
            
            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    
                    printf("%d, EVICTED", *clock);
                    print_process_addr(pages, curr_process->pid);
                    while (has_process(pages, curr_process->pid)==1) {
                        evict_page(pages, curr_process->pid);
                    }
                    curr_process->last_access = -1;
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", *clock, curr_process->pid, proc_remaining(processes, *clock));

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


void custom_allocator(Deque* processes, Deque* pages, int* clock, const char* sa, int quantum) {
    Node* curr = processes->head;
    // print_pages(pages);

    while (curr!=NULL) {
        int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            int required = curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            int load_time = 0;

            int evicted_pages[pages->size];
            int i = 0;

            int more = 0;
            if (required < 4 && (curr_process->mem_size/4 < 4)) {
                more = curr_process->mem_size/4;
            } else {
                more = 4;
            }

            while (already_loaded(pages, curr_process->pid)+free_pages(pages)<more) {

                int recent_used_process = recent_used_id(processes, curr_process->pid);
                int evicted = evict_page(pages, recent_used_process);
                evicted_pages[i++] = evicted;
                if (has_process(pages, recent_used_process) == -1) {
                    update_access(processes, recent_used_process);
                }
            }
            if (i > 0) {
                qsort(evicted_pages, i, sizeof(int), cmpfunc);
                print_evicted(evicted_pages, *clock, i);
            }

            int loaded = load_process_topage(pages, curr_process->pid, required, *clock);
            load_time = 2 * loaded;
            // page fault penalty
            curr_process->remaining_time += curr_process->mem_size/4 - already_loaded(pages, curr_process->pid);
            curr_process->last_access = *clock;
            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_usage_page(pages));
            print_process_addr(pages, curr_process->pid);
            
            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    
                    printf("%d, EVICTED", *clock);
                    print_process_addr(pages, curr_process->pid);
                    while (has_process(pages, curr_process->pid)==1) {
                        evict_page(pages, curr_process->pid);
                    }
                    curr_process->last_access = -1;
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", *clock, curr_process->pid, proc_remaining(processes, *clock));

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