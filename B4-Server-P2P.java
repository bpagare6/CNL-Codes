import java.io.*;
import java.net.*;
import java.util.*;


class Server
{
	public static void main(String[] args) {
		try
		{
			ServerSocket ss=new ServerSocket(8000);
			System.out.println("\nServer Started...");
			Socket s=ss.accept();
			System.out.println("Connected");

			DataInputStream dis=new DataInputStream(s.getInputStream());
			DataOutputStream dos=new DataOutputStream(s.getOutputStream());

			Scanner sc=new Scanner(System.in);
			String msg="$";
			String res;
			while(!msg.equals("exit"))
			{
				res=dis.readUTF();
				System.out.println("Peer 1 : "+res);
				msg=sc.next();
				dos.writeUTF(msg);
			}
			ss.close();
		}
		catch(Exception e)
		{
			System.out.println(e);
		}
		
		
	}
}