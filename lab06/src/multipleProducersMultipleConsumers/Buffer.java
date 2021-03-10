package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Buffer {
	// int value;
	private static final int capacity = 50;
	private static final int timeout = 1;
	private static final TimeUnit timeout_unit = TimeUnit.SECONDS;

	private ArrayBlockingQueue<Integer> queue;
	
	Buffer() {
		this.queue = new ArrayBlockingQueue<Integer>(capacity);
	}
	
	void put(int value) {
		try {
			this.queue.offer(value, timeout, timeout_unit);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	int get() {
		try {
			return this.queue.poll(timeout, timeout_unit);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}
}
