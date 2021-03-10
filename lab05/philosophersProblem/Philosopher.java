package philosophersProblem;

/**
 * @author cristian.chilipirea
 * 
 */
public class Philosopher implements Runnable {
	Object leftFork, rightFork;
	int id;

	public Philosopher(int id, Object leftFork, Object rightFork) {
		this.leftFork = leftFork;
		this.rightFork = rightFork;
		this.id = id;
	}

	private void sleep() {
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		System.out.println("Ph. " + id + " is thinking...");
		sleep();
		if(id < Main.N - 1)
			synchronized (leftFork) {
				System.out.println("Ph. " + id + " is taking his left fork");
				sleep(); // delay added to make sure the dead-lock is visible
				sleep();
				synchronized (rightFork) {
					System.out.println("Ph. " + id + " is eating...");
					sleep();
					System.out.println("Ph. " + id + " has put down his right fork");
				}
				System.out.println("Ph. " + id + " is back to thinking...");
			}
		else
			synchronized (rightFork) {
				System.out.println("Ph. " + id + " is taking his right fork");
				sleep(); // delay added to make sure the dead-lock is visible
				sleep();
				synchronized (leftFork) {
					System.out.println("Ph. " + id + " is eating...");
					sleep();
					System.out.println("Ph. " + id + " has put down his left fork");
				}
				System.out.println("Ph. " + id + " is back to thinking...");
			}
	}
}
