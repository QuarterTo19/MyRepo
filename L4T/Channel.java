package paczka;

import java.util.ArrayList;

public class Channel {

	//protected ArrayList<String> data;
	protected ArrayList<byte[]> data;
	protected boolean Access = false;
	protected int chosen_id;
	public Channel (int how_many)
	{
		Math.pow((double)2.0, Double.parseDouble(Integer.toString(how_many)));
		this.data = new ArrayList<byte[]>();
		this.chosen_id = 0;
		
	}
	

	
}
