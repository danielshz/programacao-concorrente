import sys
import time
import math
from multiprocessing.pool import Pool

def primo(n):
    if n <= 1:
        return 0
    if n == 2:
        return 1
    if n % 2 == 0:
        return 0

    i = 3

    while i < math.sqrt(n) + 1:
        if n % i == 0:
            return 0
        i += 2
    return 1

def primo_concorrente(N):
    inicio = time.time()
    pool = Pool()

    numeros = list(range(1, N))
    resultados = pool.map(primo, numeros)
    total_concorrente = sum(resultados)

    print("Concorrente: {}". format(total_concorrente))

    fim = time.time()
    print("{} segundos\n".format(fim - inicio))

    return total_concorrente

def primo_sequencial(N):
    total_sequencial = 0

    inicio = time.time()

    for i in range(1, N + 1):
        if primo(i):
            total_sequencial += 1
    
    print("Sequencial: {}".format(total_sequencial))

    fim = time.time()
    print("{} segundos".format(fim - inicio))

    return total_sequencial

if __name__ == '__main__':
    # Leitura de argumentos

    if len(sys.argv) == 1:
        print("Escreva um número inteiro positivo N maior igual a 1 como argumento no terminal")
        exit()

    N = 0

    try:
        N = int(sys.argv[1])
        if N <= 0:
            raise Exception
    except:
        print("Argumento deve ser um inteiro positivo maior igual a 1")
        exit()

    # Execução concorrente
    total_concorrente = primo_concorrente(N)

    # Execução sequencial
    total_sequencial = primo_sequencial(N)

    # Teste concorrente == sequencial

    if total_concorrente != total_sequencial:
        print("Erro, execução concorrente incorreta (não possui resultado equivalente ao sequencial)")
        exit()

