import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.OutputStream;
import java.util.Random;



public class DelegateMain {

	public static void main(String[] args) {
		long firstVersionDuration, secondVersionDuration;
		int v1switches, v2switches;
		// generate random data byte array
		Random randomGenerator = new Random();
		byte[] nbyte = new byte[1000000];

		randomGenerator.nextBytes(nbyte);
		ByteArrayInputStream is = new ByteArrayInputStream(nbyte);
		OutputStream os = new ByteArrayOutputStream();

		firstVersionDuration = System.nanoTime();
		v1switches = ClassicMethod.process(is, os);
		firstVersionDuration = System.nanoTime() - firstVersionDuration;
		
		is = new ByteArrayInputStream(nbyte);
		os = new ByteArrayOutputStream();
		
		secondVersionDuration = System.nanoTime();
		v2switches = DelegateMethod.process(is, os);
		secondVersionDuration = System.nanoTime() - secondVersionDuration;

		System.out.println("Classic version switched " + v1switches + " times during : " + firstVersionDuration / 1000 +
				"\nDelegate version switched " + v2switches + " times during : " + secondVersionDuration / 1000);
	}

}
