//gcc file.c -o file_exe
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *enpv[])
{
	double *a;
	a = (double*)malloc(sizeof(double)*51);

	FILE *file;
	file = fopen("fprintf.txt", "r");

	int i;
	for(i=0; i<51;i++)
	{
		fscanf(file,"%lf\n",&a[i]);
	}
	for(i=0; i<51;i++)
	{
		printf("%f\n",a[i]);
	}
	fclose(file);

	free(a);
	return 0;
}
