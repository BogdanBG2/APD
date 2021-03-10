package parallelBinarySearch;

public class Main {
	public static final int[] v = {0, 2, 4, 6, 7, 8, 10, 11, 12, 13, 16, 17, 20, 21, 23, 29};
	public static final int N = 16;
	public static final int P = 4;
	public static final int X = 11;
	public static int I;
	public static boolean found = false;
	
	public static void main(String[] args) {
		Thread[] threads = new Thread[P];
		for (int i = 0; i < P; ++i)
			threads[i] = new Thread(new MyThread(i));
		
		// Starting the threads
		for (int i = 0; i < P; ++i)
			threads[i].start();
		
		// Finishing the threads
		for (int i = 0; i < P; ++i) {
			try {
				threads[i].join();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		if(found)
			System.out.println(X + " gasit la indicele " + I);
		else
			System.out.println(X + " nu se afla in vectorul dat");
	}

}
