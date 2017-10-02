// ConsoleApplication1.cpp: 
//change coding (in windows os)!!!!! 
//setlocale( LC_ALL,"Russian" );
//

//#include "stdafx.h"
//#include <iostream>
#include "functions.h"
#include <mpi.h>

//using namespace std;

int main(int argc, char** argv)
{
	//initialization of 2d massive
	double **a;
    	a = new double *[2];
    	for (int i =0; i<2; i ++)
	a[i] = new double [2];
	//initializatioon of 1d massive
	double *b;
	b = new double [2];
	double *x;
	x = new double [2];

	double *grad;
	grad = new double [2];

	//functions.h
	InitializeMas(a,b);
	InitializeNachZnach(x);
	
	//initialisation MPI
	int rank, size, type=99;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank==0)
	{

		double eps=0.0000000000001;
		double *epps;
		epps = new double[2];
		double *next;
		next = new double[2];
		int ii=0;
		do
		{
			next[0]=newx(x,a,b);
			next[1]=newy(x,a,b);
			epps[0]=abs(x[0]-next[0]);
			epps[1]=abs(x[1]-next[1]);
			if(abs(x[0]-next[0])>eps) {
				x[0]=next[0];
				if(abs(x[1]-next[1])>eps) x[1]=next[1];
			}
			else
			{
				if(abs(x[1]-next[1])>eps) x[1]=next[1];
				else break;
			}
			ii++;
		}
		while(ii<1000);
		//cout<<"znachenie min:"<<endl;
		printf("znachenie min:\n");
		writemass1(x,2);
		printf("znachenie grad v min:\n");
		//cout<<"znachenie grad v min:"<<endl;
		grad[0]=dfundx(x,a,b);
		grad[1]=dfundy(x,a,b);
		writemass1(grad,2);	
	//delete massives
	for (int i=0; i< 2; i++)delete a[i];
	delete a;
	delete b;
	delete x;
	}
	MPI_Finalize();
	return 0;
}

