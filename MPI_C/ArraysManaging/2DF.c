#include<mpi.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//#define sizei 5
//#define sizej 5

//Получение идентификатора элемента
int ic(int i, int sizei, int sizej){return (((i))%(sizei));}
int jc(int i, int sizei, int sizej){return (((i-ic(i,sizei,sizej))/(sizei))%(sizej));}

int intSqrt(int d){
	int i;
	int res = 0;
	for(i=0; i<10000; i++){
		if(i*i>d){
			res=i-1;
			break;
		}
	}
	return res;
}

//Инициализация MPI
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

//----Действия над простыми массивами, спомощью которых будем проверять результат--------
void initSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej ){
	*firstArray = malloc(sizei*sizej*sizeof(int*));
	*secondArray = malloc(sizei*sizej*sizeof(int*));
	*resultArray = malloc(sizei*sizej*sizeof(int*));
}


void fullSimpleCheckArrays(int **firstArray, int **secondArray, int sizei, int sizej){
	int i;
	for(i=0; i<sizei*sizej; i++){
		*(*firstArray+i)=i;
		*(*secondArray+i)=i*2;
	}
}

void sumSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej){
	int i;
	for(i=0; i<sizei*sizej; i++){
		*(*resultArray+i)=*(*firstArray+i)+*(*secondArray+i);
	}
}

void printSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej){
	int i;
	for(i=0; i<sizei*sizej; i++){
		printf("values : c[%i,%i]=%i (%i+%i) \n",ic(i,sizei,sizej),jc(i,sizei,sizej),*(*resultArray+i),*(*firstArray+i),*(*secondArray+i));
	}
}

void processSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej, int world_rank){
	if(world_rank==0){
		initSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej);
		fullSimpleCheckArrays(firstArray, secondArray, sizei, sizej);
		sumSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej);
		//printSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej);
	}
}
//---------------------------------------------------------------------------------------

//----Непосредственно, работа с массивами для MPI----------------------------------------

//----1)Разделение 2хмерного массива на полоски------------------------------------------

int IMPI_Determine_UnderArraysStripes(int **array, int **underArray, int *parts_count_i, int *parts_count_j, bool twoDimension, int *underArraySize_i, int *underArraySize_j, int *underArrayLastSize_i, int *underArrayLastSize_j, bool *remainUnderArray_i, bool *remainUnderArray_j, int sizei, int sizej, int world_size ){

	if(!twoDimension){
		//Одномерная декомпозиция
		if(sizei%(world_size-1)==0){
			*parts_count_i = world_size-1;
			*parts_count_j = 1;
			*underArraySize_i = sizei/(world_size-1);
			*underArraySize_j = sizej;
			*underArrayLastSize_i = 0;
			*underArrayLastSize_j = 0;
			*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
			*remainUnderArray_i = false;
			*remainUnderArray_j = false;
			printf("parts_count : (%d:%d)*\n",*parts_count_i, *parts_count_j);
		}else{
			*underArraySize_i = sizei/(world_size-2);
			*underArraySize_j = sizej;
			*parts_count_i = world_size-2;
			*parts_count_j = 1;
			*underArrayLastSize_i = sizei%(world_size-2);
			*underArrayLastSize_j = sizej;
			*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
			*remainUnderArray_i = true;
			*remainUnderArray_j = false;
			printf("parts_count : (%d:%d)**\n",*parts_count_i, *parts_count_j);
		}
	}else{
		//Двухмерная декомпозиция
		if(sizei%intSqrt(world_size-1)==0){
			if(sizej%((world_size-1)/(intSqrt(world_size-1)))){
				//i нацело, j нацело
				*parts_count_i = intSqrt(world_size-1);
				*parts_count_j = (world_size-1)/(intSqrt(world_size-1));
				*underArraySize_i = sizei/(*parts_count_i);
				*underArraySize_j = sizej/(*parts_count_j);
				*underArrayLastSize_i = sizei%*parts_count_i;
				*underArrayLastSize_j = sizej%*parts_count_j;
				*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
				*remainUnderArray_i = false;
				*remainUnderArray_j = false;
				printf("parts_count : ((%d+%d):(%d+%d))***\n",*parts_count_i, *remainUnderArray_i, *parts_count_j, *remainUnderArray_j);
			}else{
				//i нацело, j не нацело
				*parts_count_i = intSqrt(world_size-1);
				*parts_count_j = (world_size-1)/(intSqrt(world_size-1))-1;
				*underArraySize_i = sizei/(*parts_count_i);
				*underArraySize_j = sizej/(*parts_count_j);
				*underArrayLastSize_i = sizei%*parts_count_i;
				*underArrayLastSize_j = sizej%*parts_count_j;
				*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
				*remainUnderArray_i = false;
				*remainUnderArray_j = true;
				printf("parts_count : ((%d+%d):(%d+%d))***\n",*parts_count_i, *remainUnderArray_i, *parts_count_j, *remainUnderArray_j);
			}
		}else{
			if(sizej%(world_size-1)/(intSqrt(world_size-1))){
				//i не нацело, на j нацело
				*parts_count_i =intSqrt(world_size-1)-1;
				*parts_count_j = (world_size-1)/(intSqrt(world_size-1));
				*underArraySize_i = sizei/(*parts_count_i);
				*underArraySize_j = sizej/(*parts_count_j);
				*underArrayLastSize_i = sizei%*parts_count_i;
				*underArrayLastSize_j = sizej%*parts_count_j;
				*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
				*remainUnderArray_i = true;
				*remainUnderArray_j = false;
				printf("parts_count : ((%d+%d):(%d+%d))***\n",*parts_count_i, *remainUnderArray_i, *parts_count_j, *remainUnderArray_j);
			}else{
				//i не нацело, на j не нацело
				*parts_count_i = intSqrt(world_size-1)-1;
				*parts_count_j = (world_size-1)/(intSqrt(world_size-1))-1;
				*underArraySize_i = sizei/(*parts_count_i);
				*underArraySize_j = sizej/(*parts_count_j);
				*underArrayLastSize_i = sizei%*parts_count_i;
				*underArrayLastSize_j = sizej%*parts_count_j;
				*underArray = malloc(*underArraySize_i**underArraySize_j*sizeof(int*));
				*remainUnderArray_i = false;
				*remainUnderArray_j = false;
				printf("parts_count : ((%d+%d):(%d+%d))***\n",*parts_count_i, *remainUnderArray_i, *parts_count_j, *remainUnderArray_j);
			}
		}
	}
}

void IMPI_PrintArraysInfo(int **array,  int **underArray, int *partsCount_i,  int *partsCount_j, int *underArraySize_i,  int *underArraySize_j,  int *underArrayLastSize_i,  int *underArrayLastSize_j,  bool *remainUnderArray_i,  bool *remainUnderArray_j, int sizei, int sizej, int world_rank){
	printf("underArray [%i,%i]\n", *underArraySize_i, *underArraySize_j);
	printf("underLastArray [%i,%i]\n", *underArrayLastSize_i, *underArrayLastSize_j);
	printf("remainUnderArray [%i,%i]\n", *remainUnderArray_i, *remainUnderArray_j);
	printf("partsCountArray [%i,%i]\n", *partsCount_i, *partsCount_j);
}

void IMPI_Split_Array(int **array,  int **underArray, int *partsCount_i,  int *partsCount_j, int *underArraySize_i,  int *underArraySize_j,  int *underArrayLastSize_i,  int *underArrayLastSize_j, int sizei, int sizej, int world_rank, int world_size){
	bool remainUnderArray_i;
	bool remainUnderArray_j;
	//в этих переменных хранится количество массивов по i и j
	int i,j,ip,jp;
	int i_arf,i_art,j_arf,j_art,block_id;

	IMPI_Determine_UnderArraysStripes(array, underArray, partsCount_i, partsCount_j, false, underArraySize_i, underArraySize_j, underArrayLastSize_i, underArrayLastSize_j,&remainUnderArray_i, &remainUnderArray_j, sizei, sizej, world_size);

	if(world_rank==0){
		if(!remainUnderArray_i){
			if(!remainUnderArray_j){
				for(ip=0; ip<*partsCount_i; ip++){
					for(jp=0; jp<*partsCount_j; jp++){
						//По i-норм, по j-норм
						i_arf = ip**underArraySize_i;
						i_art = (ip**underArraySize_i)+*underArraySize_i;
						j_arf = jp**underArraySize_j;
						j_art = (jp**underArraySize_j)+*underArraySize_j;
						printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j-norm)(%d/%d/%d/%d)*\n", block_id, i_arf, i_art, j_arf, j_art, ip, jp, *partsCount_i, *partsCount_j);
					}
				}
			}else{
				for(ip=0; ip<*partsCount_i; ip++){
					for(jp=0; jp<*partsCount_j+1; jp++){
						//По i-норм, по j-не норм
						if(jp==*partsCount_j){
							block_id = ip+jp**partsCount_i;
							i_arf = ip**underArraySize_i;
							i_art = (ip**underArraySize_i)+*underArraySize_i;
							j_arf = jp**underArraySize_j;
							j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
							printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j-ne norm)(%d/%d/%d/%d)**\n", block_id, i_arf, i_art, j_arf, j_art, ip, jp, *partsCount_i, *partsCount_j);
						}else{
							block_id = ip+jp**partsCount_i;
							i_arf = ip**underArraySize_i;
							i_art = (ip**underArraySize_i)+*underArraySize_i;
							j_arf = jp**underArraySize_j;
							j_art = (jp**underArraySize_j)+*underArraySize_j;
							printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j- norm)(%d/%d/%d/%d)***\n", block_id, i_arf, i_art, j_arf, j_art, ip, jp, *partsCount_i, *partsCount_j);
						}
					}
				}
			}
		}else{
			if(!remainUnderArray_j){
				for(ip=0; ip<*partsCount_i+1; ip++){
					for(jp=0; jp<*partsCount_j; jp++){
						//По i-не норм, по j-норм
						if(ip==*partsCount_i){
							block_id = ip+jp**partsCount_i;
							i_arf = ip**underArraySize_i;
							i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
							j_arf = jp**underArraySize_j;
							j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
							printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-ne norm, j-norm)(%d/%d/%d/%d)****\n", block_id, i_arf, i_art, j_arf, j_art, ip, jp, *partsCount_i, *partsCount_j);
						}else{
							block_id = ip+jp**partsCount_i;
							i_arf = ip**underArraySize_i;
							i_art = (ip**underArraySize_i)+*underArraySize_i;
							j_arf = jp**underArraySize_j;
							j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
							printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j-norm)(%d/%d/%d/%d)*****\n", block_id, i_arf, i_art, j_arf, j_art, ip, jp, *partsCount_i, *partsCount_j);
						}
					}
				}
			}else{
				for(ip=0; ip<*partsCount_i+1; ip++){
					for(jp=0; jp<*partsCount_j+1; jp++){
						//По i-не норм, по j-не норм
						if(ip==*partsCount_i){
							if(jp==*partsCount_j){
								block_id = ip+jp**partsCount_i;
								i_arf = ip**underArraySize_i;
								i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
								j_arf = jp**underArraySize_j;
								j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
								printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-ne norm, j-ne norm)******\n", block_id, i_arf, i_art, j_arf, j_art);
							}else{
								block_id = ip+jp**partsCount_i;
								i_arf = ip**underArraySize_i;
								i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
								j_arf = jp**underArraySize_j;
								j_art = (jp**underArraySize_j)+*underArraySize_j;
								printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-ne norm, j-norm)*******\n", block_id, i_arf, i_art, j_arf, j_art);
							}
						}else{
							if(jp==*partsCount_j){
								block_id = ip+jp**partsCount_i;
								i_arf = ip**underArraySize_i;
								i_art = (ip**underArraySize_i)+*underArraySize_i;
								j_arf = jp**underArraySize_j;
								j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
								printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j-norm)********\n", block_id, i_arf, i_art, j_arf, j_art);
							}else{
								block_id = ip+jp**partsCount_i;
								i_arf = ip**underArraySize_i;
								i_art = (ip**underArraySize_i)+*underArraySize_i;
								j_arf = jp**underArraySize_j;
								j_art = (jp**underArraySize_j)+*underArraySize_j;
								printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); (i-norm, j-ne norm)*********\n", block_id, i_arf, i_art, j_arf, j_art);
							}
						}

					}
				}
			}
		}

	}else{
		//Не главные потоки
	}
}


/**TODO: потом переименовать метод или же в main зафигачить вызовы методов**/
void processMpiCheckArrays(int **firstArray,  int **firstUnderArray,  int *firstPartsCount_i,  int *firstPartsCount_j,  int *firstUnderArraySize_i,  int *firstUnderArraySize_j,  int *firstUnderArrayLastSize_i,  int *firstUnderArrayLastSize_j,  bool *firstRemainUnderArray_i,  bool *firstRemainUnderArray_j,
		                   int **secondArray, int **secondUnderArray, int *secondPartsCount_i, int *secondPartsCount_j, int *secondUnderArraySize_i, int *secondUnderArraySize_j, int *secondUnderArrayLastSize_i, int *secondUnderArrayLastSize_j, bool *secondRemainUnderArray_i, bool *secondRemainUnderArray_j,
		                   int sizei, int sizej, int world_rank, int world_size){
	if(world_rank==0){
		IMPI_Split_Array(firstArray,firstUnderArray, firstPartsCount_i, firstPartsCount_j, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j, sizei, sizej, world_rank, world_size);

//		IMPI_Determine_UnderArraysStripes(firstArray, firstUnderArray, firstPartsCount_i, firstPartsCount_j, true, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j,firstRemainUnderArray_i, firstRemainUnderArray_j, sizei, sizej, world_size);
//		IMPI_PrintArraysInfo(firstArray, firstUnderArray, firstPartsCount_i, firstPartsCount_j, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j,firstRemainUnderArray_i, firstRemainUnderArray_j, sizei, sizej, world_rank);

		//----------------------Для второго массива------------------------------------------------
		//IMPI_Determine_UnderArraysStripes(secondArray, secondUnderArray, secondPartsCount_i, secondPartsCount_j, secondUnderArraySize_i, secondUnderArraySize_j, secondUnderArrayLastSize_i, secondUnderArrayLastSize_j,secondRemainUnderArray_i, secondRemainUnderArray_j, sizei, sizej, world_size);
		//IMPI_PrintArraysInfo(secondArray, secondUnderArray, secondPartsCount_i, secondPartsCount_j, secondUnderArraySize_i, secondUnderArraySize_j, secondUnderArrayLastSize_i, secondUnderArrayLastSize_j,secondRemainUnderArray_i, secondRemainUnderArray_j, sizei, sizej, world_rank);
	}
}

//---------------------------------------------------------------------------------------

int main(int argc, char** argv){
	
	int world_size;
	int world_rank;
	int name_len;
	char *processor_name;

	int *firstArray;
	int firstPartsCount_i;
	int firstPartsCount_j;
	int *firstUnderArray;
	int firstUnderArraySize_i;
	int firstUnderArraySize_j;
	int firstUnderArrayLastSize_i;
	int firstUnderArrayLastSize_j;
	bool firstRemainUnderArray_i;
	bool firstRemainUnderArray_j;

	int *secondArray;
	int secondPartsCount_i;
	int secondPartsCount_j;
	int *secondUnderArray;
	int secondUnderArraySize_i;
	int secondUnderArraySize_j;
	int secondUnderArrayLastSize_i;
	int secondUnderArrayLastSize_j;
	bool secondRemainUnderArray_i;
	bool secondRemainUnderArray_j;

	int sizei=45;
	int sizej=45;
	int *resultArray;

	IMPI_Init(&world_size, &world_rank, &processor_name, &name_len);
	processSimpleCheckArrays(&firstArray, &secondArray, &resultArray, sizei, sizej, world_rank);

	processMpiCheckArrays(&firstArray,  &firstUnderArray, &firstPartsCount_i, &firstPartsCount_j, &firstUnderArraySize_i,  &firstUnderArraySize_j,  &firstUnderArrayLastSize_i,  &firstUnderArrayLastSize_j,  &firstRemainUnderArray_i,  &firstRemainUnderArray_j,
			              &secondArray, &secondUnderArray, &secondPartsCount_i, &secondPartsCount_j, &secondUnderArraySize_i, &secondUnderArraySize_j, &secondUnderArrayLastSize_i, &secondUnderArrayLastSize_j, &secondRemainUnderArray_i, &secondRemainUnderArray_j,
			              sizei, sizej, world_rank, world_size);

	MPI_Finalize();
}
