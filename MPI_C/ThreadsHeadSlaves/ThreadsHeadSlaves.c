#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){

        //Initialize the MPI environment
        MPI_Init(NULL, NULL);

        //Get the number of process
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        //Get the rank of the process
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        //Get the name of the processor
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        int name_len;
        MPI_Get_processor_name(processor_name, &name_len);

	//----------------------------------------------------------------
	//Сюда писать сам код
	//---------------------------------------------------------------
	int number, answer;
	int i;
	if(world_rank==0){
		number = 568;
		for(i=1; i<world_size; i++){
			MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
		for(i=1; i<world_size; i++){
			MPI_Recv(&answer, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Sending successfully from %d!\n", answer);
		}
		printf("Thread of World Rank %d send number %d to all threads\n",world_rank, number);

	}else{
       		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		answer = world_rank;
		MPI_Send(&answer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		//printf("Thread of World Rank %d recive number %d\n",world_rank, number);
	}

	//---------------------------------------------------------------
        //Finalize the MPI environment
        MPI_Finalize();


}

