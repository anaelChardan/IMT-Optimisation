import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

	abstract class ProcessingDelegate {
		protected int dataCounter;
		protected int switchCounter;
		protected ProcessingDelegate nextProcessingDelegate;
		public abstract ProcessingDelegate process(InputStream is, OutputStream os, ProcessingDelegate [] delegates) throws IOException;

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
		public ReadLenProcessingDelegate(ProcessingDelegate nextProcessingDelegate) {
			super(nextProcessingDelegate);
		}

		@Override
		public ProcessingDelegate process(InputStream is, OutputStream os, ProcessingDelegate [] delegates) throws IOException {
			int dataRead;

			if( (dataRead = is.read()) != -1 ) {
				for(ProcessingDelegate d : delegates) {
					d.setDataCounter(dataRead);
				}
			}
			
			return (dataRead != -1) ? nextProcessingDelegate : null;
		}
	}

	class Add1ProcessingDelegate extends ProcessingDelegate {
		public Add1ProcessingDelegate(ProcessingDelegate nextProcessingDelegate) {
			super(nextProcessingDelegate);
		}

		@Override
		public ProcessingDelegate process(InputStream is, OutputStream os, ProcessingDelegate [] delegates) throws IOException {
			int dataRead = 0, counter = this.dataCounter;

			while( (counter-- != 0) && (dataRead = is.read()) != -1 ) {
				os.write(dataRead + 1);
			}
			
			if( dataRead != -1 ) {
				switchCounter++;
				return nextProcessingDelegate;
			} else
				return null;
		}
	}

	class Substract1ProcessingDelegate extends ProcessingDelegate {
		public Substract1ProcessingDelegate(
				ProcessingDelegate nextProcessingDelegate) {
			super(nextProcessingDelegate);
		}

		@Override
		public ProcessingDelegate process(InputStream is, OutputStream os, ProcessingDelegate [] delegates) throws IOException {
			int dataRead = 0, counter = this.dataCounter;

			while( (counter-- != 0) && (dataRead = is.read()) != -1 ) {
				os.write(dataRead - 1);
			}
			
			if( dataRead != -1 ) {
				switchCounter++;
				return nextProcessingDelegate;
			} else
				return null;
		}
	}

	class MultiplyBy2ProcessingDelegate extends ProcessingDelegate {
		public MultiplyBy2ProcessingDelegate(
				ProcessingDelegate nextProcessingDelegate) {
			super(nextProcessingDelegate);
		}

		@Override
		public ProcessingDelegate process(InputStream is, OutputStream os, ProcessingDelegate [] delegates) throws IOException {
			int dataRead = 0, counter = this.dataCounter;

			while( (counter-- != 0) && (dataRead = is.read()) != -1 ) {
				os.write(dataRead * 2);
			}
			
			if( dataRead != -1 ) {
				switchCounter++;
				return nextProcessingDelegate;
			} else
				return null;
		}
	}

public class DelegateMethod {

	public static int process(InputStream is, OutputStream os) {
		ProcessingDelegate readLen = new ReadLenProcessingDelegate(null);
		ProcessingDelegate mult2 = new MultiplyBy2ProcessingDelegate(readLen);
		ProcessingDelegate sub1 = new Substract1ProcessingDelegate(mult2);
		ProcessingDelegate add1 = new Add1ProcessingDelegate(sub1);
		readLen.setNextProcessingDelegate(add1);
		
		ProcessingDelegate delegate = readLen;
		ProcessingDelegate [] delegates = {mult2, sub1, add1};
		
		try {
			while( ((delegate = delegate.process(is, os, delegates)) != null) ) {
			}
		} catch (IOException e) {
			System.err.println("Stream read failed : " + e.getLocalizedMessage());
		}

		return mult2.getSwitchCounter() + sub1.getSwitchCounter() + add1.getSwitchCounter();		
	}

}
