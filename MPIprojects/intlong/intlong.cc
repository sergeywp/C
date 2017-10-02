#include <iostream>
#include "/home/sergey/programms/gmp-6.0.0/gmpxx.h"
using namespace std;
int main(int argc, char** argv)
{
	mpz_t a,b,c;
	mpz_inits(a,NULL);
	mpz_inits(b,c,NULL);
	mpz_set_str(a, "12321343144512134231212134231143112432524",10);
	mpz_set_str(b, "-123456789009866741207609760",10);
	mpz_add(c,a,b);
	cout<<"\nsumm: ";
	mpz_out_str(stdout, 10, c);
	cout<<endl;
	cin.get();
	return 0;
}
