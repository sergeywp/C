//gcc runprog.c -o runnn
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *enpv[])
{
	system("mpiexec -l -n 4 /home/sergey/MPIprojects/Method/Method/exe_mpi_end");
	return 0;
}
