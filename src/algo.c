#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "deque.h"
#include "process.h"
#include "ram.h"
#include "algo.h"
#include "stats.h"


// First come first serve algorithm
void ff(Deque* processes, Deque* ram_list, char* memory_alloc) {

    assert(processes!=NULL);

    Stats stats = new_stats();
    
    Node *curr = processes->head;

    // Start the clock by 0
    int clock = 0;

    // When unlimited memory
    if (strncmp(memory_alloc, "u", 1) == 0) {
        
        while (curr!=NULL) {
            Process* curr_process = (Process *) curr->data;
            if (clock >= curr_process->arrival_time) {
                printf("%d, RUNNING, id=%d, remaining-time=%d\n", clock, curr_process->pid, curr_process->remaining_time);
                while (curr_process->remaining_time > 0) {

                    clock++;
                    curr_process->remaining_time--;

                    if (curr_process->remaining_time == 0) {
                        stats.curr_throughput++;
                        printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, proc_remaining(processes, clock));
                    }

                    // Update the stats once increment the clock
                    stats = check_throughput(stats, clock);
                }

                // Update the stats once finish a job
                stats = update_stats(stats, clock, *curr_process);

                curr = curr->next;
                deque_pop(processes);
                
            } else {
                clock++;
                stats = check_throughput(stats, clock);
            }
        }
    }
    
    // When using swapping-X for memory managment
    if (strncmp(memory_alloc, "p", 1) == 0) {

        while (curr!=NULL) {
            
            Process* curr_process = (Process *) curr->data;
            if (clock >= curr_process->arrival_time) {
                
                // In ff, next turning process must not in the RAM since it will not be swapped out until it has finished
                int load_time = 0;
                load_process(ram_list, curr_process, 0, clock);
                load_time = 2 * curr_process->mem_size / 4;

                printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%, mem-addresses=%s\n", \
                    clock, curr_process->pid, curr_process->remaining_time, load_time, mem_uasge(ram_list), process_addr(ram_list, curr_process->pid));
                
                clock += load_time;
                
                while (curr_process->remaining_time > 0) {
                    clock++;
                    curr_process->remaining_time--;

                    if (curr_process->remaining_time == 0) {
                        stats.curr_throughput++;
                        // In ff, only oneeviction is needed and which is the current process
                        printf("%d, EVICTED, mem-addresses=%s\n", clock, process_addr(ram_list, curr_process->pid));
                        evict_space(ram_list, curr_process->pid);
                        printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, proc_remaining(processes, clock));
                    }
                    stats = check_throughput(stats, clock);
                }

                stats = update_stats(stats, clock, *curr_process);

                curr = curr->next;
                deque_pop(processes);
            
            } else {
                clock++;
                stats = check_throughput(stats, clock);
            }
        }
    }

    stats.finish_time = clock;
    print_stats(stats);
}



// Round-robin algorithm
void rr(Deque* processes, Deque* ram_list, char* memory_alloc, int quantum) {

    Stats stats = new_stats();
    
    assert(processes!=NULL);
    Node *curr = processes->head;

    // Start the clock by 0
    int clock = 0;

    // When unlimited memory
    if (strncmp(memory_alloc, "u", 1) == 0) {

        while (curr!=NULL) {
            
            int elapsed = 0;
            Process* curr_process = (Process *) curr->data;

            if (clock >= curr_process->arrival_time) {
                // When unlimited memory
                printf("%d, RUNNING, id=%d, remaining-time=%d\n", clock, curr_process->pid, curr_process->remaining_time);
                while (elapsed <= quantum) {

                    if (elapsed == quantum) {
                        // Move current node to after the last process that has already arrived
                        // Since we have read the whole list of processes, some of them may not arrived by the time the quantum reached
                        move_to_last(processes, clock);
                        curr = processes->head;
                        break;
                    }

                    clock++;
                    elapsed++;
                    curr_process->remaining_time--;

                    if (curr_process->remaining_time == 0) {
                        stats.curr_throughput++;
                        stats = update_stats(stats, clock, *curr_process);
                        // Finish current process
                        printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, proc_remaining(processes, clock));
                        curr = curr->next;
                        deque_pop(processes);
                        elapsed = quantum+1; // Stop the loop without break
                    }
                    stats = check_throughput(stats, clock);    
                }
            }
            else {
                clock++;
                stats = check_throughput(stats, clock);
            }
        }
    }


    if (strncmp(memory_alloc, "p", 1) == 0) {

        while (curr!=NULL) {
            
            int elapsed = 0;
            Process* curr_process = (Process *) curr->data;

            if (clock >= curr_process->arrival_time) {
                // Check if the process is in the memory already
                // If not, load the process first
                int exist = find_process(ram_list, curr_process->pid);
                int load_time = 0;
                
                if (exist == -1) {
                    // Check if there is enough continous space for loading the process
                    int available_pos = 0;
                    while (1) {
                        available_pos = available_space(ram_list, curr_process->mem_size);
                        if (available_pos < 0) {
                            int least_used_process = least_used(ram_list);
                            printf("%d, EVICTED, mem-addresses=%s\n", clock, process_addr(ram_list, least_used_process));
                            
                            evict_space(ram_list, least_used_process);
                        } else {
                            break;
                        }
                    }
                    load_process(ram_list, curr_process, available_pos, clock);
                    // exist = available_pos;
                    load_time = 2 * curr_process->mem_size / 4;
                } else {
                    // Update the process's last access time
                    update_time(ram_list, curr_process->pid, clock);
                }

                // When unlimited memory
                printf("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%, mem-addresses=%s\n", \
                    clock, curr_process->pid, curr_process->remaining_time, load_time, mem_uasge(ram_list), process_addr(ram_list, curr_process->pid));

                clock += load_time;

                while (elapsed <= quantum) {

                    if (elapsed == quantum) {
                        move_to_last(processes, clock);
                        curr = processes->head;
                        break;
                    }

                    clock++;
                    elapsed++;
                    curr_process->remaining_time--;

                    if (curr_process->remaining_time == 0) {
                        stats.curr_throughput++;
                        stats = update_stats(stats, clock, *curr_process);
                        // Finish current process
                        printf("%d, EVICTED, mem-addresses=%s\n", clock, process_addr(ram_list, curr_process->pid));
                        
                        evict_space(ram_list, curr_process->pid);
                        printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, proc_remaining(processes, clock));
                        curr = curr->next;

                        deque_pop(processes);
                        elapsed = quantum+1; // Stop the loop without break
                    }
                    stats = check_throughput(stats, clock);    
                }
            }
            else {
                clock++;
                stats = check_throughput(stats, clock);
            }
        }
    }

    stats.finish_time = clock;
    print_stats(stats);
}