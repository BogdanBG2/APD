package task6;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

class MyTask extends RecursiveTask<Void> {

	/**
	 *
	 *
	 */
	private static final long serialVersionUID = 1L;
	
	int[] graph;
	int step;
	
	MyTask(int[] graph, int step) {
		this.graph = graph.clone();
		this.step = step;
	}
	@Override
	protected Void compute() {
		if (Main.N == step) {
            Main.printQueens(graph);
            return null;
        }
		
		List<MyTask> tasks = new ArrayList<>();
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (Main.check(newGraph, step)) {
            	MyTask t = new MyTask(newGraph, step + 1);
            	tasks.add(t);
            	t.fork();
                //queens(newGraph, step + 1);
            }
        }

        
        
        for(MyTask t : tasks)
        	t.join();
        
		return null;
	}
}

public class Main {
    public static int N = 4;

    static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }

    public static void queens(int[] graph, int step) {
        if (Main.N == step) {
            printQueens(graph);
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
                queens(newGraph, step + 1);
            }
        }
    }
    public static void main(String[] args) {
        int[] graph = new int[N];
        //queens(graph, 0);
        ForkJoinPool fjp = new ForkJoinPool(4);
        fjp.invoke(new MyTask(graph, 0));
        fjp.shutdown();
    }
}
