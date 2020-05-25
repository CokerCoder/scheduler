#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deque.h"
#include "process.h"
#include "ram.h"


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
                    break;
                }
            }
            curr = curr->next;
        } else {
            (*clock)++;
        }
    }
}


void swapping_allocator(Deque* processes, Deque* ram_list, int* clock, const char* sa, int quantum) {
    Node* curr = processes->head;
    
    while (curr!=NULL) {
        int elapsed = 0;
        Process* curr_process = (Process *) curr->data;

        if (*clock >= curr_process->arrival_time) {
            int exist_in_ram = find_process(ram_list, curr_process->pid);
            int load_time = 0;

            if (exist_in_ram == -1) {
                int available_pos = 0;
                while (1) {
                    available_pos = available_space(ram_list, curr_process->mem_size);
                    if (available_pos < 0) {
                        int least_used_process = least_used(ram_list);
                        printf("%d, EVICTED, mem-addresses=%s\n", *clock, process_addr(ram_list, least_used_process));
                        evict_space(ram_list, least_used_process);
                    } else {
                        break;
                    }
                }

                load_process(ram_list, curr_process, available_pos, *clock);
                load_time = 2 * curr_process->mem_size / 4;
            } else {
                update_time(ram_list, curr_process->pid, *clock);
            }

            printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%, mem-addresses=%s\n", \
                    *clock, curr_process->pid, curr_process->remaining_time, load_time, mem_uasge(ram_list), process_addr(ram_list, curr_process->pid));

            *clock += load_time;

            while (curr_process->remaining_time > 0) {

                (*clock)++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    curr_process->finish_time = *clock;
                    printf("%d, EVICTED, mem-addresses=%s\n", *clock, process_addr(ram_list, curr_process->pid));
                    evict_space(ram_list, curr_process->pid);
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