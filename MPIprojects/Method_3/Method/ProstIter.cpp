#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream.h>

using namespace std;

//x*x-13*x+36
//x=(x*x+36)/13

double fun1(double x)
{
	return x*x-13*x+36;
}
double fun(double x)
{
	return (x*x+36)/13;
}
double PrIter(double x)
{
	double eps = 0.0000000000001;
	int i=0;
	double pred, next, prov;
	pred = x;
	do
	{
		//next = fun(pred);
		next = (pred*pred+36)/13;
		prov=next-pred;
		//printf("%e\n",prov);
		cout<<prov<<endl;
		//if (next-pred<eps)break;
		pred=next;
		i++;
	}
	while(i<100);
	return next;
}

int main(int argc, char** argv)
{
	double xf;
	//xf = (double*)malloc(sizeof(double));
	double f = 1;
	//printf("%e\n",&f);
	//printf("ligljhl\n");
	cout<<f<<endl;
	//f = (double*)malloc(sizeof(double));
	double x1;
	//x1 = (double*)malloc(sizeof(double));
	x1=0.0;
	xf=PrIter(x1);
	f=fun1(xf);
	//printf("%e   %e\n",&xf,&f);
	cout<<xf<<"    "<<f<<endl;
	//free(xf);
	//free(x1);
	//free(f);

	return 0;
}
