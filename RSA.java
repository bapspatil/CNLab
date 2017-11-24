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
		System.out.println("Enter message to encrypt: ");
		String message = in.nextLine();
		int size = message.length();
		char[] msg = new char[size];
		
	}

}
