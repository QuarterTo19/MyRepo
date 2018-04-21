package paczka;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
	
public class Bob implements Runnable{
	
	//24, 32, 40
	private ArrayList<byte[]> plains;	
	private ArrayList<byte[]> puzzles_Bob;
	
	private String puzzle_mess = "One";
	private SecureRandom sr;
	private int ID;
	private Channel channel;
	private int ID_com;
	private SecretKey session_Key;
	private double iterations;
	private int parameter;
	
	protected Bob(int how_many, Channel ch)
	{
		this.parameter = how_many;
		this.iterations = Math.pow((double)2.0, Double.parseDouble(Integer.toString(how_many)));
		
		this.plains = new ArrayList<byte[]>(); 
		
		this.puzzles_Bob = new ArrayList<byte[]>();
		
		this.sr = new SecureRandom(); //let system use prng
		this.ID = this.sr.nextInt(0xFFF); //generate first ID
		
		this.channel = ch;
		this.ID_com = 0;
		
	}
	
	private synchronized void gen_messages() throws NoSuchAlgorithmException, IOException
	{
		System.out.println("Bob: Generating puzzles");
		
		KeyGenerator keygen = KeyGenerator.getInstance("AES");
		SecretKey secretKey;

		
		for(double i = 0; i < iterations; i++)
		{
			//concatenate ID
			puzzle_mess = (ID+"") + puzzle_mess;
			
			//generate key
			
			keygen.init(256);
			secretKey = keygen.generateKey();
			
			//concatenate key
			puzzle_mess = puzzle_mess.concat(secretKey.toString());
			
			//save message 
			plains.add(puzzle_mess.getBytes());

			
			puzzle_mess = "One";
			ID = sr.nextInt(0xFFF);
		}
	}
	
	private synchronized void gen_ciphs() throws Exception
	{
		System.out.println("Bob: Encrypting puzzles");
		
		//generate little key encrypt every puzzle
		KeyGenerator littlekey = KeyGenerator.getInstance("AES");
		SecretKey secret_littlekey;
		Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		byte[] iv = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	    IvParameterSpec ivspec = new IvParameterSpec(iv);
		//IvParameterSpec zeroIv = new IvParameterSpec(new byte[16]);
		byte[] tmp;

		for(double i = 0; i < iterations; i++)
		{
			littlekey.init(128);//16 bytes
			//
			secret_littlekey = littlekey.generateKey();
			tmp = secret_littlekey.getEncoded();
			for(int j = 0; j < ((128/8) - parameter/8); j++)
			{
				tmp[j] = (byte)0;
			}
			secret_littlekey = new SecretKeySpec(tmp, 0, tmp.length, "AES");
			//System.out.println(Arrays.toString(secret_littlekey.getEncoded()));
			cipher.init(Cipher.ENCRYPT_MODE, secret_littlekey, ivspec);
			puzzles_Bob.add(cipher.doFinal(plains.get((int)i)));
		}

	}
	
	//send messages
	private synchronized void send_messages()
	{
		System.out.println("Bob: I'm sending puzzles");
		channel.Access = true;
		channel.data = this.puzzles_Bob;
		channel.Access = false;
	}
	
	
	private synchronized void receiveID()
	{
		System.out.println("Bob: I'm waiting for ID from Alice");
		while(channel.chosen_id == 0)
		{
			System.out.print("");
		}
		while(true) { if(channel.Access == false) break;}
		channel.Access = true;
		ID_com = channel.chosen_id;
		channel.Access = false;
		
		
	}
	
	private synchronized void pick_the_key()
	{
		String alices_choice;
		for(double i = 0; i < iterations; i++)
		{
			alices_choice = new String(plains.get((int)i));
			if(alices_choice.contains(ID_com+""))
			{
				int beg;
				
				beg = alices_choice.indexOf("One");
				beg = beg + 3;
				
				String thekey = alices_choice.substring(beg);

				session_Key = new SecretKeySpec(thekey.getBytes(), 0, thekey.getBytes().length, "AES");
				break;
			}
		}
		System.out.println("Bob: Session key established");
		
	}
	
	public synchronized void run() 
	{
		try {
			
			gen_messages();
			
			gen_ciphs();
			
			send_messages();
			
			receiveID();
			
			pick_the_key();
			
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	
}
