package bapspatil;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Scanner;

public class UDPServer {

	public static void main(String[] args) throws Exception {
		Scanner sc = new Scanner(System.in);
		DatagramSocket skt = new DatagramSocket(6794);
		byte[] buffer = new byte[1000];
		System.out.println("Enter server message");
		while(true) {
			DatagramPacket request = new DatagramPacket(buffer, buffer.length);
			skt.receive(request);
			String message = sc.nextLine();
			byte[] sendMsg = message.getBytes();
			DatagramPacket reply = new DatagramPacket(sendMsg, sendMsg.length, request.getAddress(), request.getPort());
			skt.send(reply);
		}
	}
}
