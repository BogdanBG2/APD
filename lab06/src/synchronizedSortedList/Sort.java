package synchronizedSortedList;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;

public class Sort extends Thread {
    private final List<Integer> list;
    private final Semaphore sem;
    
    public Sort(List<Integer> list, Semaphore sem) {
        super();
        this.list = list;
        this.sem = sem;
    }

    @Override
    public void run() {
        try {
        	sem.acquire();
        } catch (InterruptedException e) {
        	e.printStackTrace();
        }
    	
    	Collections.sort(list);
    	sem.release();
    }
}
