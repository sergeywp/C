// ConsoleApplication1.cpp: //change coding (in windows os)!!!!! //setlocale( LC_ALL,"Russian" ); //
//4*x*x-4*x*y+3*y*y+x+y
//compiling: mpicc -o exe_c mpi_c.c
//running: mpiexec -l -n N ./exe.c
//#include "stdafx.h"
//#include <iostream>
#include "functions.h"
#include <stdlib.h>
#include "mpi.h"
#include <sys/types.h>
#include <unistd.h>

//using namespace std;

int main(int argc, char** argv)
{

	//initialization of 2d massive
	double **a;
    	a = (double**)malloc(sizeof(double*)*2);
    	for (int i1 =0; i1<2; i1 ++)
    		a[i1] = (double*)malloc(sizeof(double)*2);

	//initializatioon of 1d massive
	double *b;
    	b = (double*)malloc(sizeof(double)*2);
	//b = new double [2];
	double *x;
    	x = (double*)malloc(sizeof(double)*2);
	//x = new double [2];
	double *grad;
	grad = (double*)malloc(sizeof(double)*2);
	//grad = new double [2];

	//Initialize MPI
	int rank, size, type=99;
	double dbuff;
	int type2=98;
	int type3=97;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	
	
	//variables
	double eps=0.0000000000001;
	double *epps;
	//epps = new double[2];
	epps = (double*)malloc(2*sizeof(double));
	double *next;
	next = (double*)malloc(2*sizeof(double));
	//next = new double[2];
	//x=MinMeth(x,a,b);
	if(size==2)
	{
		if(rank==0)
		{
			InitializeMas(a,b);
			FILE  *filekoef;
			filekoef = fopen("/home/sergey/MPIprojects/Method/Method/koef.dat","w");
			InitializeNachZnach(x);
			int tag=0;
			for(int i2=0; i2<2; i2++)
			{
				for(int i3=0; i3<2; i3++)
				{
					dbuff=a[i2][i3];
					MPI_Send(&dbuff,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
					tag++;
					fprintf(filekoef,"%e\n",a[i2][i3]);
					//printf("%lf  ",a[i2][i3]);
				}
			}
			for(int i2=0; i2<2; i2++)
			{
				dbuff=b[i2];
				MPI_Send(&dbuff,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
				tag++;
				fprintf(filekoef,"%e\n",b[i2]);
			}

			fclose(filekoef);
			FILE *file;
			file = fopen("/home/sergey/MPIprojects/Method/Method/data.dat", "w");

			//for(int i2=0; i2<2; i2++)
			fprintf(file,"%e  %e 0.0\n", x[0],x[1]);
			int i=0;
			do
			{
				for(int i2=0; i2<2; i2++)
				{
					dbuff=x[i2];
					MPI_Send(&dbuff,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD);
					tag++;
				}
				next[0]=newxg(x,a,b);
				//fprintf(file,"%e  %e 0.0\n",next[0],x[1]);
				epps[0]=abs(x[0]-next[0]);
				//writemass1(epps,2);
				//cout<<epps[0]<<"   "<<epps[1]<<endl;
				MPI_Recv(&dbuff,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD,&status);
				next[1]=dbuff;
				fprintf(file,"%e  %e 0.0\n",next[0],next[1]);
				tag++;
				MPI_Recv(&dbuff,1,MPI_DOUBLE,1,tag,MPI_COMM_WORLD,&status);
				epps[1]=dbuff;
				tag++;
				
				//printf("%lf      %lf\n",next[1],epps[1]);
				if(epps[0]>eps) {
					x[0]=next[0];
					if(epps[1]>eps) x[1]=next[1];
				}
				else
				{
					if(epps[1]>eps) x[1]=next[1];
					else break;
				}
				i++;
			}
			while(i<1000);
			fclose(file);
			system("/home/sergey/gnuplot/1/test/gnuplot_exe");
			//cout<<"znachenie min:"<<endl;
			printf("znachenie min:\n");
			writemass1(x,2);
			grad[0]=dfundx(x,a,b);
			//cout<<"znachenie grad v min:"<<endl;
			printf("znachenie grad v min:\n");
			grad[1]=dfundy(x,a,b);
			writemass1(grad,2);
			//writemass2(a,2);
			//delete massives
			for (int i=0; i< 2; i++) free(a[i]);
			free(a);
			free(b);
			//delete x;
			free(x);
		} 
		else 
		{
			//printf("process number %d\n",rank);
			int tag=0;
			for(int i2=0;i2<2;i2++)
			{
				for(int i3=0;i3<2;i3++)
				{
					MPI_Recv(&dbuff,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
					a[i2][i3]=dbuff;
					tag++;
					//printf("%lf ",a[i2][i3]);
				}
			}
			for(int i2=0;i2<2;i2++)
			{
				MPI_Recv(&dbuff,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
				b[i2]=dbuff;
				tag++;
			}
			int i=0;
			do
			{
				for(int i2=0;i2<2;i2++)
				{
					MPI_Recv(&dbuff,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD,&status);
					x[i2]=dbuff;
					tag++;
				}
				next[1]=newyg(x,a,b);
				epps[1]=abs(x[1]-next[1]);
				dbuff=next[1];
				MPI_Send(&dbuff,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
				tag++;
				dbuff=epps[1];
				MPI_Send(&dbuff,1,MPI_DOUBLE,0,tag,MPI_COMM_WORLD);
				tag++;
				i++;
				if(epps[1]<eps)break;
			}
			while(i<1000);
		//writemass2(a,2);
		//writemass1(b,2);
		}
	}
	else
	{ 
		if(rank==0)
		printf("Not 2 process.\nAutomatic stop.\n");
	}
		MPI_Finalize();
	

	return 0;
}

