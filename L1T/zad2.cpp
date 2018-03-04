//rand() prediction
//autor: Michał Szala, 208750
//Cryptography Labs - L1A2


#include<iostream>
#include<cstdlib>
#include<ctime>
#include<vector>

using namespace std;

vector<unsigned long int> input;
vector<unsigned long int> predicted;

//predicting rand()
void predict_rand(unsigned long int start)
{
    unsigned long int i31 = input[start - 31];
    unsigned long int i3 = input[start - 3];

     unsigned long int prediction = (i31 + i3) % (1u << 31); //gives probability of guess at rate of ~ 0.75

     predicted.push_back(prediction);
}

int main()
{

  unsigned long int correct = 0; //correct guesses
  unsigned long int how_many = 0; //how_many inputs
  double result;

  how_many = 15100;  //so far simply hardcoded

  srand(time(NULL)); //random seed each execution

  for(int i = 0; i < how_many; i++)
  {
    input.push_back(rand()); //generating data

    if(i >= 31) //that's our beginning
    {
      predict_rand(i);
      if(input[i] == predicted[i - 31]) //if prediction is right
      {
        correct += 1;
      }
    }
  }

  result = (double)correct / (double)predicted.size(); //calculating result

  cout << result << endl;

 return 0;
}
