//Micha³ Szala, 208750
//Cryptography L2/A1
//Decoding ciphered messages.
//Im holding ciphers somewhere else
package Assignment;


import java.io.File;
import java.io.FileInputStream;
import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.xml.bind.DatatypeConverter;

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Hex;

public class Zadanie {
		
	private int sym;
	private int starto = 7;
	
	private String cribres = "";
	private String plaint = "";
	private String c3 = ""; //xor'ed messages land here
	private String c4 = ""; //message converted to hex string
	
	private String pos_key = "";
	
	ArrayList<String> mylist = new ArrayList<String>();

	void ReadCiphs()
	{
		try {
			File f = new File("Crypto-ciphers.txt");
			
			FileInputStream fis = new FileInputStream("Crypto-ciphers.txt");
			byte[] s = new byte[(int) f.length()];
			
			fis.read(s);
			
			fis.close();
			String file_text = new String(s);
			
			//System.out.println(file_text);
			
			//extracting ciphers
			
			Pattern mypattern = Pattern.compile("[0-1 ]+");
			Matcher m = mypattern.matcher(file_text);
			while(m.find())
			{
				String mess = m.group();
				if(mess.length() < 3)
				 ; //omijamy
				else
				mylist.add(mess);
					
			}

			
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	
	//xor c1 c2 here
	void toXor(String x1, String x2)
	{
		int p;
		if(x1.length() < x2.length())
			p = x1.length();
		else
			p = x2.length();
		
		for(int i = 0; i < p; i++)
		{
			if(x1.charAt(i) == ' ') c3 += " ";
			if(x1.charAt(i) == '0' && x2.charAt(i) == '0')
			{
				c3 += "0";
			}
			else if(x1.charAt(i) == '1' && x2.charAt(i) == '1')
			{
				c3 += "0";
			}
			else if(x1.charAt(i) == '0' && x2.charAt(i) == '1')
			{
				c3 += "1";
			}
			else if(x1.charAt(i) == '1' && x2.charAt(i) == '0')
			{
				c3 += "1";
			}
		}
	}	
	
	void toHex(String in) throws UnsupportedEncodingException
	{
		for(int i = 0; i < in.length(); i++)
		{
			if(in.charAt(i) == ' ') {starto = 7; c4 += Integer.toHexString(sym); sym = 0; ; continue;}
			else if(in.charAt(i) == '1')
			{
				sym += Math.pow(2, starto);
				starto -=1;
				
			}
			else if(in.charAt(i) == '0')
			{
				starto -= 1;
			}
		}
		//c4 = c4.trim();
		
		//System.out.println("\nHex code: " + c4);
	
	}
	
	void tocrib(String crib) //cribbin' time
	{
		char[] ar = Hex.encodeHex(crib.getBytes());
		String ars = new String(ar); 
		
		BigInteger big1 = new BigInteger(ars, 16);
		
		
		for(int i = 0; i < (c4.length() - ars.length()); i++)
		{
			//if(i % 10 == 0) System.out.println();
		 BigInteger big2 = new BigInteger(c4.substring(i, ars.length()+i), 16);
		 
		
		 BigInteger res = big1.xor(big2);
		 
		 
		 String show = res.toString(16);
		 
		 try {
			 
		 byte[] s = Hex.decodeHex(show);
		 if(i == 0)
		 cribres = new String(s);
			
		 }catch(DecoderException e)
		 {
			 continue;
		 }
		}
	}
	
	
	void KeyRestore(String phrase) // if one time pad was used
	{
		//convert to 
		//c12 is the beginning of it
		c3 = ""; c4 = "";
		try {
			toHex(mylist.get(5)); //c2 e1
			String temp = c4; // ?
			char ar[] = Hex.encodeHex(phrase.getBytes()); //" JULIET" now we have to generate each possible char
			String ars = new String(ar);
			
			BigInteger big1 = new BigInteger(ars, 16);
			
			BigInteger big2 = new BigInteger(temp.substring(0, ars.length()), 16);
				
			BigInteger res = big1.xor(big2);
				 
			String show = res.toString(16);
			
			
				 
			try {
					 
				 byte[] s = Hex.decodeHex(show);
				 //System.out.println(": " + new String(s) + " ");
				 pos_key += new String(s);
						
				}catch(DecoderException e)
				 {
					 e.printStackTrace();
				 }
						
		}catch(UnsupportedEncodingException e)
		{
			e.printStackTrace();
		}
	}
	
	void Restore_Mess(String inciph)
	{
	
				c3 = "";c4 = "";
				try {
					toHex(inciph); //in c4
					char ar[] = Hex.encodeHex(pos_key.getBytes());
					String ars = new String(ar);
					
					BigInteger big1 = new BigInteger(ars, 16);
					
					
					BigInteger big2 = new BigInteger(c4.substring(0, ars.length()), 16);
					
					BigInteger res = big1.xor(big2);
					 
					String show = res.toString(16);

					
					byte[] s = Hex.decodeHex(show);
					plaint = new String(s);
					
				}catch(Exception e) {
					
					e.printStackTrace();
				}
					
	}			
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Zadanie z = new Zadanie();
		try {

			z.ReadCiphs();
			for(int i = 0; i < z.mylist.size(); i++)
			{
				//
			z.toXor(z.mylist.get(5), z.mylist.get(i)); //got it
			z.toHex(z.c3);
			z.tocrib(" JULIET Three words"); // got sth
			z.c3 = "";z.c4 = "";
			System.out.println("Crib" + ": " + z.cribres); //gives: past comp -> which extends to past compare -> find it, got it
			}
	

		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}

}
//e2 ROMEO Al - some scraps
//Seems like Romeo and Juliet			
//String keyBegin = " JULI?";
//String saver = keyBegin;
//
//z.KeyRestore(keyBegin);			
//z.Restore_Mess(z.mylist.get(19)); 
//System.out.println("Ciph" + ": " + z.plaint); //unreadable
//z.pos_key = ""; z.c3 = "";z.c4 = "";
//
//
//
//for(int i = 0; i < 256; i++) //get any ascii, iterate throu it :v
//{
//	char c = (char)i;
//	keyBegin += Character.toString(c);
//	z.KeyRestore(keyBegin);
//	z.Restore_Mess(z.mylist.get(19));
//	z.pos_key = ""; z.c3 = "";z.c4 = "";
//	if(z.plaint.equals("Second "))
//		break;
//	else
//	keyBegin = saver;
//}
//
//System.out.println(keyBegin);
//System.out.println(z.plaint);