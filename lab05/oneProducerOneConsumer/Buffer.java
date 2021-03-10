package oneProducerOneConsumer;
/**
 * @author cristian.chilipirea
 *
 */
public class Buffer {
	int a;

	Buffer() {
		a = -10;
	}
	void put(int value) { // producer
		synchronized(this) {
			if(a != -10) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			a = value;
			notify();
		}
	}

	int get() { // consumer
		int r = a;
		synchronized(this) {
			if(a == -10) {
				try {
					wait();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			r = a;
			a = -10;
			notify();
		}
		return r;
	}
}
