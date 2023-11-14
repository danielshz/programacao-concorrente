import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class PrimoTest {

    private Contador contador;
    private FilaTarefas pool;
    private static final int NTHREADS = 4;
    private static final int N = 1000000;
    private long quantidadePrimos;

    @Before
    public void setUp() {
        this.contador = new Contador(0);
        this.pool = new FilaTarefas(NTHREADS);
        this.quantidadePrimos = PrimoSequencial.PrimoQuant(N);
    }

    @Test
    public void quantPrimosCorretude() {
        for(int i = 1; i <= N; i++) {
            Runnable r = new Primo(i, contador);
            pool.execute(r);
        }

        pool.shutdown();

        assertEquals(quantidadePrimos, contador.get());
    }
}