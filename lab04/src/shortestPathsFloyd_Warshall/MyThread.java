package shortestPathsFloyd_Warshall;

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
		
		for (int k = start; k < end; k++)
			for (int i = 0; i < 5; i++)
				for (int j = 0; j < 5; j++)
					Main.graph2[i][j] = Math.min(Main.graph2[i][k] + Main.graph2[k][j],
							                     Main.graph2[i][j]);
	}

}
