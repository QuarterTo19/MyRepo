#include<iostream>
#include<NTL/ZZ.h>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<ctime>
#include<NTL/vector.h>
#include<vector>
#include<tuple>
#include<omp.h>

using namespace std;
using namespace NTL;

ZZ p(0), ordp(0);
ZZ twoexp(0);
ZZ ex1(0);
long pow1;
int picker = 0;
ZZ fx1(0);
ZZ Q(0);
ZZ g(0), x(0), y(0);

ZZ mod1(0), mod2(0), mod3(0), mod4(0), mod5(0), mod6(0), mod7(0), mod8(0), mod9(0), mod10(0);
ZZ modular(0);
ZZ wynik(0);
int elements = 8;

vector<ZZ> nums; //initial numbers
vector<long> expon; //set of exponents
vector<ZZ> resl; //results of operation


vector<ZZ> eq1; // 2
vector<ZZ> eq2;
vector<ZZ> eq3;
vector<ZZ> eq4;
vector<ZZ> eq5; //Q
vector<ZZ> eq6;
vector<ZZ> eq7;
vector<ZZ> eq8;
vector<ZZ> eq9;
vector<ZZ> eq10;
vector<ZZ> forgcd;
vector<ZZ> sols;
vector<ZZ> user;

vector<tuple<ZZ, ZZ>> forcrt;

int match = 0;

void generatedata()
{

  nums.push_back((ZZ)2);

  pow1 = (long)RandomBnd(10) + 3;
  expon.push_back(pow1);

  power(twoexp, (ZZ)2, pow1);
  resl.push_back(twoexp);

  cout << twoexp <<endl;

  for(int i = 0; i < elements; i++)
  {
    ex1 = GenPrime_ZZ((long)10, (long)9000);
    nums.push_back(ex1);

    pow1 = (long)RandomBnd(3)+5;
    expon.push_back(pow1);

    power(fx1, ex1, pow1);
    resl.push_back(fx1);

    cout << fx1  << endl;
  }
  //cout << ex1 << " ";

  Q = GenPrime_ZZ((long)100, (long)9000);
  nums.push_back(Q);
  resl.push_back(Q);

  eq5.push_back((ZZ)1);
  mod5 = Q;

  cout << Q << endl;
  //cout << Q << " ";

}

void calcdata()
{
  ZZ value(1);

  for(int i = 0; i < resl.size(); i++)
  {
    //value *= resl[i];
    mul(value, value, resl[i]);
  }
  ordp = value;

  p = ordp + (ZZ)1;
  //cout << "p: " << p << "  ord g: " << ordp << endl;
}

void make_equations(vector<ZZ>& into, ZZ& modulus)
{
  //generate equation for 2

  ZZ value(0);

  value = nums[picker];
  for(long  i = 0; i < expon[picker]; i++)
  {
      power(value, nums[picker], i);
      into.push_back(value);
  }
  modulus = resl[picker];

}

void crtsolve3()
{
  vector<ZZ> Minv;
  int counter = 0;
  ZZ q, r, r1, r2, t, t1, t2;
  ZZ total(1);
  for(vector<tuple<ZZ, ZZ>> :: iterator it = forcrt.begin(); it != forcrt.end(); ++it)
  {
    total *= get<1>(*it);
  }

 for(vector<tuple<ZZ, ZZ>> :: iterator it = forcrt.begin(); it != forcrt.end(); ++it)
 {
   r1 = get<1>(*it);
   r2 = total / get<1>(*it);

   t1 = (ZZ)0;
   t2 = (ZZ)1;

   while(r2 > 0)
   {
     q = r1 / r2;
     r = r1 - q * r2;
     r1 = r2;
     r2 = r;

     t = t1 - q * t2;
     t1 = t2;
     t2 = t;
   }
   if(r1 == 1)
   Minv.push_back(t1);


   if(Minv[counter] < (ZZ)0)
   {
     Minv[counter] = Minv[counter] + get<1>(*it);
   }

   counter += 1;

 }

 ZZ sol(0);
 counter = 0;
 for(vector<tuple<ZZ, ZZ>> :: iterator it = forcrt.begin(); it != forcrt.end(); ++it)
 {
   sol += ( (get<0>(*it) * total * Minv[counter]) / get<1>(*it) );
   counter += 1;
 }
 if((sol%total) == (x%(ordp/Q)))
 {
    match = 1;
    wynik = (sol%total);
  }
}

int main()
{
  restart:
  srand(time(NULL));
  cout << "Factorization: " << endl;
  generate:
  generatedata();
  calcdata();

  if(ProbPrime(p, (long)9000) == 0) {nums.clear(); expon.clear(); resl.clear(); system("clear"); goto generate; }
  cout << endl <<  "p =  " << p << "\nordg =  " << ordp << endl;
  //g = g*g;
  compute:

  ZZ test; g = RandomBnd(ordp);
  PowerMod(test, g, ordp, p);

  if(test != 1)
  {
    while(true)
    {
       PowerMod(test, g, ordp, p);
       if(test == (ZZ)1) break;
       else g = RandomBnd(ordp);
    }
  }




  x = RandomBnd(ordp);
  PowerMod(y, g, x, p);

  make_equations(eq1, mod1); //2 is here
  picker += 1;

  make_equations(eq2, mod2);
  picker += 1;

  make_equations(eq3, mod3);
  picker += 1;

  make_equations(eq4, mod4); //for 3 values
  picker += 1;

   make_equations(eq6, mod6); //for 4 values
   picker += 1;
  //
  make_equations(eq7, mod7); //for 5 values
   picker += 1;
  //
   make_equations(eq8, mod8); //for 6 values
   picker += 1;
  //
   make_equations(eq9, mod9); //for 7 values
   picker += 1;
  //
   make_equations(eq10, mod10); //for 8 values
   picker += 1;

  #pragma omp parallel for schedule(static) shared(resl) private(sols, user, modular)
  for(int i = 0; i < elements+1; i++) //for now only 1
  {
      switch(i) //debug time tho
      {
        case 0: user = eq1; modular = mod1; break; //2 is here
        case 1: user = eq2; modular = mod2; break;
        case 2: user = eq3; modular = mod3; break;
        case 3: user = eq4; modular = mod4; break; //for 3 values
        case 4: user = eq6; modular = mod6; break; //for 4 inputs
        case 5: user = eq7; modular = mod7; break; //for 5 inputs
        case 6: user = eq8; modular = mod8; break; //for 6 inputs
        case 7: user = eq9; modular = mod9; break; //for 7 inputs
        case 8: user = eq10; modular = mod10; break; //for 8 inputs

      }
      ZZ y1(0), numto(0), g1(0), solution(1), value(0), savey(0);
      long counter = 1;
      y1 = y; numto = nums[i]; g1 = g;
      savey = y;

      for(int j = 0; j < user.size(); j++)
      {

        PowerMod(y1, savey, (ordp / numto), p);
        PowerMod(g1, g, (ordp / nums[i]), p);

          ZZ test(0);
          for(ZZ z(0); z < nums[i]; z++)
          {
            PowerMod(test, g, (ZZ)z*(ordp / nums[i]), p); //cos tutaj sie wali :P


            if(test == y1){
                #pragma omp critical
                sols.push_back((ZZ)z);
                solution = (ZZ)z;
             }
          }

        PowerMod(g1, g, (-1)*(solution*user[j]), p);
        value += (user[j] * solution);
        MulMod(savey, savey, g1, p);
        counter += 1;
        power(numto, nums[i], counter);
      }
    value = value % modular;
    #pragma omp critical
    forcrt.push_back(make_tuple(value, modular));


  }

  crtsolve3();

  if(match == 1)
  {
    cout << endl << g << "^x = " << y << endl;

    cout << endl << "Set of congruences: " << endl;

    for(vector<tuple<ZZ, ZZ>>::iterator IT = forcrt.begin(); IT != forcrt.end(); ++IT)
    {
          cout << "x = " <<  get<0>(*IT) << " mod " << get<1>(*IT) << endl;
    }

    cout << endl << "result: "<< wynik << "\n(xmod): " << x % (ordp/Q)<< endl;

    cout << "Done." << endl;
  }
  else
  {
  //cleaning
  quit:

  eq1.clear();eq2.clear();eq3.clear();

  eq4.clear();eq6.clear();eq7.clear();

  eq8.clear();eq9.clear(); eq10.clear();

  forcrt.clear();sols.clear();user.clear();

  forgcd.clear();

  picker = 0;
  goto compute;
  }
  return 0;
}
