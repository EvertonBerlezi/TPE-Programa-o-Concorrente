#include "processamento.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1

int *listaIds = NULL;
int totalIds = 0;
int indiceAtual = 0;

pthread_mutex_t mutex_leitura = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escrita = PTHREAD_MUTEX_INITIALIZER;

FILE *arquivoLog = NULL;

void obterTimestamp(char *buffer, int tamanho) {
    time_t agora = time(NULL);
    struct tm *info_tempo = localtime(&agora);
    strftime(buffer, tamanho, "%Y-%m-%d %H:%M:%S", info_tempo);
}

void apiMockada(int id, char *resposta_api) {
    usleep(100); // simula a latencia da API
    sprintf(resposta_api, "{\"id\": %d, \"status\": \"ok\", \"valor\": %.2f}", id, (float)id * 4.5);
}

// funcao executada por cada thread de P1
void *execucaoThread(void *arg) {
    int numeroThread = (int)(long)arg;
    char respostaApi[300];
    char timestamp[30];

    while (TRUE) {
        // mutex para cada thread pegar um ID sem condicao de corrida
        pthread_mutex_lock(&mutex_leitura);

        if (indiceAtual >= totalIds) {
            pthread_mutex_unlock(&mutex_leitura);
            break;
        }

        int idProcessado = listaIds[indiceAtual];
        indiceAtual++;

        pthread_mutex_unlock(&mutex_leitura);

        // consulta fora do mutex para permitir o paralelismo
        apiMockada(idProcessado, respostaApi);
        obterTimestamp(timestamp, sizeof(timestamp));

        // mutex para cada thread escrever no log sem condicao de corrida
        pthread_mutex_lock(&mutex_escrita);
        fprintf(arquivoLog, "%s, Thread-%d, %d, %s\n", timestamp, numeroThread, idProcessado, respostaApi);
        pthread_mutex_unlock(&mutex_escrita);
    }

    return NULL;
}
