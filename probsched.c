#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_PROCESSES 100

typedef struct {
    int id;
    double arrival_time;
    double burst_time;
    double priority;
    double start_time;
    double finish_time;
    double turnaround_time;
    double waiting_time;
} Process;

void generate_processes(Process processes[], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        processes[i].arrival_time = (double)rand() / RAND_MAX * 10;  // Poisson-like arrival
        processes[i].burst_time = (double)rand() / RAND_MAX * 5 + 1;  // Exponential burst time
        processes[i].priority = rand() % 10 + 1;  // Random priority
    }
}

void fcfs(Process processes[], int n) {
    double current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].start_time = current_time;
        processes[i].finish_time = current_time + processes[i].burst_time;
        processes[i].turnaround_time = processes[i].finish_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].start_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
    }
}

void print_results(Process processes[], int n) {
    double total_turnaround = 0, total_waiting = 0;
    printf("ID  Arrival  Burst  Start  Finish  Turnaround  Waiting\n");
    for (int i = 0; i < n; i++) {
        printf("%2d  %7.2f  %5.2f  %5.2f  %6.2f  %10.2f  %7.2f\n",
               processes[i].id, processes[i].arrival_time, processes[i].burst_time,
               processes[i].start_time, processes[i].finish_time,
               processes[i].turnaround_time, processes[i].waiting_time);
        total_turnaround += processes[i].turnaround_time;
        total_waiting += processes[i].waiting_time;
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround / n);
    printf("Average Waiting Time: %.2f\n", total_waiting / n);
}

int main() {
    Process processes[MAX_PROCESSES];
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    generate_processes(processes, n);
    fcfs(processes, n);
    print_results(processes, n);
    return 0;
}
