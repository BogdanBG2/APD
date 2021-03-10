package doubleVectorElements;
/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	static int N = 30000;
	static int cores = 8;
	static int v[] = new int[N];

	public static void main(String[] args) {
				
		for(int i=0;i<N;i++)
			v[i]=i;
		
		// Parallelize me
		Thread[] threads = new Thread[cores];
		for (int i = 0; i < cores; ++i) {
			threads[i] = new Thread(new MyThread(i));
			threads[i].start();
		}
		
		for (int i = 0; i < cores; ++i) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		for (int i = 0; i < N; i++) {
			if(v[i] != i*2) {
				System.out.println("Wrong answer");
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
