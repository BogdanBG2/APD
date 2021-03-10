package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnable implements Runnable {
	int[] graph;
	int step;
	ExecutorService tpe;
	AtomicInteger inQueue;
	
	MyRunnable(int[] graph, int step, ExecutorService tpe, AtomicInteger inQueue) {
		this.graph = graph;
		this.step = step;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}
	
	@Override
	public void run() {
		if (Main.N == step) {
	            Main.printQueens(graph);
	            return;
	        }
	        for (int i = 0; i < Main.N; ++i) {
	            int[] newGraph = graph.clone();
	            newGraph[step] = i;

	            if (Main.check(newGraph, step)) {
	            	inQueue.incrementAndGet();
	            	tpe.submit(new MyRunnable(newGraph, step + 1, tpe, inQueue));
	            }
	        }
        int left = inQueue.decrementAndGet();
        if (left == 0)
        	tpe.shutdown();
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
        AtomicInteger inQueue = new AtomicInteger(0);
    	ExecutorService tpe = Executors.newFixedThreadPool(4);
        
    	int[] graph = new int[N];
        //queens(graph, 0);
    	inQueue.incrementAndGet();
    	tpe.submit(new MyRunnable(graph, 0, tpe, inQueue));
    }
}
