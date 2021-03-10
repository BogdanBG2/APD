package multipleProducersMultipleConsumersNBuffer;

// import java.util.Queue;

/**
 * @author Gabriel Gutu <gabriel.gutu at upb.ro>
 *
 */
public class Buffer {
    
    LimitedQueue queue;
    
    public Buffer(int size) {
        queue = new LimitedQueue(size);
    }

	void put(int value) {
		synchronized(this) {
	        if(queue.size() == queue.getLimit()) {
	        	try {
	        		wait();
	        	} catch (Exception e) {
	        		e.printStackTrace();
	        	}
	        }
	        queue.add(value);
	        notifyAll();
		}
	}

	int get() {
		int r = -1;
		synchronized(this) {
			if(queue.size() == 0) {
				try {
					wait();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			r = (int)queue.poll();
			notifyAll();
		}
        return r;
	}
}
