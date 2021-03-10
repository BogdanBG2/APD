package doubleVectorElements;

public class MyThread implements Runnable {
	
	private int id;
	
	MyThread(int id) {
		this.id = id;
	}
	@Override
	public void run() {
		// TODO Auto-generated method stub
		int start = id * Main.N / Main.cores;
		int end = Math.min(Main.N, (id + 1) * Main.N / Main.cores);
		
		for(int i = start; i < end; ++i)
			Main.v[i] = Main.v[i] * 2;
	}
	
}
