
public class Tremplin {

	public static void main(String[] args) {
		long firstVersionDuration, secondVersionDuration;
		long iterationCounter = 10000, counter;
		
		// L'utilisation de StringBuilder s'impose vite
		// quand les concaténations se multiplient
		// car String est immuable
		counter = iterationCounter;
		String s = ""; // une instance sacrifiée
		firstVersionDuration = System.nanoTime();
		while( counter-- != 0 ) {
			s += "bon";
		}
		firstVersionDuration = System.nanoTime() - firstVersionDuration;
		
		counter = iterationCounter;
		StringBuilder sb = new StringBuilder();
		secondVersionDuration = System.nanoTime();
		while( counter-- != 0 ) {
			sb.append("bon");
		}
		secondVersionDuration = System.nanoTime() - secondVersionDuration;
		
		System.out.println("String : " + firstVersionDuration / 1000 +
						   " - StringBuilder : " + secondVersionDuration / 1000);
	}

}
