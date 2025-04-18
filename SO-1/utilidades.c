#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "algoritmos.h"

// Função para solicitar os detalhes dos processos
int input_process_details(Process processes[], int n) {
    printf("Enter process details (arrival_time, burst_time priority, deadline, period, queue_level):\n");
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1; // Define o ID do processo
        printf("Process %d:\n", i + 1);

        // Solicita o tempo de chegada
        printf("  Arrival time: ");
        if (scanf("%lf", &processes[i].arrival_time) != 1 || processes[i].arrival_time < 0) {
            fprintf(stderr, "Erro: Tempo de chegada inválido para o processo %d.\n", i + 1);
            return 1;
        }

        // Solicita o tempo de burst
        printf("  Burst time: ");
        if (scanf("%lf", &processes[i].burst_time) != 1 || processes[i].burst_time <= 0) {
            fprintf(stderr, "Erro: Tempo de burst inválido para o processo %d.\n", i + 1);
            return 1;
        }

        // Solicita a prioridade
        printf("  Priority: ");
        if (scanf("%lf", &processes[i].priority) != 1 || processes[i].priority < 0) {
            fprintf(stderr, "Erro: Prioridade inválida para o processo %d.\n", i + 1);
            return 1;
        }

        // Solicita o deadline
        printf("  Deadline: ");
        if (scanf("%lf", &processes[i].deadline) != 1 || processes[i].deadline < 0) {
            fprintf(stderr, "Erro: Deadline inválido para o processo %d.\n", i + 1);
            return 1;
        }

        // Solicita o período (para Rate Monotonic)
        printf("  Period: ");
        if (scanf("%lf", &processes[i].period) != 1 || processes[i].period < 0) {
            fprintf(stderr, "Erro: Período inválido para o processo %d.\n", i + 1);
            return 1;
        }

        // Solicita o nível da fila (para Multilevel Queue Scheduling)
        printf("  Queue Level (0, 1, 2): ");
        do {
            if (scanf("%d", &processes[i].queue_level) != 1 || processes[i].queue_level < 0 || processes[i].queue_level > 2) {
                fprintf(stderr, "Erro: Nível de fila inválido para o processo %d. Tente novamente.\n", i + 1);
                while (getchar() != '\n'); // Limpa o buffer de entrada
            }
        } while (processes[i].queue_level < 0 || processes[i].queue_level > 2);

        printf("Tempo de espera: ");
        if (scanf("%lf", &processes[i].waiting_time) != 1 || processes[i].waiting_time < 0) {
            fprintf(stderr, "Erro: Nível de fila inválido para o processo %d.\n", i + 1);
            return 1;
        }

    	printf("Tempo de retorno: ");
        if (scanf("%lf", &processes[i].turnaround_time) != 1 || processes[i].turnaround_time < 0) {
            fprintf(stderr, "Erro: Nível de fila inválido para o processo %d.\n", i + 1);
            return 1;
        }

        printf("Tempo de inicio: ");
        if (scanf("%lf", &processes[i].start_time) != 1 || processes[i].start_time < 0) {
            fprintf(stderr, "Erro: Nível de fila inválido para o processo %d.\n", i + 1);
            return 1;
        }

        printf("Tempo de termino: ");
        if (scanf("%lf", &processes[i].finish_time) != 1 || processes[i].finish_time < 0) {
            fprintf(stderr, "Erro: Nível de fila inválido para o processo %d.\n", i + 1);
            return 1;
        }

        printf("Tempo restante: ");
        if (scanf("%lf", &processes[i].remaining_time) != 1 || processes[i].remaining_time < 0) {
            fprintf(stderr, "Erro: Nível de fila inválido para o processo %d.\n", i + 1);
            return 1;
        }
    }
    return 0;
}

// Carrega processos a partir de lista_estatica.txt
int load_processes_from_file(const char *filename, Process processes[], int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s.\n", filename);
        return 1;
    }

    int i = 0;
    while (fscanf(file, "%d %lf %lf %lf %lf %lf %d",
                  &processes[i].id,
                  &processes[i].arrival_time,
                  &processes[i].burst_time,
                  &processes[i].priority,
                  &processes[i].deadline,
                  &processes[i].period,
                  &processes[i].queue_level) == 7) {
        i++;
        if (i >= MAX_PROCESSES) {
            fprintf(stderr, "Erro: Número máximo de processos excedido.\n");
            fclose(file);
            return 1;
        }
    }

    *n = i;
    fclose(file);
    printf("Processos carregados com sucesso do arquivo %s.\n", filename);
    return 0;
}

// Imprime os resultados dos processos
void print_results(Process processes[], int n) {
    printf("ID\tArrival\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n",
               processes[i].id,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].turnaround_time);
    }
}

// Calcula as estatísticas dos processos
void calculate_statistics(Process processes[], int n) {
    double total_turnaround = 0, total_waiting = 0, total_idle_time = 0;
    double throughput = 0, cpu_utilization = 0;
    int deadline_misses = 0;
    double simulation_time = 0;

    // Calcula o tempo total de turnaround e espera
    for (int i = 0; i < n; i++) {
        total_turnaround += processes[i].turnaround_time;
        total_waiting += processes[i].waiting_time;

        // Verifica se o processo perdeu o prazo (exemplo: prazo arbitrário de 10 unidades de tempo)
        if (processes[i].finish_time > processes[i].arrival_time + 10) {
            deadline_misses++;
        }

        // Atualiza o tempo total de simulação com o maior finish_time
        if (processes[i].finish_time > simulation_time) {
            simulation_time = processes[i].finish_time;
        }
    }

    // Calcula o tempo ocioso total da CPU
    for (int i = 1; i < n; i++) {
        if (processes[i].arrival_time > processes[i - 1].finish_time) {
            total_idle_time += processes[i].arrival_time - processes[i - 1].finish_time;
        }
    }

    // Garante que o tempo de simulação seja maior que 0
    if (simulation_time <= 0) {
        fprintf(stderr, "Aviso: Tempo de simulação inválido (simulation_time <= 0). Ajustando para 1.\n");
        simulation_time = 1.0; // Define um valor padrão para evitar divisões por zero
    }

    // Calcula a utilização da CPU
    cpu_utilization = ((simulation_time - total_idle_time) / simulation_time) * 100;

    // Calcula o throughput (número de processos concluídos por unidade de tempo)
    throughput = n / simulation_time;

    // Exibe as estatísticas
    printf("\nEstatísticas:\n");
    printf("Tempo médio de Turnaround: %.2f\n", total_turnaround / n);
    printf("Tempo médio de Espera: %.2f\n", total_waiting / n);
    printf("Utilização da CPU: %.2f%%\n", cpu_utilization);
    printf("Throughput: %.2f processos por unidade de tempo\n", throughput);
    printf("Perdas de Prazo: %d\n", deadline_misses);
}
