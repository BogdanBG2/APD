package bug2;
/**
 * @author cristian.chilipirea
 * 
 *         Why does this code not block? We took the same lock twice!
 */
public class MyThread implements Runnable {
	static int i;

	@Override
	public void run() {
		synchronized (this) { // Deja ruleaza primul lacat,
			synchronized (this) { // ceea ce face inutila existenta celui de-al doilea lacat.
				i++;
			}
		}
	}
}
