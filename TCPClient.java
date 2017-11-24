package bapspatil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.Scanner;

public class TCPClient {

	public static void main(String[] args) throws IOException {
		Socket socket = new Socket("localhost", 1534);	
		Scanner in = new Scanner(System.in);
		System.out.println("Enter file name:");
		String fileRequestedStr = in.nextLine();
		DataOutputStream stream = new DataOutputStream(socket.getOutputStream());
		stream.writeBytes(fileRequestedStr + "\n");
		BufferedReader serverReplyBufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		String reply = serverReplyBufferedReader.readLine();
		if(reply.equals("Yes")) {
			while((reply = serverReplyBufferedReader.readLine()) != null)
				System.out.println(reply);
			serverReplyBufferedReader.close();
			stream.close();
			in.close();
			socket.close();
		} else {
			System.out.println("File not found.");
		}
	}
}
