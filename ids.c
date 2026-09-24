#include "ids.h"

#include <stdio.h>
#include <stdlib.h>

int contarLinhasArquivo(const char *caminho, int *totalLinhas) {
    FILE *arquivo = fopen(caminho, "r");
    char linha[200];
    int contador = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        contador++;
    }

    fclose(arquivo);
    *totalLinhas = contador;
    return 1;
}

// le os ids do arquivo e os armazena em um vetor
int *carregarListaIds(const char *caminho, int *total) {
    FILE *arquivo = fopen(caminho, "r");
    int contador = 0;
    int id;

    if (!arquivo) {
        printf("Erro ao abrir arquivo de IDs.\n");
        return NULL;
    }

    // primeira leitura para contar quantos IDs existem para poder determinar o tamanho do vetor
    while (fscanf(arquivo, "%d", &id) == 1) {
        contador++;
    }

    rewind(arquivo);
    int *lista = malloc(contador * sizeof(int));
    if (lista == NULL) {
        printf("Erro ao alocar memoria para a lista.\n");
        fclose(arquivo);
        return NULL;
    }

    // segunda leitura para armazenar os IDs no vetor
    for (int i = 0; i < contador; i++) {
        fscanf(arquivo, "%d", &lista[i]);
    }

    fclose(arquivo);
    *total = contador;
    return lista;
}
