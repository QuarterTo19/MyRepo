//Sender getting bits
#include<iostream>
#include<string.h>
#include<cstdlib>
#include<algorithm>
using namespace std;

//message
string mess = "Hello from the otter side!";
string repr = "";
string purebits = "";
//convert to binary - from 0 to 7
void makebits(int val)
{
  while(val > 0)
  {
    if(val%2 == 0)
    {
      repr += "0";
      val /= 2;
    }
    else if(val%2 == 1)
    {
      repr += "1";
      val /= 2;
    }
  }

  for(int i = repr.length(); i < 8; i++)
  {
    repr += "0";
  }
  reverse(repr.begin(), repr.end()); //dunno if needed

  //we are gonna send reversed bits
  //  cout << repr << " ";
  purebits += repr;
  repr = "";
}
//extract each letter
void readChars()
{
  int id;
  for(int i = 0; i < mess.length(); i++)
  {
    id = mess[i];
     //cout << mess[i] << " " << id << " ";
    makebits(id);
  }
}

int main()
{
  readChars();
  cout << purebits << endl;
  return 0;
}
