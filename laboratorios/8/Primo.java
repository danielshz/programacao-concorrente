import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicLong;
import java.lang.Math;

// Classe que define um pool de threads
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;

        this.queue = new LinkedList<Runnable>();
        this.threads = new MyPoolThreads[nThreads];

        for(int i = 0; i < nThreads; i++) {
            this.threads[i] = new MyPoolThreads();
            this.threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if(!this.shutdown) {
                this.queue.addLast(r);
                this.queue.notify();
            }
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown = true;
            queue.notifyAll();
        }

        for(int i = 0; i < nThreads; i++) {
            try {
                threads[i].join(); 
            } catch(InterruptedException e) {
                return; 
            }
        }
    }

    private class MyPoolThreads extends Thread {
        public void run() {
            Runnable r;

            while(true) {
                synchronized(queue) {
                    while(queue.isEmpty() && (!shutdown)) {
                        try { 
                            queue.wait(); 
                        } catch(InterruptedException ignored) {}
                    }

                    if(queue.isEmpty() && shutdown)
                        return;  

                    r = (Runnable) queue.removeFirst();
                }

                try {
                    r.run();
                } catch (RuntimeException e) {}
            } 
        } 
    }
}

// PASSO 1: cria uma classe que implementa a interface Runnable 
class Primo implements Runnable {
    private long n;
    private Contador contador;

    public Primo(long n, Contador contador) {
        this.n = n;
        this.contador = contador; 
    }

    // Metodo executado pela thread
    public void run() {
        if(this.n <= 1)
            return;

        else if(this.n == 2)
            contador.inc();
            
        else if(this.n % 2 == 0)
            return;

        else {
            for(int i = 3; i < Math.sqrt(this.n) + 1; i += 2) {
                if(this.n % i == 0)
                    return;
            }
    
            contador.inc();
        }
    }
}

class PrimoSequencial {
    public static long PrimoQuant(int N) {
        long quant = 0;

        for(int n = 1; n <= N; n++) {
            int i = 3;

            if(n <= 1)
                continue;
    
            else if(n == 2) {
                quant++;
                continue;
            }
         
            else if(n % 2 == 0)
                continue;
    
            else {
                for(i = 3; i < Math.sqrt(n) + 1; i += 2) {
                    if(n % i == 0)
                        break;
                }

                if(i >= Math.sqrt(n) + 1)
                    quant++;
            }
        }

        return quant;
    }
}

class Contador {
    private AtomicLong atomic;

    public Contador(long valorInicial) {
        this.atomic = new AtomicLong(valorInicial);
    }

    public long get() {
        return this.atomic.get();
    }

    public void inc() {
        this.atomic.getAndIncrement();
    }
}

// Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 4;
    private static final int N = 10000;

    public static void main(String[] args) {
        // PASSO 2: cria o pool de threads
        Contador contador = new Contador(0);
        FilaTarefas pool = new FilaTarefas(NTHREADS);
        
        // PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
        for(int i = 1; i <= N; i++) {
            Runnable r = new Primo(i, contador);
            pool.execute(r);
        }

        // PASSO 4: esperar pelo termino das threads
        pool.shutdown();
        
        System.out.print("Quantidade de primos (Concorrente): ");
        System.out.println(contador.get());

        System.out.print("Quantidade de primos (Sequencial): ");
        System.out.println(PrimoSequencial.PrimoQuant(N));
    }
}
