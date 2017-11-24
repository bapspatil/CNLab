package bapspatil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class TCPClient {

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		Socket s = new Socket("localhost", 1534);	
		BufferedReader k = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Enter file name:");
		String filename = k.readLine();
		DataOutputStream d = new DataOutputStream(s.getOutputStream());
		d.writeBytes(filename + "\n");
		BufferedReader i = new BufferedReader(new InputStreamReader(s.getInputStream()));
		String st;
		st = i.readLine();
		if(st.equals("Yes")) {
			while((st = i.readLine()) != null)
				System.out.println(st);
			i.close();
			d.close();
			k.close();
			s.close();
		} else {
			System.out.println("File not found.");
		}
	}
}
