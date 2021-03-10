package bug5;

import java.util.concurrent.BrokenBarrierException;

/**
 * @author cristian.chilipirea
 * 
 *         Solve the dead-lock
 *         
 *         Ordinea de executie:
 *         MyThreadA.A -> Barrier -> MyThreadA.B ->
 *         -> Barrier -> MyThreadB.B -> MyThreadB.A 
 */
public class MyThreadA implements Runnable {

	@Override
	public void run() {
		synchronized (Main.lockA) {
			for (int i = 0; i < Main.N; i++)
				Main.valueA++;
			
			try {
				Main.barrier.await();
			} catch (BrokenBarrierException | InterruptedException e) {
                e.printStackTrace();
			}
			synchronized (Main.lockB) {
				for (int i = 0; i < Main.N; i++)
					Main.valueB++;
			}
		}
	}
}
