package bugConcurrentHashMap;

import java.util.concurrent.ConcurrentHashMap;

public class MyThread implements Runnable {
	public static ConcurrentHashMap<Integer, Integer> hashMap = new ConcurrentHashMap<>();
	private final int id;

	public MyThread(int id) {
		this.id = id;
	}

	private void addValue(int key, int value) {
		Integer previous_value = hashMap.putIfAbsent(key, value);
		
		if(previous_value != null)
			hashMap.put(key, previous_value + value); 
	}
	
	@Override
	public void run() {
		if (id == 0) {
			for (int i = 0; i < Main.N; i++) {
				addValue(i, 2 * i);
			}
		} else {
			for (int i = 0; i < Main.N; i++) {
				addValue(i, 3 * i);
			}
		}
	}
}
