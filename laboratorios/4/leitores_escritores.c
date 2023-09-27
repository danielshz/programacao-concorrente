#include <pthread.h>
#include "leitores_escritores.h"

void EntraLeitura(
        pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr,
        int* threads_lendo, int* threads_escrevendo
    ) {
    pthread_mutex_lock(mutex);

    while(*threads_escrevendo > 0)
        pthread_cond_wait(cond_leit, mutex);

    (*threads_lendo)++;
    pthread_mutex_unlock(mutex);
}

void SaiLeitura(
        pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr,
        int* threads_lendo, int* threads_escrevendo
    ) {
    pthread_mutex_lock(mutex);
    (*threads_lendo)--;

    if(*threads_lendo == 0)
        pthread_cond_signal(cond_escr);

    pthread_mutex_unlock(mutex);
}

void EntraEscrita(
        pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr,
        int* threads_lendo, int* threads_escrevendo
    ) {
    pthread_mutex_lock(mutex);

    while((*threads_lendo > 0) || (*threads_escrevendo > 0))
        pthread_cond_wait(cond_escr, mutex);

    (*threads_escrevendo)++;
    pthread_mutex_unlock(mutex);
}

void SaiEscrita(
        pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr,
        int* threads_lendo, int* threads_escrevendo
    ) {
    pthread_mutex_lock(mutex);
    (*threads_escrevendo)--;

    pthread_cond_signal(cond_escr);
    pthread_cond_broadcast(cond_leit);

    pthread_mutex_unlock(mutex);
}