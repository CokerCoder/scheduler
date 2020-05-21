#ifndef STATS_H
#define STATS_H

#include "process.h"

typedef struct stats Stats;
struct stats {
    int finish_time;
    int total_throughput; // number of 60-sec rounds
    int curr_throughput;
    int max_throughput;
    int min_throughput;
    double total_processes;
    double total_turnaround;
    double total_overhead;
    double max_overhead;
};


Stats new_stats();
Stats check_throughput(Stats stats, int clock);
Stats update_stats(Stats stats, int clock, Process curr_process);
void print_stats(Stats stats);


#endif