import java.lang.reflect.Array;
import java.util.Arrays;
import java.util.Random;

public class DelegateMain {
	
	private static int COEF = 1;
	private static int DATA_LEN = 30000/*00*/;

	public static void main(String[] args) {
		compareImplementations();
		long firstVersionDuration, secondVersionDuration;
		int v1switches, v2switches;
		
		// generate random data byte array
		Random randomGenerator = new Random();
		byte[] inputByte = new byte[DATA_LEN];
		byte[] outputByte = new byte[DATA_LEN];
		
		randomGenerator.nextBytes(inputByte);

		firstVersionDuration = System.nanoTime();
		v1switches = ClassicMethod.process(inputByte, outputByte, COEF);
		firstVersionDuration = System.nanoTime() - firstVersionDuration;
		
		secondVersionDuration = System.nanoTime();
		v2switches = DelegateMethod.process(inputByte, outputByte, COEF);
		secondVersionDuration = System.nanoTime() - secondVersionDuration;

		System.out.println("Classic version switched " + v1switches + " times during : " + firstVersionDuration / 1000 +
				"\nDelegate version switched " + v2switches + " times during : " + secondVersionDuration / 1000);
		
	}

	public static void compareImplementations() {
		int v1switches, v2switches;
		{
			byte[] in = {3, 0, 1, 2, 3, 4, 5, 6, 7, 8, 
								1, 10, 20, 30, 40, 50, 60,/**/ 
								2, 42, 42, 42};
			byte[] out = new byte[in.length];
			v1switches = ClassicMethod.process(in, out, COEF);
			System.out.println("v1 : " + Arrays.toString(out) + " (" + v1switches + ")");
			out = new byte[in.length];
			v2switches = DelegateMethod.process(in, out, COEF);
			System.out.println("v2 : " + Arrays.toString(out) + " (" + v2switches + ")");
		}
		
		// generate random data byte array
		Random randomGenerator = new Random();
		byte[] inputByte = new byte[DATA_LEN];
		byte[] outputByte1 = new byte[DATA_LEN];
		byte[] outputByte2 = new byte[DATA_LEN];
		
		Arrays.fill(inputByte, (byte) 10);
		randomGenerator.nextBytes(inputByte);

		v1switches = ClassicMethod.process(inputByte, outputByte1, COEF);
		v2switches = DelegateMethod.process(inputByte, outputByte2, COEF);

		System.out.println("Classic version switched " + v1switches + " times\nDelegate version switched " + v2switches + " times");
		
		for(int i = 0; i < outputByte1.length; i++) {
			if( outputByte1[i] != outputByte2[i] ) {
				for(int j = i - 20; j < i + 10; j++)
					System.out.println("i=" + j + " : d=" + inputByte[j] + " : v1=" + outputByte1[j] + " : v2=" + outputByte2[j]);
				i = outputByte1.length + 1; // or break...
			}
		}
		
	}

}
