
class ArraysReadWriteData { // a struct rather than a class
	public byte[] input;
	public int readIndex;
	public byte[] output;
	public int writeIndex;
}

abstract class ProcessingDelegate {
	protected int dataCounter;
	protected int switchCounter;
	protected ProcessingDelegate nextProcessingDelegate;
	public abstract ProcessingDelegate process(ArraysReadWriteData data, ProcessingDelegate [] delegates);

	public ProcessingDelegate(ProcessingDelegate nextProcessingDelegate) {
		super();
		this.nextProcessingDelegate = nextProcessingDelegate;
	}

	public void setDataCounter(int dataCounter) {
		this.dataCounter = dataCounter;
	}

	public void setNextProcessingDelegate(ProcessingDelegate nextProcessingDelegate) {
		this.nextProcessingDelegate = nextProcessingDelegate;
	}

	public int getSwitchCounter() {
		return switchCounter;
	}
}

class ReadLenProcessingDelegate extends ProcessingDelegate {
	protected int coeff;
	
	public ReadLenProcessingDelegate(int coef) {
		super(null);
		this.coeff = coef;
	}

	@Override
	public ProcessingDelegate process(ArraysReadWriteData data, ProcessingDelegate [] delegates) {
		int dataCount;

		dataCount = data.input[data.readIndex++];
		dataCount = Math.abs(dataCount) * coeff;
		for(ProcessingDelegate d : delegates) {
			d.setDataCounter(dataCount);
		}
		if( data.readIndex < data.input.length ) {
			return dataCount != 0 ? nextProcessingDelegate : this;
		}
		return null;
	}
}

class Add1ProcessingDelegate extends ProcessingDelegate {
	public Add1ProcessingDelegate(ProcessingDelegate nextProcessingDelegate) {
		super(nextProcessingDelegate);
	}

	@Override
	public ProcessingDelegate process(ArraysReadWriteData data, ProcessingDelegate [] delegates) {
		int counter = Math.min(this.dataCounter, data.input.length - data.readIndex);

		while( counter-- != 0) {
			data.output[data.writeIndex++] = (byte) (data.input[data.readIndex++] + 1);
			//data.output[data.writeIndex++] = 1;  data.readIndex++;
			//data.output[data.writeIndex++] = (byte) (counter + 1);  data.readIndex++;
		}

			switchCounter++;
		if( data.readIndex < data.input.length ) {
			return nextProcessingDelegate;
		}
		return null;
	}
}

class Substract1ProcessingDelegate extends ProcessingDelegate {
	public Substract1ProcessingDelegate(
			ProcessingDelegate nextProcessingDelegate) {
		super(nextProcessingDelegate);
	}

	@Override
	public ProcessingDelegate process(ArraysReadWriteData data, ProcessingDelegate [] delegates) {
		int counter = Math.min(this.dataCounter, data.input.length - data.readIndex);

		while( counter-- != 0) {
			data.output[data.writeIndex++] = (byte) (data.input[data.readIndex++] - 1);
			//data.output[data.writeIndex++] = -1; data.readIndex++;
			//data.output[data.writeIndex++] = (byte) (counter + 1); data.readIndex++;
		}

			switchCounter++;
		if( data.readIndex < data.input.length ) {
			return nextProcessingDelegate;
		} 
		return null;
	}
}

class MultiplyBy2ProcessingDelegate extends ProcessingDelegate {
	public MultiplyBy2ProcessingDelegate(
			ProcessingDelegate nextProcessingDelegate) {
		super(nextProcessingDelegate);
	}

	@Override
	public ProcessingDelegate process(ArraysReadWriteData data, ProcessingDelegate [] delegates) {
		int counter = Math.min(this.dataCounter, data.input.length - data.readIndex);

		while( counter-- != 0) {
			data.output[data.writeIndex++] = (byte) (data.input[data.readIndex++] * 2);
			//data.output[data.writeIndex++] = 2; data.readIndex++;
			//data.output[data.writeIndex++] = (byte) (counter + 1); data.readIndex++;
		}

			switchCounter++;
		if( data.readIndex < data.input.length ) {
			return nextProcessingDelegate;
		}
		return null;
	}
}

public class DelegateMethod {

	public static int process(byte [] input, byte [] output, int coef) {
		ProcessingDelegate readLen = new ReadLenProcessingDelegate(coef);
		ProcessingDelegate mult2 = new MultiplyBy2ProcessingDelegate(readLen);
		ProcessingDelegate sub1 = new Substract1ProcessingDelegate(mult2);
		ProcessingDelegate add1 = new Add1ProcessingDelegate(sub1);
		readLen.setNextProcessingDelegate(add1);

		ProcessingDelegate delegate = readLen;
		ProcessingDelegate [] delegates = {mult2, sub1, add1};

		ArraysReadWriteData data = new ArraysReadWriteData();
		data.input = input;
		data.output = output; 

		while( ((delegate = delegate.process(data, delegates)) != null) ) {
		}

		return mult2.getSwitchCounter() + sub1.getSwitchCounter() + add1.getSwitchCounter();		
	}

}
