package bapspatil;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

	public static void main(String[] args) throws IOException {
		ServerSocket serverSocket = new ServerSocket(1534);
		Socket socket = serverSocket.accept();
		System.out.println("Connection established.");
		BufferedReader clientRequestBufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		String fileRequestedStr = clientRequestBufferedReader.readLine();
		DataOutputStream stream = new DataOutputStream(socket.getOutputStream());
		File fileRequested = new File(fileRequestedStr);
		boolean hasFile;
		if(fileRequested.exists()) {
			hasFile = true;
			stream.writeBytes("Yes\n");
		} else {
			hasFile = false;
			stream.writeBytes("No\n");
		}
		if(hasFile) {
			FileReader fileReader = new FileReader(fileRequestedStr);
			BufferedReader fileBufferedReader = new BufferedReader(fileReader);
			String fileContents;
			while((fileContents = fileBufferedReader.readLine()) != null)
				stream.writeBytes(fileContents + "\n");
			stream.close();
			fileBufferedReader.close();
			clientRequestBufferedReader.close();
			serverSocket.close();
			socket.close();
			fileReader.close();
		}
	}
}
