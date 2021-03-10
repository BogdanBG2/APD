package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

class MyRunnable implements Runnable {
	ArrayList<Integer> partialPath;
	int destination;
	ExecutorService tpe;
	AtomicInteger inQueue;
	
	MyRunnable(ArrayList<Integer> partialPath, int destination, ExecutorService tpe, AtomicInteger inQueue) {
		this.partialPath = partialPath;
		this.destination = destination;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}
	@Override
	public void run() {
		if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            return;
        }
		
		int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int[] ints : Main.graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                // getPath(newPartialPath, destination);
                inQueue.incrementAndGet();
                tpe.submit(new MyRunnable(newPartialPath, destination, tpe, inQueue));
            }
        }
        
        int left = inQueue.decrementAndGet();
        if (left == 0)
        	tpe.shutdown();
	}
}


public class Main {
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };

    static void getPath(ArrayList<Integer> partialPath, int destination) {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            return;
        }

        // se verifica nodurile pentru a evita ciclarea in graf
        int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        for (int[] ints : graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                getPath(newPartialPath, destination);
            }
        }
    }

    public static void main(String[] args) {
        ArrayList<Integer> partialPath = new ArrayList<>();
        // se vor calcula toate caile de la nodul 0 la nodul 3 in cadrul grafului
        partialPath.add(0);
        //getPath(partialPath, 3);
        AtomicInteger inQueue = new AtomicInteger(0);
        ExecutorService tpe = Executors.newFixedThreadPool(4);
        inQueue.incrementAndGet();
        tpe.submit(new MyRunnable(partialPath, 3, tpe, inQueue));
    }
}
