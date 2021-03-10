package synchronizedSortedList;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

public class Reader extends Thread {
    private final String filename;
    private final List<Integer> list;
    private final Semaphore sem;
    
    public Reader(String filename, List<Integer> list, Semaphore sem) {
        super();
        this.filename = filename;
        this.list = list;
        this.sem = sem;
    }

    @Override
    public void run() {
        try {
            @SuppressWarnings("resource")
			Scanner scanner = new Scanner(new File(filename));
            while (scanner.hasNextInt()) {
                list.add(scanner.nextInt());
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } finally {
        	sem.release();
        }
    }
}
