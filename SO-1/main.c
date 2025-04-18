#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "algoritmos.h"



// Função principal
int main() {
    Process processes[MAX_PROCESSES];
    int n, choice, input_choice;
    double time_quantum;
   

    // Solicita ao usuário que escolha o algoritmo de escalonamento
    printf("Select Scheduling Algorithm:\n");
    printf("1. First Come First Serve (FCFS)\n");
    printf("2. Round Robin (RR)\n");
    printf("3. Shortest Job First (SJF)\n");
    printf("4. Priority Scheduling (Non-Preemptive)\n");
    printf("5. Priority Scheduling (Preemptive)\n");
    printf("6. Rate Monotonic (RM)\n");
    printf("7. Earliest Deadline First (EDF)\n");
    printf("8. Multilevel Queue Scheduling\n");
    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1 || (choice < 1 || choice > 8)) {
        fprintf(stderr, "Erro: Escolha inválida de algoritmo.\n");
        return 1;
    }

    // Solicita ao usuário o tipo de entrada
    printf("Select Input Type:\n");
    printf("1. Static List of Processes (from file)\n");
    printf("2. Manual Input of Processes\n");
    printf("Enter your choice: ");
    if (scanf("%d", &input_choice) != 1 || (input_choice < 1 || input_choice > 2)) {
        fprintf(stderr, "Erro: Escolha inválida de tipo de entrada.\n");
        return 1;
    }

    // Lida com o tipo de entrada
    if (input_choice == 1) {
        if (load_processes_from_file("lista_estatica.txt", processes, &n) != 0) {
            return 1;
        }
    } else if (input_choice == 2) {
        printf("Enter the number of processes: ");
        if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_PROCESSES) {
            fprintf(stderr, "Erro: Número inválido de processos.\n");
            return 1;
        }
        if (input_process_details(processes, n) != 0) {
            return 1;
        }
    }

    // Solicita o time quantum se o algoritmo escolhido for Round Robin
    if (choice == 2) {
        printf("Enter time quantum for Round Robin: ");
        if (scanf("%lf", &time_quantum) != 1 || time_quantum <= 0) {
            fprintf(stderr, "Erro: Time quantum inválido.\n");
            return 1;
        }
    }

    // Executa o algoritmo escolhido com base na entrada do usuário
    printf("\nSimulating...\n");
    switch (choice) {
        case 1:
            fcfs(processes, n);
            break;
        case 2:
            round_robin(processes, n, time_quantum);
            break;
        case 3:
            sjf(processes, n);
            break;
        case 4:
            priority_scheduling_non_preemptive(processes, n);
            break;
        case 5:
            priority_scheduling_preemptive(processes, n);
            break;
        case 6:
            rate_monotonic(processes, n);
            break;
        case 7:
            earliest_deadline_first(processes, n);
            break;
        case 8:
            multilevel_queue_scheduling(processes, n);
            break;
        default:
            fprintf(stderr, "Erro: Algoritmo inválido.\n");
            return 1;
    }

    // Exibe os resultados dos processos
    print_results(processes, n);

    // Calcula e exibe as estatísticas dos processos
    calculate_statistics(processes, n);

    return 0;
}
