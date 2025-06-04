#include "scheduler.h"
#include <stdbool.h>

Metrics fcfs_metrics(Process proc[], int n) {
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        if (currentTime < proc[i].arrivalTime)
            currentTime = proc[i].arrivalTime;

        proc[i].startTime = currentTime;
        proc[i].completionTime = currentTime + proc[i].burstTime;

        int turnaround = proc[i].completionTime - proc[i].arrivalTime;
        int waiting = turnaround - proc[i].burstTime;
        int response = proc[i].startTime - proc[i].arrivalTime;

        totalTAT += turnaround;
        totalWT += waiting;
        totalRT += response;

        currentTime = proc[i].completionTime;
    }

    Metrics m = {
        .avgTurnaround = totalTAT / n,
        .avgWaiting = totalWT / n,
        .avgResponse = totalRT / n
    };
    return m;
}

// SJF (non-preemptive)
Metrics sjf_metrics(Process proc[], int n) {
    bool completed[n];
    for (int i = 0; i < n; i++) completed[i] = false;

    int completedCount = 0, currentTime = 0;
    float totalTAT = 0, totalWT = 0, totalRT = 0;

    while (completedCount < n) {
        int idx = -1;
        int minBurst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && proc[i].arrivalTime <= currentTime) {
                if (proc[i].burstTime < minBurst) {
                    minBurst = proc[i].burstTime;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            currentTime++;
            continue;
        }

        proc[idx].startTime = currentTime;
        proc[idx].completionTime = currentTime + proc[idx].burstTime;

        int turnaround = proc[idx].completionTime - proc[idx].arrivalTime;
        int waiting = turnaround - proc[idx].burstTime;
        int response = proc[idx].startTime - proc[idx].arrivalTime;

        totalTAT += turnaround;
        totalWT += waiting;
        totalRT += response;

        currentTime = proc[idx].completionTime;
        completed[idx] = true;
        completedCount++;
    }

    Metrics m = {
        .avgTurnaround = totalTAT / n,
        .avgWaiting = totalWT / n,
        .avgResponse = totalRT / n
    };
    return m;
}

// Round Robin
Metrics rr_metrics(Process proc[], int n, int timeQuantum) {
    float totalTAT = 0, totalWT = 0, totalRT = 0;
    int currentTime = 0, completed = 0;
    bool started[n];

    for (int i = 0; i < n; i++) {
        proc[i].remainingTime = proc[i].burstTime;
        proc[i].startTime = -1;
        started[i] = false;
    }

    while (completed < n) {
        bool didSomething = false;

        for (int i = 0; i < n; i++) {
            if (proc[i].remainingTime > 0 && proc[i].arrivalTime <= currentTime) {
                didSomething = true;

                if (!started[i]) {
                    proc[i].startTime = currentTime;
                    started[i] = true;
                }

                int timeSlice = (proc[i].remainingTime > timeQuantum) ? timeQuantum : proc[i].remainingTime;
                proc[i].remainingTime -= timeSlice;
                currentTime += timeSlice;

                if (proc[i].remainingTime == 0) {
                    proc[i].completionTime = currentTime;

                    int turnaround = proc[i].completionTime - proc[i].arrivalTime;
                    int waiting = turnaround - proc[i].burstTime;
                    int response = proc[i].startTime - proc[i].arrivalTime;

                    totalTAT += turnaround;
                    totalWT += waiting;
                    totalRT += response;

                    completed++;
                }
            }
        }

        if (!didSomething) {
            currentTime++;
        }
    }

    Metrics m = {
        .avgTurnaround = totalTAT / n,
        .avgWaiting = totalWT / n,
        .avgResponse = totalRT / n
    };
    return m;
}

