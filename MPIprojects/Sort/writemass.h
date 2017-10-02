//#include "stdafx.h"
//#include <iostream>
//using namespace std;
#include <stdlib.h>
#include <stdio.h>

void writemass2(double **a, int n)
{								  
	//cout<<"------------------------------"<<endl;
	printf("-------------------------------\n");
	for(int i=0; i<n;i++)
	{
		for(int j=0; j<n;j++)
		{
			//cout<<a[i][j]<<" ";
			printf("%lf ",a[i][j]);
		}
		//cout<<endl;
		printf("\n");
	}
	//cout<<"------------------------------"<<endl;
	printf("-------------------------------\n");

}
//Function of showing 1d massive #include <iostream>
void writemass1(double *a, int n)
{								  
	//cout<<"------------------------------"<<endl;
	printf("-------------------------------\n");

	for(int i=0; i<n;i++)
	{
		//cout<<a[i]<<" ";
		printf("%lf ", a[i]);
	}
	//cout<<endl;
	printf("\n");
	//cout<<"------------------------------"<<endl;
	printf("-------------------------------\n");

}
