#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#include "ids.h"
#include "processamento.h"

int main() {
    const int totalExecucoes = 2;
                                // vvv numero de threads a serem executadas em cada execucao do P1
    const int numeroExecucoes[] = {1, 8};
                                            // caminho do arquivo de lista de IDs
    const char *caminhoLista = "listas_ids/lista_ids_grande.txt";
    double tempos[totalExecucoes];
    char statusExecucoes[totalExecucoes][80];
    int enriquecimentoIncompleto[totalExecucoes];
    int totalIdsEntrada;

    if (!contarLinhasArquivo(caminhoLista, &totalIdsEntrada)) {
        return 1;
    }

    for (int i = 0; i < totalExecucoes; i++) {
        int numeroThreads = numeroExecucoes[i];
        const char *nomeLog = (numeroThreads == 1)
            ? "log_1_thread.txt"
            : "log_n_threads.txt";

        // cronometragem e criacao do P1
        struct timespec inicio;
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork falhou");
            return 1;
        }

        // execucao do P1
        if (pid == 0) {
            // leitura da lista
            listaIds = carregarListaIds(caminhoLista, &totalIds);
            if (listaIds == NULL) {
                exit(1);
            }

            arquivoLog = fopen(nomeLog, "w");
            if (arquivoLog == NULL) {
                printf("Erro ao abrir arquivo de log.\n");
                exit(1);
            }

            // criacao de 1 ou N threads
            pthread_t threads[numeroThreads];

            for (int j = 0; j < numeroThreads; j++) {
                pthread_create(&threads[j], NULL, execucaoThread, (void *)(long)(j + 1));
            }

            for (int j = 0; j < numeroThreads; j++) {
                pthread_join(threads[j], NULL);
            }

            fclose(arquivoLog);
            free(listaIds);

            exit(0);
        }

        // P0 espera o filho terminar e para a contagem do tempo.
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid falhou");
            return 1;
        }

        struct timespec fim;
        clock_gettime(CLOCK_MONOTONIC, &fim);
        double tempoTotal = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1000000000.0;
        int linhasLog;
        if (!contarLinhasArquivo(nomeLog, &linhasLog)) {
            linhasLog = -1;
        }
        const char *statusP1;

        if (WIFEXITED(status)) {
            statusP1 = (WEXITSTATUS(status) == 0)
                ? "termino normal"
                : "termino com erro";
        } else if (WIFSIGNALED(status)) {
            statusP1 = "termino por sinal";
        } else {
            statusP1 = "termino desconhecido";
        }

        tempos[i] = tempoTotal;
        snprintf(statusExecucoes[i], sizeof(statusExecucoes[i]), "%s", statusP1);
        enriquecimentoIncompleto[i] = (linhasLog != totalIdsEntrada);
    }

    printf("\nRelatorio:\n");
    printf("N threads | Tamanho da lista | Tempo total (s) | Status\n");
    for (int i = 0; i < totalExecucoes; i++) {
        printf("%9d | %16d | %15.3f | %s%s\n",
               numeroExecucoes[i],
               totalIdsEntrada,
               tempos[i],
               statusExecucoes[i],
               enriquecimentoIncompleto[i]
                   ? "; enriquecimento incompleto"
                   : "");
    }

    return 0;
}


