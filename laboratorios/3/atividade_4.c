#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int contador = 0;
int NTHREADS = 1;

pthread_mutex_t mutex;
pthread_cond_t cond;

void barreira(int nthreads) {
    pthread_mutex_lock(&mutex);
    contador++;

    if (contador < nthreads) {
        pthread_cond_wait(&cond, &mutex);
    } else {
        contador = 0;
        printf("\n");
        pthread_cond_broadcast(&cond);
    }

    pthread_mutex_unlock(&mutex);
}

void* PrintarMensagens(void *t) {
    int* t_id = (int*) t;

    printf("Olá da thread %d\n", *t_id);

    barreira(NTHREADS);

    printf("Que dia bonito %d\n", *t_id);

    barreira(NTHREADS);

    printf("Até breve da thread %d\n", *t_id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    NTHREADS = atoi(argv[1]);

    pthread_t threads[NTHREADS];
    int* t_ids;

    t_ids = malloc(NTHREADS * sizeof(int));

    for(int i = 1; i <= NTHREADS; i++)
        t_ids[i - 1] = i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for(int i = 0; i < NTHREADS; i++)
        pthread_create(&threads[i], NULL, PrintarMensagens, (void *)&t_ids[i]);
    
    for(int i = 0; i < NTHREADS; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    free(t_ids);

    return 0;
}
