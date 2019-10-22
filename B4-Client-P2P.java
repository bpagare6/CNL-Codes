import java.io.*;
import java.net.*;
import java.util.*;

class Client
{
	public static void main(String[] args) {
		try
		{
			Socket s=new Socket("localhost",8000);
			System.out.println("Connected");

			DataInputStream dis=new DataInputStream(s.getInputStream());
			DataOutputStream dos=new DataOutputStream(s.getOutputStream());

			Scanner sc=new Scanner(System.in);

			String msg="$";
			String res;
			while(!msg.equals("exit"))
			{
				msg=sc.next();
				dos.writeUTF(msg);
				res=dis.readUTF();
				System.out.println("Peer 2 : "+res);
			}
			s.close();

		}
		catch(Exception e)
		{
			System.out.println(e);
		}
	}
}