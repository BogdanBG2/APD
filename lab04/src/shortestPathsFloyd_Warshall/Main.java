package shortestPathsFloyd_Warshall;
/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	static int M = 9;
	static int N = 5;
	static int cores = 2;
	static int graph2[][] = { { 0, 1, M, M, M }, 
			          { 1, 0, 1, M, M }, 
			          { M, 1, 0, 1, 1 }, 
			          { M, M, 1, 0, M },
			          { M, M, 1, M, 0 } };
	public static void main(String[] args) {
		
		int graph[][] = { { 0, 1, M, M, M }, 
		          { 1, 0, 1, M, M }, 
		          { M, 1, 0, 1, 1 }, 
		          { M, M, 1, 0, M },
		          { M, M, 1, M, 0 } };
		// Parallelize me (You might want to keep the original code in order to compare)
		for (int k = 0; k < 5; k++) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
				}
			}
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				System.out.print(graph[i][j] + " ");
			System.out.println();
		}
		
		System.out.println("--- Sus e graful secvential");
		System.out.println("--- Jos e graful paralelizat");

		Thread[] t = new Thread[cores];
		for(int i = 0; i < cores; ++i) {
			t[i] = new Thread(new MyThread(i));
			t[i].start();
		}
		for(int i = 0; i < cores; ++i) {
			try {
				t[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				System.out.print(graph2[i][j] + " ");
			System.out.println();
		}
	}
}
