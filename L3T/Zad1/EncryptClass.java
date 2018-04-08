package Zad1;

import java.io.Console;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.security.KeyStore;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.spec.AlgorithmParameterSpec;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.crypto.Cipher;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;



public class EncryptClass {

	//odes to be supported
	static String mode = "AES/CBC/PKCS5Padding"; 
	static String mode2 = "AES/OFB/PKCS5Padding";
	static String mode3 = "AES/CTR/PKCS5Padding";
	private String Keypath = "";
	private String password = "";
	private String ivpath = "";
	private String alias = "";
	private Console con = System.console();
	private char[] psw;
	private Scanner readMe = new Scanner(System.in); //for this password stuff :(
	private IvParameterSpec ivspec;
	private String fromuser = "";
	private ArrayList<String> Files = new ArrayList<String>(); //for Files
	private ArrayList<String> messages = new ArrayList<String>();
	private ArrayList<byte[]> ciphers = new ArrayList<byte[]>();
	
	final int key_length = 256; //might not be used at all ?
	byte[] iv;

	
	
	EncryptClass() throws Exception
	{
		File file = new File("config");
		FileInputStream fis = new FileInputStream("config");
		byte[] s = new byte[(int)file.length()];
		fis.read(s);
		fis.close();
		
		Pattern pattern = Pattern.compile(":.*");
		Matcher matcher = pattern.matcher(new String(s));
		int p = 0;
		while(matcher.find())
		{
			if(p == 0)
			password = matcher.group().substring(2);
			if(p == 1)
			ivpath = matcher.group().substring(2);
			if(p == 2)
			Keypath = matcher.group().substring(2);
			if(p == 3)
			alias = matcher.group().substring(2);
			
			p+=1;
		}
		
		iv = new byte[16];
		int seed = 1;
		int x = seed; //just put it simple nao
		for(int i = 0; i < 16; i++)
		{
			x = i+1;
			iv[i] = (byte)(x);
		}
		
	}
	
	
	
	//encrypting data - file
	protected void encryptdata(String how, byte[] IV, byte[] bytes, String toFile) throws Exception
	{
		//we should increase the iv each time, like iv + 1
		for(int i = 0; i < 16; i++)
		{
		++iv[i];
		++iv[i];
		++iv[i];
		
		}
		//initialising Cipher
		Cipher encryption = Cipher.getInstance(how);
				
		KeyStore ks = KeyStore.getInstance("JCEKS");
		ks.load(new FileInputStream(Keypath), password.toCharArray());
		SecretKey key = (SecretKey) ks.getKey(alias, password.toCharArray());
		//'restoring the key'

		
		//Ciphering a File right now
		//encryption.init(Cipher.ENCRYPT_MODE, key, new IvParameterSpec(IV));
		ivspec = new IvParameterSpec(iv);

		encryption.init(Cipher.ENCRYPT_MODE, key, ivspec);
		
		
		byte[] encrypted = encryption.doFinal(bytes);
		
		byte[] giveback = new byte[iv.length + encrypted.length];
		System.arraycopy(iv, 0, giveback, 0, iv.length);
		System.arraycopy(encrypted, 0, giveback, iv.length, encrypted.length);
		
		FileOutputStream fos = new FileOutputStream(toFile);
		fos.write(giveback);
		fos.close();
		
		
	}
	
	//decrypting data - file
	protected void decryptData(String how, byte[] IV, byte[] bytes, String toFile) throws Exception
	{
		byte[] myiv = new byte[16];
		System.arraycopy(bytes, 0, myiv, 0, 16);
	
		byte[] to_decrypt = new byte[bytes.length - iv.length];
		System.arraycopy(bytes, iv.length, to_decrypt, 0, bytes.length - iv.length);
		
		Cipher decryption = Cipher.getInstance(how);
		
		
		KeyStore ks = KeyStore.getInstance("JCEKS");
		ks.load(new FileInputStream(Keypath), password.toCharArray());
		SecretKey key = (SecretKey) ks.getKey(alias, password.toCharArray());
	
		
		//decrypting stuff
		//decryption.init(Cipher.DECRYPT_MODE, key, new IvParameterSpec(IV));
		ivspec = new IvParameterSpec(myiv);

		decryption.init(Cipher.DECRYPT_MODE, key, ivspec);
		
		
		byte[] decrypted = decryption.doFinal(to_decrypt);
		
		FileOutputStream fos = new FileOutputStream(toFile);
		fos.write(decrypted);
		fos.close();
	}
	
	//encrypt a String 
	protected byte[] encryptMessage(String how, String message) throws Exception
	{
		
		//we should increase the iv each time, like iv + 1
		for(int i = 0; i < 16; i++)
		{
		++iv[i];
		++iv[i];
		++iv[i];
		
		}
		byte[] thedata = message.getBytes();
		Cipher cipher = Cipher.getInstance(how);
		
		KeyStore ks = KeyStore.getInstance("JCEKS");
		ks.load(new FileInputStream(Keypath), password.toCharArray());
		SecretKey key = (SecretKey) ks.getKey(alias, password.toCharArray());
		ivspec = new IvParameterSpec(iv);
		
		cipher.init(Cipher.ENCRYPT_MODE, key, ivspec);
		
		byte[] cdata = cipher.doFinal(thedata);
		
		byte[] giveback = new byte[iv.length + cdata.length];
		System.arraycopy(iv, 0, giveback, 0, iv.length);
		System.arraycopy(cdata, 0, giveback, iv.length, cdata.length);
		
		return giveback;
		
		
	}
	
	//decrypt a String
	protected String decryptMessage(String how, byte[] encrypted) throws Exception
	{
		byte[] myiv = new byte[16];
		System.arraycopy(encrypted, 0, myiv, 0, 16);
			
		//take out encrypted
		byte[] to_decrypt = new byte[encrypted.length - iv.length];
		System.arraycopy(encrypted, iv.length, to_decrypt, 0, encrypted.length - iv.length);
		
		
		KeyStore ks = KeyStore.getInstance("JCEKS");
		ks.load(new FileInputStream(Keypath), password.toCharArray());
		SecretKey key = (SecretKey) ks.getKey(alias, password.toCharArray());
		ivspec = new IvParameterSpec(myiv);
		
		Cipher cipher = Cipher.getInstance(how);
		
		cipher.init(Cipher.DECRYPT_MODE, key, ivspec);
		
		byte[] decoded = cipher.doFinal(to_decrypt);
		
		return new String(decoded);
	}
	
	//start encrypting file(s)
	protected void encryptFile(String FileName) throws Exception
	{
		File file = new File(FileName);
		FileInputStream fis = new FileInputStream(FileName);
		byte[] data = new byte[(int)file.length()];
		fis.read(data);
		fis.close();
				
		encryptdata(mode, iv, data, FileName);
	}
	//start decrypting file(s)
	protected void decryptFile(String FileName) throws Exception
	{
		File file2 = new File(FileName);
		FileInputStream fis2 = new FileInputStream(FileName);
		byte[] data2 = new byte[(int)file2.length()];
		fis2.read(data2);
		fis2.close();
		
		decryptData(mode, iv, data2, FileName);
	}
	
	protected byte[] challenge(String m1, String m2) throws Exception
	{
		int coin = Random.class.newInstance().nextInt(0xFFFF);


		

		if(coin % 2 == 0)
		{
			byte[] c1 = null;
			c1 = encryptMessage(mode, m1);
			return c1;
		}
		else if(coin % 2 == 1)
		{		
			byte[] c2 = null;
			c2 = encryptMessage(mode, m2);
			return c2;
		}
		
		return null;
	}
	

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		try {
		EncryptClass ec = new EncryptClass();
		//ec.setconfig();
		byte[] mcipher = null;
		int todo = 0;
		int chosen_mode = 0;
		String FileName  = "";

		
		System.out.println("Modes: 0 - EncF 1 - DecF, 2 - MultF. Enc, 3 - MultF. Dec, 4 - Chal");
		System.out.println("5 - EncM, 6 - DecM, 7 - MultM. Enc, 8 - MultM Dec");
		while(true)
		{
		
		Scanner scanner = new Scanner(System.in);
		System.out.print("What mode: ");
		chosen_mode = scanner.nextInt();
		
		switch(chosen_mode)
		{
		
		case 0: 
			
			
			System.out.println("Type file for encryption:");
			System.out.print("> ");
			FileName = scanner.next();
			ec.encryptFile(FileName);
			
			break;
			
		case 1:
			System.out.println("Type file for decryption:");
			System.out.print("> ");
			FileName = scanner.next();
			ec.decryptFile(FileName);
			break;
			
		case 2:
			System.out.println("Type files, to finish type -1");
			
			while(true)
			{
				System.out.println("> ");
				FileName = scanner.next();
				if(FileName.equals("-1"))
					break;
				else
				ec.Files.add(FileName);
			}
			
			for(String name : ec.Files)
			{
				ec.encryptFile(name);
			}
			
			break;
		case 3: 
			System.out.println("Type files, to finish type -1");
			
			while(true)
			{
				System.out.println("> ");
				FileName = scanner.next();
				if(FileName.equals("-1"))
					break;
				else
				ec.Files.add(FileName);
			}

			for(String name : ec.Files)
			{
				ec.decryptFile(name);
			}
			
			break;
		case 4:
				System.out.println("Message1: ");
				String mi = scanner.next();
				System.out.println("Message2: ");
				String mii = scanner.next();
			
				byte[] chal = ec.challenge(mi, mii);
				System.out.println(new String(chal));
				
			break;
		case 5:
			System.out.println("Give a message");
			String msg = scanner.next();
			mcipher = ec.encryptMessage(ec.mode, msg);
			ec.ciphers.add(mcipher);
			System.out.println(new String(mcipher));
			
			break;
		case 6:
			if(ec.ciphers.size() > 0) {
				System.out.println("which cipher to decode");
				int p = scanner.nextInt();
				if(p >= ec.ciphers.size())
				{
				 System.out.println("decoding previous encrypted message");
				System.out.println(new String(ec.decryptMessage(ec.mode, ec.ciphers.get(ec.ciphers.size()-1))));
				}
				else
				{
					System.out.println("decoding c_" + p + " message");
					System.out.println(new String(ec.decryptMessage(ec.mode, ec.ciphers.get(p))));
				}
			}else
			{
				System.out.println("no encrypted messages");
			}
			break;
		case 7:
			System.out.println("Type messages, -1 to stop");
			while(true)
			{
				String msguser = scanner.next();
				if(msguser.equals("-1"))
					break;
				else
					ec.messages.add(msguser);
			}
			System.out.println("Ciphers: ");
			for(String p : ec.messages)
			{
				mcipher = ec.encryptMessage(ec.mode, p);
				ec.ciphers.add(mcipher);
				
				System.out.println(new String(mcipher));
			}
			break;
		case 8:
			System.out.println("Decrypting");
			for(byte[] p : ec.ciphers)
			{
				System.out.println(new String(ec.decryptMessage(ec.mode, p)));
			}
			break;
		default:
			break;
		}
		
		}
		}catch(Exception e)
		{	
			;

		}
	}

}


