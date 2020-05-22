#include <stdio.h>

#include "stats.h"
#include "process.h"



Stats new_stats() {
    Stats stats = {0, 0, 0, 0, 999, 0, 0, 0, 0};
    return stats;
}


Stats check_throughput(Stats stats, int clock) {
    if (clock % 60 == 0 && clock > 0) {
        if (stats.curr_throughput < stats.min_throughput) {
            stats.min_throughput = stats.curr_throughput;
        }
        else if (stats.curr_throughput > stats.max_throughput) {
            stats.max_throughput = stats.curr_throughput;
        }
        stats.total_throughput++;
        stats.curr_throughput = 0;
    }
    return stats;
}


Stats update_stats(Stats stats, int clock, Process curr_process) {
    stats.total_processes++;
    double curr_turnaround = clock - curr_process.arrival_time;
    stats.total_turnaround += curr_turnaround;
    double curr_overhead = curr_turnaround / curr_process.job_time;
    stats.total_overhead += curr_overhead;
    if (curr_overhead > stats.max_overhead) {
        stats.max_overhead = curr_overhead;
    }
    return stats;
}


void print_stats(Stats stats) {
    int avg_throughput = (stats.total_processes+stats.total_throughput-1)/stats.total_throughput; //Round up
    int avg_turnaround = (stats.total_turnaround+stats.total_processes-1)/stats.total_processes; //Round up
    double avg_overhead = stats.total_overhead/stats.total_processes;
    printf("Throughput %d, %d, %d\nTurnaround time %d\nTime overhead %.2f %.2f\nMakespan %d\n", \
            avg_throughput, stats.min_throughput, stats.max_throughput, avg_turnaround, stats.max_overhead, avg_overhead, stats.finish_time);
}