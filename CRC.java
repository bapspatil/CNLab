package bapspatil;

import java.util.Scanner;

public class CRC {

	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		
		System.out.println("Enter the number of bits in the message: "); 
		long messageBits = in.nextLong();
		System.out.println("Enter message bits: ");
		long[] message = new long[(int) messageBits];
		for(int i = 0; i < messageBits; i++)
			message[i] = in.nextLong();
		
		System.out.println("Enter the number of bits in generator: ");
		long genBits = in.nextLong();
		System.out.println("Enter generator bits: ");
		long[] gen = new long[(int) genBits];
		for(int i = 0; i < genBits; i++)
			gen[i] = in.nextLong();
		
		long totalBits = messageBits + genBits - 1;
		long[] appMessage = new long[(int) totalBits];
		long[] rem = new long[(int) totalBits];
		
		for(int i = 0; i < message.length; i++)
			appMessage[i] = message[i];
		System.out.println("Appended bits: ");
		for(int i = 0; i < appMessage.length; i++) {
			System.out.print(appMessage[i]);
			rem[i] = appMessage[i];
		}
		
		rem = computeCrc(appMessage, gen, rem);

		long[] transMessage = new long[(int) totalBits];
		System.out.println("\nTransmitted message from transmitter: ");
		for(int i = 0; i < transMessage.length; i++) {
			transMessage[i] = (appMessage[i] ^ rem[i]);
			System.out.print(transMessage[i]);
		}
		
		System.out.println("\nEnter received message at receiver end: ");
		for(int i = 0; i < transMessage.length; i++) {
			transMessage[i] = in.nextLong();
			rem[i] = transMessage[i];
		}
		
		rem = computeCrc(transMessage, gen, rem);
		
		for(int i = 0; i < rem.length; i++) {
			if(rem[i] != 0) {
				System.out.println("Error in the received message!");
				break;
			}
			if(i == rem.length - 1)
				System.out.println("No errors.");
		}
		
		in.close();
	}
	
	static long[] computeCrc(long[] appMessage, long[] gen, long[] rem) {
		int curr = 0;
		while(true) {
			for(int i = 0; i < gen.length; i++)
				rem[curr + i] = (rem[curr + i] ^ gen[i]);
			while(rem[curr] == 0 && curr != rem.length - 1)
				curr++;
			if((rem.length - curr) < gen.length)
				break;
		}
		return rem;
	}
}
