package paczka;

import java.util.ArrayList;
import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
	
public class Alice implements Runnable{
	
	private Channel channel;
	private ArrayList<byte[]> puzzles;
	private int id;
	private SecretKey session_Key;
	private int parameter;
	
	protected Alice(int how_many, Channel ch)
	{
		this.parameter = how_many;
		this.channel = ch;
		this.puzzles = new ArrayList<byte[]>();
	}
	
	private synchronized void get_messages()
	{
		System.out.println("Alice: I'm waiting for the puzzles");
		while(channel.data.isEmpty())
		{
			System.out.print("");
		}
		
		while(true) { if(channel.Access == false) break;}
		channel.Access = true;
		puzzles = channel.data;
		channel.Access = false;
		

	}
	
	
	private synchronized void pick_and_solve() throws Exception //basic now 
	{
		System.out.println("Alice: I'm picking and solving the puzzle");
		
		Random r = new Random();
		int choice = r.nextInt(puzzles.size());
		byte[] tmp = puzzles.get(choice); //change it to aes 128 bit
		
		String plain;
		KeyGenerator AliceKey = KeyGenerator.getInstance("AES");
		SecretKey secret_littlekey;
		Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		byte[] tmp_key;
		byte[] iv = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	    IvParameterSpec ivspec = new IvParameterSpec(iv);

		SecretKeySpec secspec;
		while(true)
	{
			AliceKey.init(128);//16 bytes
			secret_littlekey = AliceKey.generateKey();
			tmp_key = secret_littlekey.getEncoded();
			
			for(int j = 0; j < ((128/8) - parameter/8); j++)
			{
				tmp_key[j] = (byte)0;
			}
			
			secspec = new SecretKeySpec(tmp_key, 0, tmp_key.length, "AES");
			
			try {
			
			cipher.init(Cipher.DECRYPT_MODE, secspec, ivspec);
			
			plain = new String(cipher.doFinal(tmp));
			
			if(plain.contains("One")) //get key and ID from that
			{
				int end;
				
				end = plain.indexOf("One");
				id = Integer.parseInt(plain.substring(0, end));

				
				end = end + 3;
				String thekey = plain.substring(end);
				
				session_Key = new SecretKeySpec(thekey.getBytes(), 0, thekey.getBytes().length, "AES");
				break;
			}
			
			}catch(Exception e)
			{
				continue;
			}
			

		
	}

	}
	
	
	private synchronized void sendID()
	{
		System.out.println("Alice: Now I'm sending its ID");
		channel.Access = true;
		channel.chosen_id = id;
		channel.Access = false;
				
	}
	
	public synchronized void run() 
	{
		try {
		get_messages();
		
		pick_and_solve();
		
		sendID();
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}

}