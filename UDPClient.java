package bapspatil;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPClient {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		DatagramSocket skt = new DatagramSocket();
		
		
		String msg = "text message";
		byte[] b = msg.getBytes();
		InetAddress host = InetAddress.getByName("127.0.0.1");
		int serverSocket = 6794;
		DatagramPacket request = new DatagramPacket(b, b.length, host, serverSocket);
		skt.send(request);
		byte[] buffer = new byte[1000];
		DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
		skt.receive(reply);
		System.out.println("Client received: " + new String(reply.getData()));
		skt.close();
	}
}
