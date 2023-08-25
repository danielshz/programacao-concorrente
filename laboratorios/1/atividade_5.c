#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 8
#define NELEMENTOS 10000

// Funções:
// 1. Função inicializar vetor: vetor[i] = i
// 3. Função elevar ao quadrado: vetor[i] = vetor[i] * vetor[i]
// 2. Função verificar resultado: vetor[i] == vetor[i] * vetor[i]

// Abordagens:
// 1. Dividir o vetor em blocos iguais
// 2. Executar em saltos

typedef struct {
   int inicio, fim, salto;
   long int* vetor;
} t_Args;

void* ElevarAoQuadrado(void *arg) {
    t_Args* args = (t_Args *) arg;

    for(int i = args->inicio; i <= args->fim; i = i + args->salto)
        args->vetor[i] = args->vetor[i] * args->vetor[i];

    pthread_exit(NULL);
}

long int* InicializarVetor(int tamanho) {
    long int* vetor;

    vetor = malloc(tamanho * sizeof(long int));

    if (vetor == NULL) {
      printf("ERRO: malloc()\n");
      exit(-1);
    }

    for(int i = 0; i < tamanho; i++)
        vetor[i] = i;
    
    return vetor;
}

int VerificarResultado(long int* vetor, int tamanho) {
    for(int i = 0; i < tamanho; i++) {
        if(vetor[i] != (i * i))
            return 0;
    }

    return 1;
}

int main() {
    pthread_t tid_sistema[NTHREADS];
    t_Args* args;

    long int* vetor = InicializarVetor(NELEMENTOS);

    // Inicializando args
    args = malloc(NTHREADS * sizeof(t_Args));

    if (args == NULL) {
        printf("ERRO: malloc()\n");
        exit(-1);
    }

    for(int i = 0; i < NTHREADS; i++) 
        args[i].vetor = vetor;  

    // Abordagem 1
    // for(int i = 0; i < NTHREADS; i++) {
    //     args[i].inicio = NELEMENTOS / NTHREADS * i;
    //     args[i].fim = NELEMENTOS / NTHREADS * (i + 1) - 1;
    //     args[i].salto = 1;
    
    //     if (pthread_create(&tid_sistema[i], NULL, ElevarAoQuadrado, (void*) &args[i])) {
    //         printf("--ERRO: pthread_create()\n"); 
    //         exit(-1);
    //     }
    // }

    // Abordagem 2
    for(int i = 0; i < NTHREADS; i++) {
        args[i].inicio = i;
        args[i].fim = NELEMENTOS;
        args[i].salto = NTHREADS;
    
        if (pthread_create(&tid_sistema[i], NULL, ElevarAoQuadrado, (void*) &args[i])) {
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

    // Verifica o resultado
    if(VerificarResultado(vetor, NELEMENTOS))
        printf("Valores corretos\n");
    else
        printf("Valores incorretos\n");

    free(args);
    free(vetor);

    pthread_exit(NULL);
}
