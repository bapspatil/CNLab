package bapspatil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class TCPClient {

	public static void main(String[] args) throws IOException {
		Socket socket = new Socket("localhost", 1534);	
		BufferedReader filenameBufferedReader = new BufferedReader(new InputStreamReader(System.in));
		System.out.println("Enter file name:");
		String fileRequestedStr = filenameBufferedReader.readLine();
		DataOutputStream stream = new DataOutputStream(socket.getOutputStream());
		stream.writeBytes(fileRequestedStr + "\n");
		BufferedReader serverReplyBufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		String reply = serverReplyBufferedReader.readLine();
		if(reply.equals("Yes")) {
			while((reply = serverReplyBufferedReader.readLine()) != null)
				System.out.println(reply);
			serverReplyBufferedReader.close();
			stream.close();
			filenameBufferedReader.close();
			socket.close();
		} else {
			System.out.println("File not found.");
		}
	}
}
