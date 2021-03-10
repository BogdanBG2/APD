package parallelBinarySearch;

public class MyThread implements Runnable {

	private int id;
	MyThread(int id) {
		this.id = id;
	}
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		int start = id * Main.N / Main.P;
		int end = Math.min(Main.N - 1, (id + 1) * Main.N / Main.P);
		
		int left = start;
		int right = end;
		if (Main.X >= Main.v[start] && Main.X <= Main.v[end])
			while (left <= right && !Main.found) {
				int m = left + (right - left) / 2;
				if(Main.v[m] == Main.X) {
					Main.found = true;
					System.out.println("Am gasit numarul dorit in intervalul thread-ului " + id);
					Main.I = m;
					break;
				}
				else if (Main.v[m] > Main.X)
					right = m - 1;
				else
					left = m + 1;
			}
		// If the given number is not in the thread interval,
		// it won't go in the loop
		System.out.println("Am terminat thread-ul " + id);
	}

}
