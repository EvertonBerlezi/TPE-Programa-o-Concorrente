#ifndef PROCESSAMENTO_H
#define PROCESSAMENTO_H

#include <pthread.h>
#include <stdio.h>

extern int *listaIds;
extern int totalIds;
extern int indiceAtual;
extern pthread_mutex_t mutex_leitura;
extern pthread_mutex_t mutex_escrita;
extern FILE *arquivoLog;

void obterTimestamp(char *buffer, int tamanho);
void apiMockada(int id, char *resposta_api);
void *execucaoThread(void *arg);

#endif
