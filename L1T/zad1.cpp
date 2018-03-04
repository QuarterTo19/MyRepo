//Linear congruential generator - prediction
//Autor: Michał Szala, 208750
//Cryptography : L1A1
#include<iostream>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<ctime>
#include<cmath>
using namespace std;



vector<unsigned long int> input;

vector<unsigned long int> data;

vector<unsigned long int> predicted;

unsigned long int predicted_m;
unsigned long int predicted_a;
unsigned long int predicted_c;

//calculate gcd for given set of numbers using euclid's algorithm substr version
void calc_gcd()
{
  unsigned long int min = data[0];

  //we aim to have all elements equal to each other
  while(adjacent_find(data.begin(), data.end(), not_equal_to<int>()) != data.end())
  {
    //find the smallest element in entire set
    for(int i = 0; i < data.size(); i++)
    {
      if(min > data[i])
      {
        min = data[i];
      }

    }
    //substr found element from everything else in the set
    for(int j = 0; j < data.size(); j++)
    {
      if(data[j] == min) continue;
      else
      {
        data[j] = data[j] - min;
      }

    }
  }
  predicted_m = data[0];
}

//simple linear congruential generator
//x = (a*x' + c) % m
//x' is a previous x
unsigned long int lcg(unsigned long int seed)
{
   unsigned long int a = 2;
   unsigned long int c = 5;
   unsigned long int m = 1011;

   seed = (seed*a + c) % m;

   return seed;
}

//for calculating m in lcg
long int calculate2(int i)
{

  long int t1 = input[i+1] - input[i];
  long int t2 = input[i+2] - input[i+1];
  long int t3 = input[i+3] - input[i+2];

  long int res = (t3*t1) - (t2*t2);

  res = abs(res);
  return res;
}

//predicting linear congruental generator
void check_prediction()
{
  //predicting lets start from 10th value
  unsigned long int start = input[10];
  for(int i = 0; i < input.size()-11; i++)
  {
    start = (predicted_a * start + predicted_c) % predicted_m;
    predicted.push_back(start);
  }

  //comparing
  unsigned long int correct = 0;
  for(int i = 11; i < input.size(); i++)
  {

    if(input[i] == predicted[i-11])
    {
      correct += 1;
    }

  }
  double correctness = (double)correct / (double)(predicted.size());
  cout <<"predicted with probability of: "<< correctness << endl;


}
//function for predicting m value
void predict_m()
{
  //take first 10 values
  for(int i = 0; i <= 10; i++)
  {
      long int result = calculate2(i);
      if(result != 0)
      {
        data.push_back(result);
      }
  }
    calc_gcd();
    cout << "predicted m is: "<<predicted_m << endl;

}

//function for predictin a value
void predict_a()
{
  predicted_a = (input[2]-input[1]) / (input[1]-input[0]);
  cout << "predicted a is: " << predicted_a << endl;

}

//function for predicting c value
void predict_c()
{
  predicted_c = input[1] - ((input[2]-input[1])*input[0])/(input[1]-input[0]);
  cout << "predicted c is: "<< predicted_c  << endl;
}


int main()
{

  unsigned long int init = 3;

  //generate input
  for(int i = 0; i < 100; i++)
  {
      init = lcg(init);
      input.push_back(init);
  }

    predict_m();
    predict_a();
    predict_c();

    check_prediction();



  return 0;
}
