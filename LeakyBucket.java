package bapspatil;

import java.util.Random;
import java.util.Scanner;

public class LeakyBucket {
	private static final int PACKETS = 5;
	
	public static void main(String[] args) {
		int pSzRm = 0;
		int[] packetSz = new int[PACKETS];
		Scanner in = new Scanner(System.in);
		Random rn = new Random();
		for(int i = 0; i < PACKETS; i++ ) {
			packetSz[i] = rn.nextInt(50);
			System.out.println(i + " " + packetSz[i]);
		}
		System.out.println("Enter the Outpute Rate: ");
		int outputRate = in.nextInt();
		System.out.println("Enter the Bucket Size: ");
		int bucketSize = in.nextInt();
		for(int i = 0; i < PACKETS; i++) {
			if((packetSz[i] + pSzRm) > bucketSize) {
				System.out.println(packetSz[i] + " " + bucketSize);
				System.out.println("Bucket capacity exceeded. PACKETS REJECTED.");
				System.out.println("Packet of size transmitted: " + outputRate);
				pSzRm -= outputRate;
				System.out.println("---Bytes remaining after transmission: " + pSzRm);
			} else {
				pSzRm += packetSz[i];
				System.out.println("Incoming packet size: " + packetSz[i]);
				System.out.println("Bytes remaining to transmit: " + pSzRm);
				if(pSzRm > bucketSize) 
					System.out.println("Bucket capacity exceeded.");
				if(pSzRm <= outputRate) {
					System.out.println("Packet of size transmitted: " + pSzRm);
					pSzRm = 0;
				} else {
					System.out.println("Packet of size transmitted: " + outputRate);
					pSzRm -= outputRate;
					System.out.println("---Bytes remaining after transmission: " + pSzRm);
				}
			}
		}
		
		while(pSzRm > 0) {
			if(pSzRm <= outputRate) {
				System.out.println("Packet of size transmitted: " + pSzRm);
				pSzRm = 0;
			} else {
				System.out.println("Packets of size transmitted: " + outputRate);
				pSzRm -= outputRate;
				System.out.println("---Bytes remaining after transmission: " + pSzRm);
			}
		}
		System.out.println("Bucket is now empty");
	}

}
