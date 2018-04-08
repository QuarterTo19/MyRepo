package Zad1;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;

public class Distinguish {

//crypto task 2
//ok so it's looka like that
	
	private String chal = "";
	private ArrayList<byte[]> ivs = new ArrayList<byte[]>();
	
	String m1 = "";
	String m2 = "";
	byte[] thepredicted = null;
	byte[] thepredict = null;
	byte[] make_save = null;
	boolean recognized = false;
	//get IV
	private void getIV(byte[] data)
	{
		
		byte[] tmp = new byte[16];
		System.arraycopy(data, 0, tmp, 0, 16);
		ivs.add(tmp);
		
	}
	
	private void IVanalysis()
	{
		if(ivs.size() > 1)
		{
			for(byte[] p : ivs)
			{
				for(int i = 0; i < p.length; i++)
					System.out.print(p[i] + "\t");
				System.out.println();
			}
			//System.out.println(correct);
			System.out.println();
			}
	}
	
	private void prepareChallenge()
	{
		byte[] last = ivs.get(ivs.size()-1);

		byte[] tmp = ivs.get(ivs.size()-1); // we 'predict new IV
		byte p, q;
		byte a, b;
		//kinda do the same for m1
		m1 = "testtesttesttest";
		byte[] msg = m1.getBytes();
		for(int i = 0; i < 16; i++)
		{
			p = last[i];
			q = tmp[i];
			++q;
			++q;
			++q;
			msg[i] = (byte)((int)p ^ (int)q ^ (int)msg[i]);
		}
		m1 = new String(msg);
		
		
		
		//generating m2
		m2 = "TestTestTestTest";
		byte[] msg2 = m2.getBytes();
		for(int i = 0; i < 16; i++)
		{
			p = last[i];
			q = tmp[i];
			++q;
			++q;
			++q;
			msg[i] = (byte)((int)p ^ (int)q ^ (int)msg2[i]);
		}
		m2 = new String(msg2);
		
		//now predicted IV
		
		//now let's just guess
	}
	
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//send m1 m2, get challenge be alble to tell which message it is with prob 1
		try {
			
			EncryptClass ec = new EncryptClass();
			Distinguish dist = new Distinguish();
			
			for(int i = 0; i < 10; i++) {
			byte[] data1 = ec.encryptMessage(ec.mode, "testtesttesttest");
			dist.getIV(data1);
				System.out.println(new String(data1));
			
			dist.make_save = data1; //saving last message
			}

			
			dist.prepareChallenge();
			byte[] cipher = ec.challenge(dist.m1, dist.m2);
			
			System.out.println();
			System.out.println(new String(cipher));
			
			
			//compare things
			for(int i = 16; i < cipher.length; i++)
			{
				if(cipher[i] == dist.make_save[i]) {
					System.out.println("Got old");
					dist.recognized = true;
					break;
				}
			}
			if(dist.recognized == false)
			{
				System.out.println("Got sth else");
			}
			
		//System.out.println(dt.chal);
		
		}catch(Exception e)
		{
			e.printStackTrace();
		}
		
	}

}