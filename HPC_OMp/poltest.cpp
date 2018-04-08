//Parallel pollard ro execution - expected
//Autor: Michał Szala, 208750
#include<omp.h>
#include<NTL/ZZ.h>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<cmath>
#include<tuple>
#include <map>
using namespace NTL;
using namespace std;

vector< tuple<ZZ, ZZ, ZZ> > dist; //distinguished points
ZZ alpha, beta, n, N;
int gotit = 0;
int tid, nthreads;
int alarm = 0;
bool working = false;


ZZ rev;
ZZ result;
ZZ test;
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
  ZZ x, a, b;
  ZZ X, A, B;
  ZZ rev;
  ZZ result;
  ZZ test;




  long bitlength = 40; //bit generated
  long astart = 20; // random
  long bstart = 200; // random
  ZZ towhere(10);

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

    cout << "alpha: " << alpha << " beta: " << beta << " N: " << N << " n: " << n << endl;
int i = 0;



ZZ expec;
ZZ j;
ZZ pi(3);
ZZ mulres;
ZZ divres;
ZZ halfn;
ZZ it(1);
int counter = 0;
long bit1, bit2;
ZZ con;
power(con, alpha, 20);
  int m = 3;
  NTL::ZZ p = NTL::SqrRoot(4 * n / (2 * m));
  NTL::ZZ result1 = (NTL::SqrRoot(p) * NTL::power_long(m, 2)) / m; // TODO check pdf on teacher website
  unsigned int end;
  NTL::conv(end, result1);

  #pragma omp parallel for schedule(static) private(x, a, b, X, A, B, rev, result, test, it) shared(alpha, beta, N, n, dist, alarm)
  for(int i = 0; i < m; i++)
  {
    begin:
    a = RandomBnd(n);
    b = RandomBnd(n);
    a = PowerMod(alpha, a, N);
    b = PowerMod(beta, b, N);
    x = MulMod(a, b, N);

    for(it = (ZZ)1; it < con; it++)
    {
        if(alarm == 1) goto counted;

        #pragma omp critical
        calculate1(x, a, b);


         if(x % 0xff  == 0)
        {

         #pragma omp critical
         dist.push_back(make_tuple(x, a, b));

          #pragma omp critical
          for(vector< tuple<ZZ, ZZ, ZZ> >::iterator it = dist.begin(); it != dist.end(); ++it)
          {
            // if(alarm == 1) goto counted;
            if(alarm == 1) break;
            //if(x == get<0>(*it)  && a != get<1>(*it) && b != get<2>(*it))
            //{

               X = get<0>(*it);
               A = get<1>(*it);
               B = get<2>(*it);
        //       if(x == X)
        //       {
                    if(b - B == 0)
                    {
                      //cout << "Failure b - B = 0" << endl;
                      //return (ZZ)-2;
                      //return ;
                      continue;

                    }else
                    {
                      //rev = (b - B) % n;
                      rev = SubMod(b, B, n);
                      rev = modInverse(rev, n);
                    if(rev == -1)
                    {
                        //cout << "Not invertible, aborting(for now)" << endl;
                        //return ;
                        continue;

                    }
                    else
                    {
                        //ZZ result;
                        //result = (rev * (A - a)) % n;
                        result = MulMod(rev, (A - a), n);
                        //ZZ test;
                        test = PowerMod(alpha, result, N);
                      // cout << omp_get_thread_num()  <<" got" << ": "<< result << endl;

                        if(test == beta)
                        {
                          alarm = 1;
                          cout  << omp_get_thread_num() << " done" << ": "<< result << endl;

                          //goto counted;
                          break;
                     }
                     else
                     {
                       // goto begin;
                       continue;
                     }
                }
               }
              // }
          //}
        }
    }
    }

//      if(alarm == 0) goto begin;

      counted:
        #pragma omp critical
        cout << omp_get_thread_num() << ": Done" << endl;
  }

return 0;
}
