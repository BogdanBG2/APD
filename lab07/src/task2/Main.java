package task2;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;


class MyRunnable implements Runnable {
	int[] colors;
	int step;
	ExecutorService tpe;
	AtomicInteger inQueue;
	
	MyRunnable(int[] colors, int step, ExecutorService tpe, AtomicInteger inQueue) {
		this.colors = colors.clone();
		this.step = step;
		this.tpe = tpe;
		this.inQueue = inQueue;
	}
	@Override
	public void run() {
		if (step == Main.N) {
            Main.printColors(colors);
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (Main.verifyColors(newColors, step)) {
            	inQueue.incrementAndGet();
            	tpe.submit(new MyRunnable(newColors, step + 1, tpe, inQueue));
            }
        }
        
        int left = inQueue.decrementAndGet();
        if (left == 0)
        	tpe.shutdown();
	}
}

public class Main {
    static int N = 10;
    static int COLORS = 3;
    static int[][] graph = { { 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 }, { 2, 7 },
            { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 }, { 5, 7 }, { 5, 8 }, { 6, 1 },
            { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 }, { 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 },
            { 9, 7 } };

    static void colorGraph(int[] colors, int step) {
        if (step == N) {
            printColors(colors);
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (verifyColors(newColors, step))
                colorGraph(newColors, step + 1);
        }
    }

    static boolean verifyColors(int[] colors, int step) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step))
                return false;
        }
        return true;
    }

    private static boolean isEdge(int a, int b) {
        for (int[] ints : graph) {
            if (ints[0] == a && ints[1] == b)
                return true;
        }
        return false;
    }

    static void printColors(int[] colors) {
        StringBuilder aux = new StringBuilder();
        for (int color : colors) {
            aux.append(color).append(" ");
        }
        System.out.println(aux);
    }

    public static void main(String[] args) {
        int[] colors = new int[N];
        for (int i = 0; i < N; ++i)
        	colors[i] = -1;
        //colorGraph(colors, 0);
        AtomicInteger inQueue = new AtomicInteger(0);
        ExecutorService tpe = Executors.newFixedThreadPool(4);
        inQueue.incrementAndGet();
        tpe.submit(new MyRunnable(colors, 0, tpe, inQueue));
    }
}