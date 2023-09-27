# Análise comparativa

## Tempos de execução (4 realizações)

### 1 Thread

| **mutex** | **rwlock** | **leitores e escritores** |
|:---------:|:----------:|:-------------------------:|
|  1.071297 |  1.170476  |          1.394116         |
|  1.068921 |  1.186224  |          1.372730         |
|  1.068925 |  1.172868  |          1.366252         |
|  1.070259 |  1.205422  |          1.335773         | 

### 2 Threads

| **mutex** | **rwlock** | **leitores e escritores** |
|:---------:|:----------:|:-------------------------:|
|  3.182811 |  2.566353  |          2.845385         |
|  3.159662 |  2.549745  |          2.804118         |
|  3.161814 |  2.562797  |          2.866682         |
|  3.138835 |  2.567629  |          2.817405         |

### 3 Threads

| **mutex** | **rwlock** | **leitores e escritores** |
|:---------:|:----------:|:-------------------------:|
|  1.879674 |  2.985643  |          3.681861         |
|  1.874216 |  2.903965  |          3.472741         |
|  1.898300 |  2.956810  |          3.580745         |
|  1.902443 |  2.935434  |          3.613182         |

### 4 Threads

| **mutex** | **rwlock** | **leitores e escritores** |
|:---------:|:----------:|:-------------------------:|
|  2.369350 |  6.505655  |          9.481488         |
|  2.714421 |  6.587199  |          9.693357         |
|  2.377477 |  6.700142  |          9.065934         |
|  2.336052 |  6.516161  |          9.680935         |

## Tempo de execução médio

| **Número de threads** |  **mutex** | **rwlock** | **leitores e escritores** |
|:---------------------:|:----------:|:----------:|:-------------------------:|
|           1           |  1.0698505 |  1.1837475 |         1.36721775        |
|           2           |  3.1607805 |  2.561631  |         2.8333975         |
|           3           | 1.88865825 |  2.945463  |         3.58713225        |
|           4           |  2.449325  | 6.57728925 |         9.4804285         |

## Análise

De forma geral, todas as implementações tiveram seu tempo de execução aumentado à medida que o número de threads utilizadas aumentou, exceto na implementação **mutex** com 2 threads. Assim como observado na Atividade 1, esse resultado ocorre devido ao uso de programação concorrente que gera uma demanda de processamento maior do que o ganho de performance com o paralelismo de tarefas.

Comparativamente, a implementação com **mutex** apresentou o melhor desempenho com 1, 3 e 4 threads, enquanto que **rwlock** teve o menor tempo de execução com 2 threads. Além disso, a implementação dos **leitores e escritores** obteve os maiores tempos de execução em todos os casos, havendo uma diferença de aproximadamente 3s do segundo maior tempo com 4 threads.

Esse desempenho melhor do **mutex** ocorre provavelmente por não haver leituras suficientes que justifiquem um gasto maior de processamento com uma implementação com exclusão para leitores, como **rwlock** e essa nova implementação de **leitores e escritores**. Ademais, a diferença de desempenho do **rwlock** e do **leitores e escritores** ocorre possivelmente devido à falta de otimização que essa nova implemenção possui, já que ambas possuem propostas semelhantes de exclusão mútua de threads para acesso à memória compartilhada.

