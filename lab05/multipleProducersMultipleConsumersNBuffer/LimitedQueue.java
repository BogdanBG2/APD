package multipleProducersMultipleConsumersNBuffer;

import java.util.LinkedList;

/**
 * @author Gabriel Gutu <gabriel.gutu at upb.ro>
 * 
 *         DO NOT MODIFY
 */
public class LimitedQueue extends LinkedList<Object> {

    /**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private final int limit;

    public LimitedQueue(int limit) {
        this.limit = limit;
    }
    
    // Acest getter a trebuit adaugat, deoarece altfel nu putem accesa o variabila privata
    public int getLimit() {
    	return this.limit;
    }
    
    @Override
    public boolean add(Object o) {
        boolean added = super.add(o);
        while (added && size() > limit) {
           super.remove();
        }
        return added;
    }

}
