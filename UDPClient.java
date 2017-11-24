package bapspatil;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient {

	public static void main(String[] args) throws Exception {
		DatagramSocket socket = new DatagramSocket();
		String requestMessage = "Send me a text message.";
		byte[] requestMessageBytes = requestMessage.getBytes();
		InetAddress host = InetAddress.getByName("127.0.0.1");
		int serverSocket = 6794;
		while(true) {
			DatagramPacket clientRequest = new DatagramPacket(requestMessageBytes, requestMessageBytes.length, host, serverSocket);
			socket.send(clientRequest);
			byte[] buffer = new byte[1000];
			DatagramPacket serverReply = new DatagramPacket(buffer, buffer.length);
			socket.receive(serverReply);
			System.out.println("Client received: " + new String(serverReply.getData()));
		}
	}
}
