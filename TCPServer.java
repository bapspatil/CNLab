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
		// TODO Auto-generated method stub
		ServerSocket se = new ServerSocket(1534);
		Socket q = se.accept();
		System.out.println("Connection established.");
		BufferedReader v = new BufferedReader(new InputStreamReader(q.getInputStream()));
		DataOutputStream dr = new DataOutputStream(q.getOutputStream());
		String g = v.readLine();
		FileReader f = null;
		BufferedReader ff = null;
		boolean b;
		File r = new File(g);
		if(r.exists())
			b = true;
		else
			b = false;
		if(b)
			dr.writeBytes("Yes\n");
		else
			dr.writeBytes("No\n");
		if(b) {
			f = new FileReader(g);
			ff = new BufferedReader(f);
			String qq;
			while((qq = ff.readLine()) != null)
				dr.writeBytes(qq + "\n");
			dr.close();
			ff.close();
			v.close();
			se.close();
			q.close();
			f.close();
		}
	}

}
