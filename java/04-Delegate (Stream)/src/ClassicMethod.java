import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class ClassicMethod {
	private enum ProcessingModes {
		ReadLen, Add1, Substract1, MultiplyBy2
	}
	
	public static int process(InputStream is, OutputStream os) {
		int switchCount = 0;
		int dataRead, dataCounterInitValue = 0, dataCounter = 0;
		ProcessingModes mode = ProcessingModes.ReadLen;
		
		try {
			while( (dataRead = is.read()) != -1 ) {
				switch( mode ) {
				case ReadLen:
					dataCounter = dataCounterInitValue = dataRead;
					mode = ProcessingModes.Add1;
					break;
				case Add1:
					os.write(dataRead + 1);
					if( --dataCounter == 0 ) {
						mode = ProcessingModes.Substract1;
						dataCounter = dataCounterInitValue;
						switchCount++;
					}
					break;
				case Substract1:
					os.write(dataRead - 1);
					if( --dataCounter == 0 ) {
						mode = ProcessingModes.MultiplyBy2;
						dataCounter = dataCounterInitValue;
						switchCount++;
					}
					break;
				case MultiplyBy2:
					os.write(dataRead * 2);
					if( --dataCounter == 0 ) {
						mode = ProcessingModes.ReadLen;
						dataCounter = dataCounterInitValue;
						switchCount++;
					}
					break;
				}
			}
		} catch (IOException e) {
			System.err.println("Stream read failed : " + e.getLocalizedMessage());
		}
		
		return switchCount;		
	}
}
