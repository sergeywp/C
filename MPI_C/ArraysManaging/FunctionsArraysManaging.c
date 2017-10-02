#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void init_Array(int **arx, int count){
	int i;
	*arx = malloc(count* sizeof(int*));
	for(i=0; i<count; i++){
		*(*arx+i)=i;
	}
}

void IMPI_Init(int *world_size, int *world_rank, char **processor_name, int *name_len){

	*processor_name = malloc(MPI_MAX_PROCESSOR_NAME*sizeof(char*));	
	
	//Initialize the MPI environment
        MPI_Init(NULL, NULL);

	/*так как у нас в функцию передаются уже адреса переменных, то в функцию мы их сразу и передаем
	(в отличае от случая если бы у нас была переменная и мы бы передавали адрес епременной и вызов функции вылядил бы следующим образом:
	MPI_Comm_size(MPI_COMM_WORLD, &world_size)*/
        //Get the number of process
        MPI_Comm_size(MPI_COMM_WORLD, world_size);

        //Get the rank of the process
        MPI_Comm_rank(MPI_COMM_WORLD, world_rank);

        //Get the name of the processor
        MPI_Get_processor_name(*processor_name, name_len);

}

int IMPI_Show_Arrays_Values(int **firstUnderArray, int **secondUnderArray, int underArraysSize, int underArraysLastSize, int world_rank, int world_size){
	int i;
	if(world_rank!=0){
		if(world_rank!=world_size-1){
			for(i=0; i<underArraysSize; i++){
				printf("first: %d, second: %d, size: %d, world_rank: %d\n",*(*firstUnderArray+i), *(*secondUnderArray+i), underArraysSize, world_rank);
			}
		}else{
			for(i=0; i<underArraysLastSize; i++){
				printf("first: %d, second: %d, size: %d, world_rank: %d\n",*(*firstUnderArray+i), *(*secondUnderArray+i), underArraysSize, world_rank);
			}
		}
	}
}

int IMPI_Determine_UnderArrays(int **array, int **underArray, int *underArraySize, int *underArrayLastSize, bool *remainUnderArray, int count, int world_size ){
	if(count%(world_size-1)==0){
		*underArraySize = count/(world_size-1);
		*underArrayLastSize = 0;
		*underArray = malloc(*underArraySize*sizeof(int*));
		*remainUnderArray =false;
	}else{
		*underArraySize = count/(world_size-2);
		*underArrayLastSize = count%(world_size-2);
		*underArray = malloc(count/(*underArraySize)*sizeof(int*));
		*remainUnderArray = true;
	}
}

int IMPI_Split_Array(int **array, int **underArray, int *underArraySize, int *underArrayLastSize, int count, int world_size, int world_rank){

	int i,j;
	bool remainUnderArray;

	IMPI_Determine_UnderArrays(array, underArray, underArraySize, underArrayLastSize, &remainUnderArray, count, world_size );
	//printf("underArraySize: %d; underArrayLastSize: %d; remainUnderArray: %d\n", underArraySize, underArrayLastSize, remainUnderArray);

	if(world_rank==0){
	//Главный поток
        
            for(i=0; i<world_size-1; i++){
			//Если разсер подмассивов делится на цело
			if(!remainUnderArray){
	              for(j=0; j<*underArraySize; j++){
        	        *(*underArray+j) = *(*array+i*(*underArraySize)+j);
					//printf("num %d\n",*(*num+j));
                	        }
				MPI_Send(*underArray, *underArraySize, MPI_INT, i+1, 0, MPI_COMM_WORLD);

			//Размер подмассивов не делится нацело
			}else{
				//Если взят подмассив сцелым количеством элементов
				if(!(i==world_size-2)){
					for(j=0; j<*underArraySize; j++){
                             			*(*underArray+j) = *(*array+i*(*underArraySize)+j);
						//printf("num %d\n",*(*num+j));
                                	}
					MPI_Send(*underArray, *underArraySize, MPI_INT, i+1, 0, MPI_COMM_WORLD);

				//Если взят массив с отсаточным количеством элементов
				}else{
					for(j=0; j<*underArrayLastSize; j++){
                                 	       *(*underArray+j) = *(*array+i*(*underArraySize)+j);
						//printf("num %d\n",*(*num+j));
	                                }
					MPI_Send(*underArray, *underArrayLastSize, MPI_INT, i+1, 0, MPI_COMM_WORLD);
				}
			}
                }

	}else{
	//Другие потоки
		//Если размер подмассивов делится на цело
		if(!remainUnderArray){
			MPI_Recv(*underArray, *underArraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			/*for(j=0; j<underArraySize; j++){
				printf("world_rank %d, j = %d, num = %d \n", world_rank,j, *(*num+j));
			}*/

		//Размер массивов не делится на цело
		}else{
			//Если взят поток, который обробатывает целое количество элементов
			if(!(world_rank==world_size-1)){
				MPI_Recv(*underArray, *underArraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for(j=0; j<*underArraySize; j++){
					//printf("world_rank %d, j = %d, num = %d \n", world_rank,j, *(*underArray+j));
				}

			//Если взят массив, который обрабатывает массив с остаточным количеством элементов
			}else{
				MPI_Recv(*underArray, *underArrayLastSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				/*for(j=0; j<underArrayLastSize; j++){
					printf("ost world_rank %d, j = %d, num = %d \n", world_rank,j, *(*num+j));
				}*/
			}
		}

	}


}

int main(int argc, char** argv){

	int i,j;
    int world_size;
	int world_rank;
    int name_len;
    char *processor_name;

	//Передаем адреса переменых в функцию
	IMPI_Init(&world_size, &world_rank, &processor_name, &name_len);
	//printf("world_size: %d ; world_rank: %d; processor_name: %s; name_len: %d\n",world_size, world_rank, processor_name, name_len);

	int *firstArray;
	int *secondArray;
	int firstUnderArraySize;
	int firstUnderArrayLastSize;

	int *firstUnderArray;
	int *secondUnderArray;
	int secondUnderArraySize;
	int secondUnderArrayLastSize;

	const int COUNT = 107;

	init_Array(&firstArray, COUNT);
	init_Array(&secondArray, COUNT);

	//printf("first\n");
	IMPI_Split_Array(&firstArray, &firstUnderArray, &firstUnderArraySize, &firstUnderArrayLastSize, COUNT, world_size, world_rank);
	//printf("second\n");
	IMPI_Split_Array(&secondArray, &secondUnderArray, &secondUnderArraySize, &secondUnderArrayLastSize, COUNT, world_size, world_rank);
	/*Разделили массивы, теперь для каждого потока, работать нужно будет с подмассивом firstUnderArray и secondUnderArray,
	  если номер потока является последним, то размер массива будет firstUnderArrayLastSize, в противном случае, размер - firstUnderArraySize.*/
	
	IMPI_Show_Arrays_Values(&firstUnderArray, &secondUnderArray, firstUnderArraySize, firstUnderArrayLastSize, world_rank, world_size);

    MPI_Finalize();


}

