#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"
#include "process.h"
#include "algo.h"
#include "stats.h"

// First come first serve algorithm
void ff(Deque* deque, char* memory_alloc, int mem_size, int quantum) {

    Stats stats = new_stats();
    
    assert(deque!=NULL);
    Node *curr = deque->head;

    // Start the clock by 0
    int clock = 0;

    while (curr!=NULL) {
        
        Process* curr_process = &curr->process;
        if (clock >= curr_process->arrival_time) {
            // When unlimited memory
            printf("%d, RUNNING, id=%d, remaining-time=%d\n", clock, curr_process->pid, curr_process->remaining_time);
            while (curr_process->remaining_time > 0) {

                clock++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    stats.curr_throughput++;
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, deque->size-1);
                }

                // Update the stats once increment the clock
                stats = check_throughput(stats, clock);
            }

            // Update the stats once finish a job
            stats = update_stats(stats, clock, *curr_process);

            curr = curr->next;
            deque_pop(deque);
            
        } else {
            clock++;
            stats = check_throughput(stats, clock);
        }
    }

    stats.finish_time = clock;
    print_stats(stats);
}



// Round-robin algorithm
void rr(Deque* deque, char* memory_alloc, int mem_size, int quantum) {

    Stats stats = new_stats();
    
    assert(deque!=NULL);
    Node *curr = deque->head;

    // Start the clock by 0
    int clock = 0;

    while (curr!=NULL) {
        
        int elapsed = 0;
        Process* curr_process = &curr->process;

        if (clock >= curr_process->arrival_time) {
            // When unlimited memory
            printf("%d, RUNNING, id=%d, remaining-time=%d\n", clock, curr_process->pid, curr_process->remaining_time);
            while (elapsed <= quantum) {

                if (elapsed == quantum) {
                    // Move current node to after the last arrival process
                    // Since we have read the whole list of processes, some of them may not arrived by the time the quantum reached
                    move_to_last(deque, clock);
                    curr = deque->head;
                    break;
                }

                clock++;
                elapsed++;
                curr_process->remaining_time--;

                if (curr_process->remaining_time == 0) {
                    stats.curr_throughput++;
                    stats = update_stats(stats, clock, *curr_process);
                    // Finish current process
                    printf("%d, FINISHED, id=%d, proc-remaining=%d\n", clock, curr_process->pid, deque->size-1);
                    curr = curr->next;
                    deque_pop(deque);
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

    stats.finish_time = clock;
    print_stats(stats);
}