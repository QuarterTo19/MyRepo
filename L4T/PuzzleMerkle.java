package paczka;

public class PuzzleMerkle{

	//check classes for description
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int size = 16; //for 24 later
		Channel c = new Channel(size);
		
		new Thread(new Bob(size, c)).start();
		new Thread(new Alice(size, c)).start();		
		
		
	}

}
