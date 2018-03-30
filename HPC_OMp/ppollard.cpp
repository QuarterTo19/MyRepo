//Parallel pollard ro executio - naive
//Autor: Michał Szala, 208750
#include<omp.h>
#include<NTL/ZZ.h>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<tuple>

using namespace NTL;
using namespace std;

//variables - main
ZZ alpha, beta, n, N;
vector< tuple<ZZ, ZZ, ZZ> > start_point; //starting point
vector< tuple<ZZ, ZZ, ZZ, ZZ, ZZ, ZZ> > both_points; //both common points
vector< tuple<ZZ, ZZ, ZZ> > logs; //another one -> for detecting cycles

int tid, nthreads;
int repeatedt = 0, repeatedr = 0;
int restart = 0;
bool towrite = false;
int was = 0;
int counted = 0;

//calculating gcd
ZZ gcde(ZZ p, ZZ b, ZZ *x, ZZ *y);

//calculating reverse in modulo
ZZ modInverse(ZZ p, ZZ m)
{
  ZZ x, y;
  ZZ g = gcde(p, m, &x, &y);
  if(g != (ZZ)1)
  {
    cout << "Doesn't exist" << endl;
    return (ZZ)-1;
  }
  else
  {
    ZZ fin;
    //fin = (x%m + m) % m;
    fin = AddMod(x%m, m, m);
    return fin;
  }
}

ZZ gcde(ZZ p, ZZ b, ZZ *x, ZZ *y)
{
  if(p == (ZZ)0)
  {
    *x = (ZZ)0; *y = (ZZ)1;
    return b;
  }

  ZZ x1, y1;
  ZZ gcd;

  gcd = gcde(b%p, p, &x1, &y1);

  ZZ test1;

  div(test1, b, p);

  mul(test1, x1, test1);

  sub(*x, y1, test1);

  *y = x1;

  return gcd;
}

//our turtle
void calculate1(ZZ& x, ZZ& a, ZZ& b)
{
  switch(x%3)
  {
    case 1: x = MulMod(x, beta, N); b = AddMod(b, (ZZ)1, n); break;
    case 0: x = SqrMod(x, N); a = MulMod(a, (ZZ)2, n); b = MulMod(b, (ZZ)2, n); break;
    case 2: x = MulMod(x, alpha, N); a = AddMod(a, (ZZ)1, n); break;
  }

}

int main()
{
  //so its look like
  //1. generate equation - done ?
  //2. generate random a, b, x looks like be what ever - done
  //3. each processor saves starting point - got that
  //4. then they go -> fixed that on test program
  //----
  //5. when one finds a point it saves it to the list - got the idea for the list
  //6. when other finds a point it checks whether it was already added
  //7. if yes then calculate result (?)
  //8. if no add it to the list
  //9. we need to save starting points

  //this part for all

  ZZ x, a, b;
  ZZ X, A, B;
  ZZ rev;
  ZZ result;
  ZZ test;
  ZZ was;
  long bitlength = 20; //bit generated
  long astart = 20; // random
  long bstart = 200; // random


  n = GenGermainPrime_ZZ(bitlength, 80);  //
  long ntest = ProbPrime(n, 80);
  mul(N, n, 2);
  add(N, N, 1);
  long Ntest = ProbPrime(N, 80);

  while(Ntest != 1 || ntest != 1)
  {
    n = GenGermainPrime_ZZ(bitlength, 80);
    ntest = ProbPrime(n, 80);
    mul(N, n, 2);
    add(N, N, 1);
    Ntest = ProbPrime(N, 80);
  }


  alpha = RandomBnd(astart);
  alpha = MulMod(alpha, alpha, N);
  beta = RandomBnd(n-(ZZ)2) + (ZZ)1;
  beta = PowerMod(alpha, beta, N);

while(alpha == 0 || beta == 0 || alpha == 1)
{
  alpha = RandomBnd(astart);
  alpha = MulMod(alpha, alpha, N);
  beta = RandomBnd(n-(ZZ)2) + (ZZ)1;
  beta = PowerMod(alpha, beta, N);
}
     // x = 1; a = 0; b = 0;
     // X = x; A = a; B = b;

    cout << "alpha: " << alpha << " beta: " << beta << " N: " << N << " n: " << n << endl;

    int i = 0;
    #pragma omp parallel for schedule(static) private(i, tid, rev, result, test, X, A, B, x, a, b, was, logs, repeatedr, repeatedt, restart) shared(alpha, beta, N, n, start_point, towrite, both_points, counted)
    for( i = 0; i < 2; i++)
    {
      //each thread add its starting x, a, b
      //x = RandomBnd(N-(ZZ)2)+(ZZ)1;

      a = RandomBnd(n);
      b = RandomBnd(n);
      a = PowerMod(alpha, a, N);
      b = PowerMod(beta, b, N);
      x = MulMod(a, b, N);
      X = x; A = a; B = b;



      #pragma omp critical
      start_point.push_back(make_tuple(x, a, b)); //put starting points


      while(true)
      {
        //turtle - move
        calculate1(x, a, b);
        logs.push_back(make_tuple(x, a, b));
        for(vector< tuple<ZZ, ZZ, ZZ> >::iterator it = logs.begin(); it != logs.end(); ++it)
        {
          if(x == get<0>(*it) && a == get<1>(*it) && b == get<2>(*it))
            {
               //we got the starting point of other thread  init to new ones
               repeatedt += 1;
               if(repeatedt == 10)
               {
                 restart = 1;
                 break;
               }
            }
        }
        //check for other starting points
        //wait here ?!
        #pragma omp critical
        both_points.push_back(make_tuple(x, a, b, X, A, B));
        //for(long long i = 0; i < both_points.size(); i++)
        for(vector< tuple<ZZ, ZZ, ZZ, ZZ, ZZ, ZZ> >::iterator it = both_points.begin(); it != both_points.end(); ++it)
        {
          // if(x == get<0>(both_points[i]) && a == get<1>(both_points[i]) && b == get<2>(both_points[i]) && X == get<3>(both_points[i]) && A == get<4>(both_points[i]) && B == get<5>(both_points[i]))
          if(x == get<0>(*it))
            {
              X = get<3>(*it);
              A = get<4>(*it);
              B = get<5>(*it);
              counted = 2;
              goto count;
            }
        }

        //for(long long i = 0; i < start_point.size(); i++)
        for(vector< tuple<ZZ, ZZ, ZZ> >::iterator it = start_point.begin(); it != start_point.end(); ++it)
        {
          if(x == get<0>(*it) && a == get<1>(*it) && b == get<2>(*it))
            {
              // #pragma omp critical
              // start_point.push_back(make_tuple(x, a, b)); //put starting points
                restart = 1;
                break;
            }
        }

        if(restart == 1)
        {
          //logs.clear(); //clear useless data
          //x = RandomBnd(N-(ZZ)2)+(ZZ)1;
          a = RandomBnd(n);
          b = RandomBnd(n);
          a = PowerMod(alpha, a, N);
          b = PowerMod(beta, b, N);
          x = MulMod(a, b, N);
          X = x; A = a; B = b;

          #pragma omp critical
          start_point.push_back(make_tuple(x, a, b)); //put starting points

          counted = 0;
          restart = 0;
          repeatedt = 0;
          repeatedr = 0;
          continue;
        }

        //rabbit - first move
        calculate1(X, A, B);
        calculate1(X, A, B);
        logs.push_back(make_tuple(X, A, B));
        for(vector< tuple<ZZ, ZZ, ZZ> >::iterator it = logs.begin(); it != logs.end(); ++it)
        {
          if(X == get<0>(*it) && A == get<1>(*it) && B == get<2>(*it))
            {
               //we got the starting point of other thread  init to new ones
               repeatedr += 1;
               if(repeatedr == 10)
               {
                 restart = 1;
                 break;
               }
            }
        }
          //check for other starting points
          #pragma omp critical //jakos tak ?
          both_points.push_back(make_tuple(x, a, b, X, A, B));
          //for(long long i = 0; i < both_points.size(); i++)
          for(vector< tuple<ZZ, ZZ, ZZ, ZZ, ZZ, ZZ> >::iterator it = both_points.begin(); it != both_points.end(); ++it)
          {
            // if(x == get<0>(both_points[i]) && a == get<1>(both_points[i]) && b == get<2>(both_points[i]) && X == get<3>(both_points[i]) && A == get<4>(both_points[i]) && B == get<5>(both_points[i]))
            if(X == get<3>(*it))
             {
               x = get<0>(*it);
               a = get<1>(*it);
               b = get<2>(*it);

                counted = 1;
                goto count;
              }
          }

        //for(long long i = 0; i < start_point.size(); i++)
        for(vector< tuple<ZZ, ZZ, ZZ> >::iterator it = start_point.begin(); it != start_point.end(); ++it)
        {
          if(X == get<0>(*it) && A == get<1>(*it) && B == get<2>(*it))
            {
               //we got the starting point of other thread  init to new ones
               // #pragma omp critical
               // start_point.push_back(make_tuple(x, a, b)); //put starting points
               restart = 1;
                break;
            }
        }

        if(restart == 1)
        {
          //logs.clear(); //nothing
          //x = RandomBnd(N-(ZZ)2)+(ZZ)1;
          a = RandomBnd(n);
          b = RandomBnd(n);
          a = PowerMod(alpha, a, N);
          b = PowerMod(beta, b, N);
          x = MulMod(a, b, N);
          X = x; A = a; B = b;

          #pragma omp critical
          start_point.push_back(make_tuple(x, a, b)); //put starting points

          counted = 0;
          restart = 0;
          repeatedt = 0;
          repeatedr = 0;
          continue;
        }

        if(x == X)
        {
          count:
           if((b - B) == 0)
           {
            // cout << "Failure b - B = 0" << endl;
             //return (ZZ)-2;
             counted = 0;
             continue;

           }else
           {
             //rev = (b - B) % n;
             rev = SubMod(b, B, n);
             rev = modInverse(rev, n);

           if(rev == -1)
           {
               //cout << "Not invertible, aborting(for now)" << endl;
               counted = 0;
               continue;
           }
           else
           {
             //ZZ result;
             //result = (rev * (A - a)) % n;
             result = MulMod(rev, (A - a), n);
             //ZZ test;
             test = PowerMod(alpha, result, N);
             if(test == beta)
             {
               if(counted == 1 || counted == 2)
               cout << "met ";
             cout << "done" << endl;
             cout << i << ": "<< result << endl;
              break;
            }
            else
            {
              counted = 0;
              continue;
            }
        }
      }
    }
  }
}
  return 0;
}
