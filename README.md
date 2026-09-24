# TPE-Programa-o-Concorrente

## Como compilar e rodar
gcc -pthread main.c processamento.c ids.c -o executar
./executar


Para trocar qual lista de IDs usar e o número de threads, edite as linhas 14 e 16
do main.c:

const int numeroExecucoes[] = {1, 8};   // quantidades de threads testadas
const char *caminhoLista = "listas_ids/lista_ids_grande.txt";   // lista usada
