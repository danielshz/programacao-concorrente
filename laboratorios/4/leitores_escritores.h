#include <pthread.h>

// Solicita e permite que a thread faça leitura em uma memória compartilhada, utilizando variáveis de condição
void EntraLeitura(pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr, int* threads_lendo, int* threads_escrevendo);

// Sinaliza que a thread terminou o processo de leitura na memória compartilhada, utilizando variáveis de condição
void SaiLeitura(pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr, int* threads_lendo, int* threads_escrevendo);

// Solicita e permite que a thread faça escrita em uma memória compartilhada, utilizando variáveis de condição
void EntraEscrita(pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr, int* threads_lendo, int* threads_escrevendo);

// Sinaliza que a thread terminou o processo de leitura na memória compartilhada, utilizando variáveis de condição
void SaiEscrita(pthread_mutex_t* mutex, pthread_cond_t* cond_leit, pthread_cond_t* cond_escr, int* threads_lendo, int* threads_escrevendo);