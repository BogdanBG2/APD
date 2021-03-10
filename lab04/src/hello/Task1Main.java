package hello;

public class Task1Main {
	public static void main(String[] args) {
		final int noCores = Runtime.getRuntime().availableProcessors();
		
		Thread[] threads = new Thread[noCores];
		
		for (int i = 0; i < noCores; ++i) {
			threads[i] = new Thread(new Task1(i));
			threads[i].start();
		}
		
		for (int i = 0; i < noCores; ++i) {
			try {
				threads[i].join();
			}
			catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
	}
}
