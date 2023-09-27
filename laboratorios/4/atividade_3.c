// Programa concorrente que cria e faz operacoes sobre uma lista de inteiros
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "list_int.h"
#include "timer.h"
#include "leitores_escritores.h"

#define QTDE_OPS 10000000 // Quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 // Quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 // Valor máximo a ser inserido

pthread_mutex_t mutex;
pthread_cond_t cond_leit;
pthread_cond_t cond_escr;

// Lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 

// Quantidade de threads no programa
int nthreads;

// Variáveis de condição que representam a quantidade de threads lendo e escrevendo/removendo
int threads_lendo = 0, threads_escrevendo = 0;

// Tarefa das threads
void* tarefa(void* arg) {
    long int id = (long int) arg;
    int op;
    int in, out, read; 

    in = out = read = 0;

    // Realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
    for(long int i = id; i < QTDE_OPS; i += nthreads) {
        op = rand() % 100;

        if(op < 98) {
            EntraLeitura(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo); /* lock de LEITURA */    
            Member(i % MAX_VALUE, head_p);
            SaiLeitura(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo);   

            read++;
        } else if(98 <= op && op < 99) {
            EntraEscrita(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo); /* lock de ESCRITA/REMOÇÃO */    
            Insert(i % MAX_VALUE, &head_p);
            SaiEscrita(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo);

            in++;
        } else if(op >= 99) {
            EntraEscrita(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo); /* lock de ESCRITA/REMOÇÃO */     
            Delete(i % MAX_VALUE, &head_p);
            SaiEscrita(&mutex, &cond_leit, &cond_escr, &threads_lendo, &threads_escrevendo);    

            out++;
        }
    }

    // Registra a quantidade de operacoes realizadas por tipo
    printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
    pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    pthread_t *tid;
    double ini, fim, delta;

    // Verifica se o número de threads foi passado na linha de comando
    if(argc < 2) {
        printf("Digite: %s <numero de threads>\n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);

    // Insere os primeiros elementos na lista
    for(int i = 0; i < QTDE_INI; i++)
        Insert(i % MAX_VALUE, &head_p);  /* Ignore return value */

    //Tomada de tempo inicial
    GET_TIME(ini);

    // Aloca espaco de memoria para o vetor de identificadores de threads no sistema
    tid = malloc(sizeof(pthread_t) * nthreads);

    if(tid == NULL) {  
        printf("--ERRO: malloc()\n"); return 2;
    }

    // Inicializa a variavel mutex
    pthread_mutex_init(&mutex, NULL);

    // Inicializa a variáveis de condição
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);

    // Cria as threads
    for(long int i = 0; i < nthreads; i++) {
        if(pthread_create(tid + i, NULL, tarefa, (void*) i)) {
            printf("--ERRO: pthread_create()\n");
            return 3;
        }
    }

    //aguarda as threads terminarem
    for(int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid + i), NULL)) {
            printf("--ERRO: pthread_join()\n");
            return 4;
        }
    }

    // Tomada de tempo final
    GET_TIME(fim);
    delta = fim - ini;
    printf("Tempo: %lf\n", delta);

    // Libera o mutex
    pthread_mutex_destroy(&mutex);

    // Libera variáveis de condição
    pthread_cond_destroy(&cond_escr);
    pthread_cond_destroy(&cond_leit);

    // Libera o espaco de memoria do vetor de threads
    free(tid);

    // Libera o espaco de memoria da lista
    Free_list(&head_p);

    return 0;
}