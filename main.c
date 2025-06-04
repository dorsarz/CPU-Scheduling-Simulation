#include <stdio.h>
#include "scheduler.h"

void reset_processes(Process original[], Process target[], int n) {
    for (int i = 0; i < n; i++) {
        target[i] = original[i];
        target[i].remainingTime = target[i].burstTime;
        target[i].startTime = -1;
        target[i].completionTime = 0;
    }
}

int main() {
    Process input[] = {
        {1, 0, 5, 5, -1, 0},
        {2, 1, 3, 3, -1, 0},
        {3, 2, 8, 8, -1, 0},
        {4, 3, 6, 6, -1, 0}
    };
    int n = sizeof(input) / sizeof(input[0]);

    Process proc1[n], proc2[n], proc3[n];
    reset_processes(input, proc1, n);
    reset_processes(input, proc2, n);
    reset_processes(input, proc3, n);

    Metrics fcfs = fcfs_metrics(proc1, n);
    Metrics sjf = sjf_metrics(proc2, n);
    Metrics rr = rr_metrics(proc3, n, 3); // time quantum = 3

    printf("FCFS:\n  Avg TAT: %.2f\n  Avg WT: %.2f\n  Avg RT: %.2f\n",
           fcfs.avgTurnaround, fcfs.avgWaiting, fcfs.avgResponse);

    printf("SJF:\n  Avg TAT: %.2f\n  Avg WT: %.2f\n  Avg RT: %.2f\n",
           sjf.avgTurnaround, sjf.avgWaiting, sjf.avgResponse);

    printf("RR (Q=3):\n  Avg TAT: %.2f\n  Avg WT: %.2f\n  Avg RT: %.2f\n",
           rr.avgTurnaround, rr.avgWaiting, rr.avgResponse);

    return 0;
}
