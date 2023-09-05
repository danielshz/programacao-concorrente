#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"

int i_global = 0;
pthread_mutex_t mutex;

typedef struct {
   long long int N;
   long long int* quantidadePrimos;
} t_Args;

int ehPrimo(long long int n) {
    if(n <= 1)
        return 0;

    if(n == 2)
        return 1;

    if(n % 2 == 0)
        return 0;

    for(long long int i = 3; i < sqrt(n) + 1; i += 2) {
        if(n % i == 0)
            return 0;
    }

    return 1;
}

void* QuantidadeDePrimos(void *arg) {
    int i_local, quantidadePrimos = 0;
    t_Args* args = (t_Args *) arg;

    pthread_mutex_lock(&mutex); 
    i_local = i_global;
    i_global++;
    pthread_mutex_unlock(&mutex);

    while(i_local < args->N) {
        if(ehPrimo(i_local))
            quantidadePrimos++;
        
        pthread_mutex_lock(&mutex); 
        i_local = i_global;
        i_global++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex); 
    *(args->quantidadePrimos) += quantidadePrimos;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    long long int quantidadePrimos = 0, N;
    int NTHREADS;
    t_Args* args;
    double inicio, fim, delta;

    GET_TIME(inicio);

    if(argc < 3) {
        printf("Digite: %s <numero maximo testado> <numero de threads>\n", argv[0]);
        return 1;
    }

    N = atoll(argv[1]);
    NTHREADS = atoll(argv[2]);

    if(NTHREADS > N)
        NTHREADS = N;

    pthread_t tid_sistema[NTHREADS];

    // Inicializando args
    args = malloc(NTHREADS * sizeof(t_Args));

    if (args == NULL) {
        printf("ERRO: malloc()\n");
        exit(-1);
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicializacao: %lf\n", delta);

    GET_TIME(inicio);

    for(int i = 0; i < NTHREADS; i++) {
        args[i].N = N;
        args[i].quantidadePrimos = &quantidadePrimos;
    
        if (pthread_create(&tid_sistema[i], NULL, QuantidadeDePrimos, (void*) &args[i])) {
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("ERRO: pthread_join() \n");
            exit(-1); 
        } 
    }

    GET_TIME(fim);   
    delta = fim - inicio;
    printf("Tempo processamento (N %lld) (NTHREADS %d): %lf\n", N, NTHREADS, delta);

    GET_TIME(inicio);

    free(args);

    GET_TIME(fim);   
    delta = fim - inicio;
    printf("Tempo finalizacao: %lf\n", delta);

    printf("Quantidade de primos: %lld\n", quantidadePrimos);

    pthread_exit(NULL);
}
