
public class ClassicMethod {
	private enum ProcessingModes {
		ReadLen, Add1, Substract1, MultiplyBy2
	}

	public static int process(byte [] input, byte [] output, int coef) {
		int switchCount = 0;
		byte dataRead;
		int readCounter = input.length, readIndex = 0, writeIndex = 0, dataCounterInitValue = 0, dataCounter = 0;
		ProcessingModes mode = ProcessingModes.ReadLen;

		while( readCounter-- != 0 ) {
			dataRead = input[readIndex++];
			switch( mode ) {
			case ReadLen:
				dataCounterInitValue = Math.abs(dataRead) * coef;
				dataCounter = Math.min(readCounter, dataCounterInitValue);
				if( dataCounter != 0 ) {
					mode = ProcessingModes.Add1;
				}
				break;
			case Add1:
				output[writeIndex++] = (byte) (dataRead + 1);
				//output[writeIndex++] = 1;
				//output[writeIndex++] = dataCounter;
				if( --dataCounter == 0 ) {
					mode = ProcessingModes.Substract1;
					dataCounter = Math.min(readCounter, dataCounterInitValue);
					switchCount++;
				}
				break;
			case Substract1:
				output[writeIndex++] = (byte) (dataRead - 1);
				//output[writeIndex++] = -1;
				//output[writeIndex++] = dataCounter;
				if( --dataCounter == 0 ) {
					mode = ProcessingModes.MultiplyBy2;
					dataCounter = Math.min(readCounter, dataCounterInitValue);
					switchCount++;
				}
				break;
			case MultiplyBy2:
				output[writeIndex++] = (byte) (dataRead *2);
				//output[writeIndex++] = 2;
				//output[writeIndex++] = dataCounter;
				if( --dataCounter == 0 ) {
					mode = ProcessingModes.ReadLen;
					switchCount++;
				}
				break;
			}
		}

		return switchCount;		
	}
}
