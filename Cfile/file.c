//gcc file.c -o file_exe
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *enpv[])
{
	double *a;
	a = (double*)malloc(sizeof(double)*50);

	FILE *file;
	file = fopen("fprintf.txt", "w");

	int i;
	for(i=0; i<50;i++)
	{
		a[i]=i*0.5+0.0000254684;
		fprintf(file,"%e \n", a[i]);
	}
	fprintf(file,"EOF!!!\n");
	fclose(file);

	free(a);
	return 0;
}
