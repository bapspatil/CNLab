package bapspatil;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Scanner;

public class UDPServer {

	public static void main(String[] args) throws Exception {
		Scanner in = new Scanner(System.in);
		DatagramSocket socket = new DatagramSocket(6794);
		System.out.println("Enter server message: ");
		
		while(true) {
			byte[] buffer = new byte[1000];
			DatagramPacket clientRequest = new DatagramPacket(buffer, buffer.length);
			socket.receive(clientRequest);
			
			String message = in.nextLine();
			byte[] messageBytes = message.getBytes();
			DatagramPacket serverReply = new DatagramPacket(messageBytes, messageBytes.length, clientRequest.getAddress(), clientRequest.getPort());
			socket.send(serverReply);
		}
	}
}
