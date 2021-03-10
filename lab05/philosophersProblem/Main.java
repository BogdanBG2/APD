package philosophersProblem;
/**
 * @author cristian.chilipirea
 * 
 */
public class Main {
	public static final int N = 6;

	public static void main(String[] args) {
		Thread threads[] = new Thread[N];
		Object forks[] = new Object[N];
		System.out.println("Philosophers problem!");
		
		
		for (int i = 0; i < N; i++)
			forks[i] = new Object();
		for (int i = 0; i < N; i++)
			threads[i] = new Thread(new Philosopher(i, forks[i], forks[(i+1)%N]));
			
		//threads[N - 1] = new Thread(new Philosopher(N - 1, forks[0], forks[N - 1]));
		
		for (int i = 0; i < N; i++)
			threads[i].start();

		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
}