#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "algoritmos.h"


// First Come First Serve (FCFS) Scheduling Processa os processos na ordem de chegada.
void fcfs(Process processes[], int n) {
    double current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        current_time += processes[i].burst_time;
    }
}

// Shortest Job First (SJF) Scheduling Processa o processo com o menor tempo de burst disponível.
void sjf(Process processes[], int n) {
    bool completed[MAX_PROCESSES] = {false};
    double current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int shortest_index = -1;
        double shortest_burst = 1e9;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_index = i;
            }
        }

        if (shortest_index == -1) {
            current_time++;
            continue;
        }

        current_time += processes[shortest_index].burst_time;
        processes[shortest_index].waiting_time = current_time - processes[shortest_index].arrival_time - processes[shortest_index].burst_time;
        processes[shortest_index].turnaround_time = processes[shortest_index].waiting_time + processes[shortest_index].burst_time;
        completed[shortest_index] = true;
        completed_count++;
    }
}

// Priority Scheduling (Preemptive) Processa o processo com a maior prioridade disponível, permitindo preempção.
void priority_scheduling_preemptive(Process processes[], int n) {
    double current_time = 0;
    int completed_count = 0;
    bool completed[MAX_PROCESSES] = {false};

    // Inicializa o tempo restante para cada processo
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed_count < n) {
        int highest_priority_index = -1;
        double highest_priority = 1e9;

        // Encontra o processo com a maior prioridade disponível
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }

        // Atualiza o tempo restante do processo selecionado
        processes[highest_priority_index].remaining_time--;
        if (processes[highest_priority_index].remaining_time == 0) {
            completed[highest_priority_index] = true;
            completed_count++;
            processes[highest_priority_index].finish_time = current_time + 1;
            processes[highest_priority_index].turnaround_time = processes[highest_priority_index].finish_time - processes[highest_priority_index].arrival_time;
            processes[highest_priority_index].waiting_time = processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;
        }
        current_time++;
    }
}

// Priority Scheduling (Non-Preemptive) Processa o processo com a maior prioridade disponível.
void priority_scheduling_non_preemptive(Process processes[], int n) {
    bool completed[MAX_PROCESSES] = {false};
    double current_time = 0;
    int completed_count = 0;

    while (completed_count < n) {
        int highest_priority_index = -1;
        double highest_priority = 1e9;

        // Encontra o processo com a maior prioridade disponível
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time && processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }

        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }

        // Atualiza os tempos do processo selecionado
        processes[highest_priority_index].start_time = current_time;
        processes[highest_priority_index].finish_time = current_time + processes[highest_priority_index].burst_time;
        processes[highest_priority_index].turnaround_time = processes[highest_priority_index].finish_time - processes[highest_priority_index].arrival_time;
        processes[highest_priority_index].waiting_time = processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;
        current_time += processes[highest_priority_index].burst_time;
        completed[highest_priority_index] = true;
        completed_count++;
    }
}

// Round Robin Scheduling Processa os processos em fatias de tempo (time quantum).
void round_robin(Process processes[], int n, double time_quantum) {
    double current_time = 0;
    int remaining_processes = n;
    double remaining_time[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }

    while (remaining_processes > 0) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (remaining_time[i] > time_quantum) {
                    current_time += time_quantum;
                    remaining_time[i] -= time_quantum;
                } else {
                    current_time += remaining_time[i];
                    processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
                    processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
                    remaining_time[i] = 0;
                    remaining_processes--;
                }
            }
        }
    }
}

// Rate Monotonic Scheduling (RM) Processa o processo com o menor período disponível.
void rate_monotonic(Process processes[], int n) {
    double current_time = 0;
    int completed_count = 0;

    // Inicializa o tempo restante para cada processo
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed_count < n) {
        int shortest_period_index = -1;
        double shortest_period = 1e9;

        // Encontra o processo com o menor período disponível
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].period < shortest_period) {
                shortest_period = processes[i].period;
                shortest_period_index = i;
            }
        }

        if (shortest_period_index == -1) {
            current_time++;
            continue;
        }

        // Executa o processo com menor período
        processes[shortest_period_index].remaining_time--;
        if (processes[shortest_period_index].remaining_time == 0) {
            completed_count++;
            processes[shortest_period_index].finish_time = current_time + 1;
            processes[shortest_period_index].turnaround_time = processes[shortest_period_index].finish_time - processes[shortest_period_index].arrival_time;
            processes[shortest_period_index].waiting_time = processes[shortest_period_index].turnaround_time - processes[shortest_period_index].burst_time;
        }
        current_time++;
    }
}

// Earliest Deadline First (EDF) Scheduling  Processa o processo com o menor deadline disponível.
void earliest_deadline_first(Process processes[], int n) {
    double current_time = 0;
    int completed_count = 0;

    // Inicializa o tempo restante para cada processo
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed_count < n) {
        int earliest_deadline_index = -1;
        double earliest_deadline = 1e9;

        // Encontra o processo com o menor deadline disponível
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].deadline < earliest_deadline) {
                earliest_deadline = processes[i].deadline;
                earliest_deadline_index = i;
            }
        }

        if (earliest_deadline_index == -1) {
            current_time++;
            continue;
        }

        // Executa o processo com menor deadline
        processes[earliest_deadline_index].remaining_time--;
        if (processes[earliest_deadline_index].remaining_time == 0) {
            completed_count++;
            processes[earliest_deadline_index].finish_time = current_time + 1;
            processes[earliest_deadline_index].turnaround_time = processes[earliest_deadline_index].finish_time - processes[earliest_deadline_index].arrival_time;
            processes[earliest_deadline_index].waiting_time = processes[earliest_deadline_index].turnaround_time - processes[earliest_deadline_index].burst_time;
        }
        current_time++;
    }
}

// Multilevel Queue Scheduling Divide os processos em filas e aplica diferentes políticas de escalonamento.
void multilevel_queue_scheduling(Process processes[], int n) {
    int queue_count = 3; // Número de filas
    Process queues[3][MAX_PROCESSES];
    int queue_sizes[3] = {0};

    // Divide os processos em filas com base no nível da fila
    for (int i = 0; i < n; i++) {
        int level = processes[i].queue_level;
        queues[level][queue_sizes[level]++] = processes[i];
    }

    // Processa cada fila com uma política diferente
    for (int level = 0; level < queue_count; level++) {
        if (level == 0) {
            fcfs(queues[level], queue_sizes[level]); // FCFS para a fila 0
        } else if (level == 1) {
            round_robin(queues[level], queue_sizes[level], 2); // Round Robin para a fila 1
        } else if (level == 2) {
            sjf(queues[level], queue_sizes[level]); // SJF para a fila 2
        }
    }

    // Atualiza os resultados no array original
    for (int level = 0; level < queue_count; level++) {
        for (int i = 0; i < queue_sizes[level]; i++) {
            processes[queues[level][i].id - 1] = queues[level][i];
        }
    }
}

