#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

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
        const int COUNT = 100;

        /*int number[COUNT];
        int num[COUNT/10];*/


	/*!!!! НЕОБХОДИМО ПРОРАБОТАТЬ ЭТОТ ВОПРОС, БУДЕТ ЛИ ПЕРВЫЙ ПОТОК, 
	       ПЕРЕДАЮЩИЙ ДАННЫЕ ВЫПОЛНЯТЬ ВЫЧИСЛЕНИЯ ИЛИ НЕТ !!!!!
		ЕСЛИ НЕТ, ТО underArraySize НУЖНО ДЕЛАТЬ НА 1 МЕНЬШУ world_rank*/
	int i,j;
	int *number;
	int *num;
	int underArraySize;
	int underArrayLastSize;
	bool remainUnderArray;

	number = malloc(COUNT* sizeof(int*));
	if(COUNT%(world_size-1)==0){
		underArraySize = COUNT/(world_size-1);
		underArrayLastSize = 0;
		num = malloc(underArraySize*sizeof(int*));
		remainUnderArray =false;
	}else{
		underArraySize = COUNT/(world_size-2);
		underArrayLastSize = COUNT%(world_size-2);
		num = malloc(COUNT/underArraySize*sizeof(int*));
		remainUnderArray = true;
	}

        if(world_rank==0){
	//Главный поток
                //Заполняю массив
                for(i=0; i<COUNT; i++){
                        *(number+i)=i;
                }

                for(i=0; i<world_size-1; i++){
			//Если разсер подмассивов делится на цело
			if(!remainUnderArray){
	                        for(j=0; j<underArraySize; j++){
        	                        num[j] = *(number+i*underArraySize+j);
                	        }
				MPI_Send(num, underArraySize, MPI_INT, i+1, 0, MPI_COMM_WORLD);

			//Размер подмассивов не делится нацело
			}else{
				//Если взят подмассив сцелым количеством элементов
				if(!(i==world_size-2)){
					for(j=0; j<underArraySize; j++){
                             			num[j] = *(number+i*underArraySize+j);
                                	}
					MPI_Send(num, underArraySize, MPI_INT, i+1, 0, MPI_COMM_WORLD);

				//Если взят массив с отсаточным количеством элементов
				}else{
					for(j=0; j<underArrayLastSize; j++){
                                 	       num[j] = *(number+i*underArraySize+j);
	                                }
					MPI_Send(num, underArrayLastSize, MPI_INT, i+1, 0, MPI_COMM_WORLD);
				}
			}
                }

	}else{
	//Другие потоки
		//Если размер подмассивов делится на цело
		if(!remainUnderArray){
			MPI_Recv(num, underArraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(j=0; j<underArraySize; j++){
				printf("world_rank %d, j = %d, num = %d \n", world_rank,j, *(num+j));
			}

		//Размер массивов не делится на цело
		}else{
			//Если взят поток, который обробатывает целое количество элементов
			if(!(world_rank==world_size-1)){
				MPI_Recv(num, underArraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for(j=0; j<underArraySize; j++){
					printf("world_rank %d, j = %d, num = %d \n", world_rank,j, *(num+j));
				}

			//Если взят массив, который обрабатывает массив с остаточным количеством элементов
			}else{
				MPI_Recv(num, underArrayLastSize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for(j=0; j<underArrayLastSize; j++){
					printf("ost world_rank %d, j = %d, num = %d \n", world_rank,j, *(num+j));
				}
			}
		}

	}

	//---------------------------------------------------------------
        //Finalize the MPI environment
        MPI_Finalize();


}

