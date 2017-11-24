package bapspatil;

import java.io.IOException;
import java.util.Scanner;

public class RSA {
	
	static int gcd(long m, long n) {
		int r;
		while(n != 0) {
			r = (int) (m % n);
			m = n;
			n = r;
		}
		return (int) m;
	}

	public static void main(String[] args) throws IOException {
		Scanner in = new Scanner(System.in);
		int i, j, k;
		System.out.println("Enter message to encrypt: ");
		String message = in.nextLine();
		int size = message.length();
		char[] msg = new char[size];
		for(i = 0; i < msg.length; i++)
			msg[i] = message.charAt(i);
		
		System.out.println("Enter p & q: ");
		int p = in.nextInt();
		int q = in.nextInt();
		int n = p * q;
		int phi = (p - 1) * (q - 1);
		for(i = 2; i < phi; i++)
			if(gcd(i, phi) == 1)
				break;
		int e = i;
		for(k = 2; k < phi; k++)
			if((e * k - 1) % phi == 0)
				break;
		int d = k;
		System.out.println("e: " + e + "\nd: " + d);
		
		int[] enc = new int[size];
		int[] dec = new int[size];
 		int[] num = new int[size];
 		
		System.out.println("Encryped message: ");
		for(i = 0; i < msg.length; i++) {
			num[i] = (int) msg[i];
			enc[i] = 1;
			for(j = 0; j < e; j++)
				enc[i] = (enc[i] * num[i]) % n;
			System.out.print(enc[i] + " ");
		}
		
		System.out.println("\nDecrypted message: ");
		for(i = 0; i < msg.length; i++) {
			dec[i] = 1;
			for(j = 0; j < d; j++)
				dec[i] = (dec[i] * enc[i]) % n;
			System.out.print((char) dec[i]);
		}
		
		in.close();
	}

}
