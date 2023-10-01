#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define TAMANHO_TEXTO_MAX 100

// Variaveis globais
sem_t slotCheio, slotVazio;  // Semaforos para sincronizacao por condicao
sem_t mutexGeral; // Semaforo geral de sincronizacao por exclusao mutua

int N; // Quantidade de threads no programa e de elementos de Buffer
char** Buffer = NULL;

// Imprime o buffer
void printBuffer(char** buffer, int tam) {
    printf("\n----------------------------------------");
    printf("\nBuffer atual: \n");

    for(int i = 0; i < tam; i++) 
        printf("%s", buffer[i]); 
    puts("----------------------------------------\n");
}

// Funcao para inserir um elemento no buffer
void Insere(char *item) {
    static int in = 0;

    sem_wait(&slotVazio); // Aguarda slot vazio para inserir
    sem_wait(&mutexGeral); // Exclusao mutua entre produtores

    strcpy(Buffer[in], item);

    in = (in + 1) % N;
    printf("Produtor: inseriu - %s", item);
    printBuffer(Buffer, N); // Log

    sem_post(&mutexGeral);
    sem_post(&slotCheio); // Sinaliza um slot cheio
}

// Funcao para retirar  um elemento no buffer
void Retira(int id) {
    char* item;
    static int out = 0;

    sem_wait(&slotCheio); // Aguarda slot cheio para retirar
    sem_wait(&mutexGeral); // Exclusao mutua entre consumidores

    item = Buffer[out];
    printf("Consumidor[%d]: retirou - %s", id, item);

    // RemoverString(Buffer, out);
    Buffer[out][0] = 0;

    out = (out + 1) % N;
    printBuffer(Buffer, N); // Log

    sem_post(&mutexGeral);
    sem_post(&slotVazio); // Sinaliza um slot vazio
}

//----------------------------------------------------------
// Produtor 
void LerArquivo(char* diretorio) {
    FILE *fptr;

    // Abre arquivo para leitura
    fptr = fopen(diretorio, "r");

    // Armazena a linha atual do arquivo
    char linha_texto[TAMANHO_TEXTO_MAX];

    // Verifica se o arquivo existe
    if(fptr == NULL) {
        printf("Diretório inválido\n");
        exit(1);
    }

    // Lê as linhas do arquivo e as imprime
    while(fgets(linha_texto, TAMANHO_TEXTO_MAX, fptr)) {
        Insere(linha_texto);

        // Gera delay
        sleep(1);
    }

    // Fecha o arquivo
    fclose(fptr);
}

//----------------------------------------------------------
// Consumidor
void *consumidor(void * arg) {
    int id = *(int *)(arg);
    free(arg);

    while(1) {
        Retira(id); // Retira o proximo item
        
        // Gera delay
        sleep(10);
    }

    pthread_exit(NULL);
}

// Alocar memória para o Buffer de strings
char** AlocarBuffer(int numeroElementos, int tamanhoTexto) {
    char **vetorDeStrings = NULL;

    vetorDeStrings = malloc(numeroElementos * sizeof(char*));

    if(vetorDeStrings == NULL)
        exit(1);

    for(int i = 0; i < numeroElementos; i++) {
        vetorDeStrings[i] = malloc((tamanhoTexto) * sizeof(char));

        if(vetorDeStrings[i] == NULL)
            exit(1);
    }

    return vetorDeStrings;
}

// Funcao principal
int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    char *diretorio = argv[2];

    // Aloca memória do Buffer
    Buffer = AlocarBuffer(N, TAMANHO_TEXTO_MAX);

    pthread_t tid[N];
    int *id;

    sem_init(&mutexGeral, 0, 1);
    sem_init(&slotCheio, 0, 0);
    sem_init(&slotVazio, 0, N);

    // Inicia os threads consumidores
    for(int i = 1; i <= N; i++) {
        id = (int *) malloc(sizeof(int));
        *id = i;

        if(pthread_create(&tid[i], NULL, consumidor, (void *) (id))) {
            printf("Erro na criacao da thread consumidora\n");
            exit(1);
        }
    }

    // Executa função de ler arquivo (consumidora)
    LerArquivo(diretorio);

    // Libera memória do Buffer
    // free(Buffer);
    pthread_exit(NULL);
}