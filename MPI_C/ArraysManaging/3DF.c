#include<mpi.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//#define sizei 5
//#define sizej 5

//Получение идентификатора элемента
int ic(int i, int sizei, int sizej, int sizek){return (((i))%(sizei));}
int jc(int i, int sizei, int sizej, int sizek){return (((i-ic(i,sizei,sizej,sizek))/(sizei))%(sizej));}
int kc(int i, int sizei, int sizej, int sizek){return (((i-ic(i,sizei,sizej,sizek)-jc(i,sizei,sizej,sizek)*sizei)/(sizei*sizej))%(sizek));}

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

int intSqrtThree(int d){
	int i;
	int res = 0;
	for(i=0; i<10000; i++){
		if(i*i*i>d){
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
void initSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej, int sizek){
	*firstArray = malloc(sizei*sizej*sizek*sizeof(int*));
	*secondArray = malloc(sizei*sizej*sizek*sizeof(int*));
	*resultArray = malloc(sizei*sizej*sizek*sizeof(int*));
}


void fullSimpleCheckArrays(int **firstArray, int **secondArray, int sizei, int sizej, int sizek){
	int i;
	for(i=0; i<sizei*sizej*sizek; i++){
		*(*firstArray+i)=i;
		*(*secondArray+i)=i*2;
	}
}

void sumSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej, int sizek){
	int i;
	for(i=0; i<sizei*sizej*sizek; i++){
		*(*resultArray+i)=*(*firstArray+i)+*(*secondArray+i);
	}
}

void printSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej, int sizek){
	int i;
	for(i=0; i<sizei*sizej*sizek; i++){
		printf("values : c[%i,%i,%i]=%i (%i+%i) \n",ic(i,sizei,sizej,sizek),jc(i,sizei,sizej,sizek),kc(i,sizei,sizej,sizek),*(*resultArray+i),*(*firstArray+i),*(*secondArray+i));
	}
}

void processSimpleCheckArrays(int **firstArray, int **secondArray, int **resultArray, int sizei, int sizej, int sizek, int world_rank){
	if(world_rank==0){
		initSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej, sizek);
		fullSimpleCheckArrays(firstArray, secondArray, sizei, sizej, sizek);
		sumSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej, sizek);
		//printSimpleCheckArrays(firstArray, secondArray, resultArray, sizei, sizej, sizek);
	}
}
//---------------------------------------------------------------------------------------

//----Непосредственно, работа с массивами для MPI----------------------------------------

//----1)Разделение 2хмерного массива на полоски------------------------------------------

int IMPI_Determine_UnderArraysStripes(int **array, int **underArray, int *parts_count_i, int *parts_count_j, int *parts_count_k, bool twoDimension, bool threeDimession, int *underArraySize_i, int *underArraySize_j, int *underArraySize_k, int *underArrayLastSize_i, int *underArrayLastSize_j, int *underArrayLastSize_k, bool *remainUnderArray_i, bool *remainUnderArray_j, bool *remainUnderArray_k, int sizei, int sizej, int sizek, int world_size ){

	if(!threeDimession){
		if(!twoDimension){
			//Одномерная декомпозиция
			if(sizei%(world_size-1)==0){
				*parts_count_i = world_size-1;
				*parts_count_j = 1;
				*parts_count_k = 1;
			}else{
				*parts_count_i = world_size-2;
				*parts_count_j = 1;
				*parts_count_k = 1;
			}
		}else{
			//Двухмерная декомпозиция
			if(sizei%intSqrt(world_size-1)==0){
				if(sizej%((world_size-1)/(intSqrt(world_size-1)))==0){
					//i нацело, j нацело
					*parts_count_i = intSqrt(world_size-1);
					*parts_count_j = (world_size-1)/(intSqrt(world_size-1));
					*parts_count_k = 1;
				}else{
					//i нацело, j не нацело
					*parts_count_i = intSqrt(world_size-1);
					*parts_count_j = (world_size-1)/(intSqrt(world_size-1))-1;
					*parts_count_k = 1;
				}
			}else{
				if(sizej%((world_size-1)/(intSqrt(world_size-1)))==0){
					//i не нацело, на j нацело
					*parts_count_i =intSqrt(world_size-1)-1;
					*parts_count_j = (world_size-1)/(intSqrt(world_size-1));
					*parts_count_k = 1;
				}else{
					//i не нацело, на j не нацело
					*parts_count_i = intSqrt(world_size-1)-1;
					*parts_count_j = (world_size-1)/(intSqrt(world_size-1))-1;
					*parts_count_k = 1;
				}
			}
		}
	}else{
		//Трехмерная декомпозиция
		if(sizei%intSqrtThree(world_size-1)==0){
			if(sizej%intSqrtThree(world_size-1)==0){
				if(sizek%((world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1))))==0){
					//i нацело, j нацело, k нацело	
					*parts_count_i = intSqrtThree(world_size-1);
					*parts_count_j = intSqrtThree(world_size-1);
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)));
				}else{
					//i нацело, j нацело, k не нацело
					*parts_count_i = intSqrtThree(world_size-1);
					*parts_count_j = intSqrtThree(world_size-1);
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)))-1;
				}
			}else{
				if(sizek%((world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1))))==0){
					//i нацело, j не нацело, k нацело	
					*parts_count_i = intSqrtThree(world_size-1);
					*parts_count_j = intSqrtThree(world_size-1)-1;
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)));
				}else{
					//i нацело, j не нацело, k не нацело
					*parts_count_i = intSqrtThree(world_size-1);
					*parts_count_j = intSqrtThree(world_size-1)-1;
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)))-1;
				}
			}
		}else{
			if(sizej%(intSqrtThree(world_size-1))==0){
				if(sizek%((world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1))))==0){
					//i не нацело, j нацело, k нацело
					*parts_count_i = intSqrtThree(world_size-1)-1;
					*parts_count_j = intSqrtThree(world_size-1);
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)));
				}else{
					//i не нацело, j нацело, k не нацело
					*parts_count_i = intSqrtThree(world_size-1)-1;
					*parts_count_j = intSqrtThree(world_size-1);
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)))-1;
				}
			}else{
				if(sizek%((world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1))))==0){
					//i не нацело, j не нацело, k нацело
					*parts_count_i = intSqrtThree(world_size-1)-1;
					*parts_count_j = intSqrtThree(world_size-1)-1;
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)));
				}else{
					//i не нацело, j не нацело, k не нацело
					*parts_count_i = intSqrtThree(world_size-1)-1;
					*parts_count_j = intSqrtThree(world_size-1)-1;
					*parts_count_k = (world_size-1)/((intSqrtThree(world_size-1))*(intSqrtThree(world_size-1)))-1;
				}
			}
		}
	}

	*underArraySize_i = sizei/(*parts_count_i);
	*underArraySize_j = sizej/(*parts_count_j);
	*underArraySize_k = sizek/(*parts_count_k);
	*underArrayLastSize_i = sizei%(*parts_count_i);
	*underArrayLastSize_j = sizej%(*parts_count_j);
	*underArrayLastSize_k = sizej%(*parts_count_k);
	*underArray = malloc(*underArraySize_i**underArraySize_j**underArraySize_k*sizeof(int*));

	if(sizei%(*parts_count_i)==0)
		*remainUnderArray_i = false;
	else
		*remainUnderArray_i = true;

	if(sizej%(*parts_count_j)==0)
		*remainUnderArray_j = false;
	else
		*remainUnderArray_j = true;

	if(sizek%(*parts_count_k)==0)
		*remainUnderArray_k = false;
	else
		*remainUnderArray_k = true;
	printf("parts_count : ((%d+%d):(%d+%d):(%d+%d))\n",*parts_count_i, *remainUnderArray_i, *parts_count_j, *remainUnderArray_j, *parts_count_k, *remainUnderArray_k);

}
	
void IMPI_PrintArraysInfo(int **array,  int **underArray, int *partsCount_i,  int *partsCount_j, int *partsCount_k, int *underArraySize_i,  int *underArraySize_j,  int *underArraySize_k, int *underArrayLastSize_i,  int *underArrayLastSize_j,  int *underArrayLastSize_k, bool *remainUnderArray_i,  bool *remainUnderArray_j,  bool *remainUnderArray_k, int sizei, int sizej, int sizek, int world_rank){
	printf("underArray [%i,%i,%i]\n", *underArraySize_i, *underArraySize_j, *underArraySize_k);
	printf("underLastArray [%i,%i,%i]\n", *underArrayLastSize_i, *underArrayLastSize_j, *underArrayLastSize_k);
	printf("remainUnderArray [%i,%i,%i]\n", *remainUnderArray_i, *remainUnderArray_j, *remainUnderArray_k);
	printf("partsCountArray [%i,%i,%i]\n", *partsCount_i, *partsCount_j, *partsCount_k);
}

void IMPI_Split_Array(int **array,  int **underArray, int *partsCount_i,  int *partsCount_j,  int *partsCount_k, int *underArraySize_i,  int *underArraySize_j, int *underArraySize_k,  int *underArrayLastSize_i,  int *underArrayLastSize_j,  int *underArrayLastSize_k, int sizei, int sizej, int sizek, int world_rank, int world_size){
	bool remainUnderArray_i;
	bool remainUnderArray_j;
	bool remainUnderArray_k;
	//в этих переменных хранится количество массивов по i и j
	int i,j,k,ip,jp,kp;
	int i_arf,i_art,j_arf,j_art,k_arf,k_art,block_id;

	IMPI_Determine_UnderArraysStripes(array, underArray, partsCount_i, partsCount_j, partsCount_k, false, true, underArraySize_i, underArraySize_j, underArraySize_k, underArrayLastSize_i, underArrayLastSize_j, underArrayLastSize_k, &remainUnderArray_i, &remainUnderArray_j, &remainUnderArray_k, sizei, sizej, sizek, world_size);
	IMPI_PrintArraysInfo             (array, underArray, partsCount_i, partsCount_j, partsCount_k,              underArraySize_i, underArraySize_j, underArraySize_k, underArrayLastSize_i, underArrayLastSize_j, underArrayLastSize_k, &remainUnderArray_i, &remainUnderArray_j, &remainUnderArray_k, sizei, sizej, sizek, world_rank);

	if(world_rank==0){
		if(!remainUnderArray_i){
			if(!remainUnderArray_j){
				if(!remainUnderArray_k){
					for(ip=0; ip<*partsCount_i; ip++){
						for(jp=0; jp<*partsCount_j; jp++){
							for(kp=0; kp<*partsCount_k; kp++){
								//По i-норм, по j-норм, по k-норм
								block_id = ip+jp**partsCount_i+kp**partsCount_j**partsCount_i;
								i_arf = ip**underArraySize_i;
								i_art = (ip**underArraySize_i)+*underArraySize_i;
								j_arf = jp**underArraySize_j;
								j_art = (jp**underArraySize_j)+*underArraySize_j;
								k_arf = kp**underArraySize_k;
								k_art = (kp**underArraySize_k)+*underArraySize_k;
								printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j, *partsCount_k);
							}
						}
					}
				}else{
					for(ip=0; ip<*partsCount_i; ip++){
						for(jp=0; jp<*partsCount_j; jp++){
							for(kp=0; kp<*partsCount_k+1; kp++){
								//По i-норм, по j-норм, по k-не норм
								if(kp==*partsCount_k){
									//остаточный по k
									block_id = ip+jp**partsCount_i+kp**partsCount_j**partsCount_i;
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArraySize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArraySize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]**\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j, *partsCount_k+1);
								}else{
									//обычный
									block_id = ip+jp**partsCount_i+kp**partsCount_j**partsCount_i;
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArraySize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArraySize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArraySize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j, *partsCount_k+1);
								}
							}
						}
					}
				}
			}else{
				if(!remainUnderArray_k){
					for(ip=0; ip<*partsCount_i; ip++){
						for(jp=0; jp<*partsCount_j+1; jp++){
							for(kp=0; kp<*partsCount_k; kp++){
								//По i-норм, по j-не норм, по k-норм
								if(jp==*partsCount_j){
									//остаточный по j
									block_id = ip+jp**partsCount_i+kp*(*partsCount_j+1)**partsCount_i;
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArraySize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArraySize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k);
								}else{
									//обычний
									block_id = ip+jp**partsCount_i+kp*(*partsCount_j+1)**partsCount_i;
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArraySize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArraySize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArraySize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k);
								}
							}
						}
					}
				}else{
					for(ip=0; ip<*partsCount_i; ip++){
						for(jp=0; jp<*partsCount_j+1; jp++){
							for(kp=0; kp<*partsCount_k+1; kp++){
								//По i-норм, по j-не норм, по k-не норм
								if(jp==*partsCount_j){
									if(kp==*partsCount_k){
										//остаточный по j и k
										block_id = ip+jp**partsCount_i+kp*(*partsCount_j+1)*(*partsCount_i);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k+1);
									}else{
										//остаточный по j
										block_id = ip+jp**partsCount_i+kp*(*partsCount_j+1)**partsCount_i;
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k+1);
									}
								}else{
									if(kp==*partsCount_k){
										//остаточный по k
										block_id = ip+jp**partsCount_i+kp**partsCount_j**partsCount_i;
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k+1);
									}else{
										//обычный
										block_id = ip+jp**partsCount_i+kp**partsCount_j**partsCount_i;
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i, *partsCount_j+1, *partsCount_k+1);
									}
								}
							}
						}
					}
				}
			}
		}else{
			if(!remainUnderArray_j){
				if(!remainUnderArray_k){
					for(ip=0; ip<*partsCount_i+1; ip++){
						for(jp=0; jp<*partsCount_j; jp++){
							for(kp=0; kp<*partsCount_k; kp++){
								//По i-не норм, по j-норм, по k-норм
								if(ip==*partsCount_i){
									//остаточный по i
									block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArraySize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArraySize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k);
								}else{
									//обычный
									block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
									i_arf = ip**underArraySize_i;
									i_art = (ip**underArraySize_i)+*underArraySize_i;
									j_arf = jp**underArraySize_j;
									j_art = (jp**underArraySize_j)+*underArraySize_j;
									k_arf = kp**underArraySize_k;
									k_art = (kp**underArraySize_k)+*underArraySize_k;
									printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k);
								}
							}
						}
					}
				}else{
					for(ip=0; ip<*partsCount_i+1; ip++){
						for(jp=0; jp<*partsCount_j; jp++){
							for(kp=0; kp<*partsCount_k+1; kp++){
								//По i-не норм, по j-норм, по k-не норм
								if(ip==*partsCount_i){
									if(kp==*partsCount_k){
										//остаточный по i и k
										block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k+1);
									}else{
										//остаточный по i
										block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k+1);
									}
								}else{
									if(kp==*partsCount_k){
										//остаточный по k
										block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k+1);
									}else{
										//обычный
										block_id = ip+jp*(*partsCount_i+1)+kp**partsCount_j*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j, *partsCount_k+1);
									}
								}
							}
						}
					}
				}
			}else{
				if(!remainUnderArray_k){
					for(ip=0; ip<*partsCount_i+1; ip++){
						for(jp=0; jp<*partsCount_j+1; jp++){
							for(kp=0; kp<*partsCount_k; kp++){
								//По i-не норм, по j-не норм, по k-норм
								if(ip==*partsCount_i){
									if(jp==*partsCount_j){
										//остаточный по i и j
										block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k);
									}else{
										//остаточный по i
										block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k);
									}
								}else{
									if(jp==*partsCount_j){
										//остаточный по j
										block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k);
									}else{
										//обычный
										block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
										i_arf = ip**underArraySize_i;
										i_art = (ip**underArraySize_i)+*underArraySize_i;
										j_arf = jp**underArraySize_j;
										j_art = (jp**underArraySize_j)+*underArraySize_j;
										k_arf = kp**underArraySize_k;
										k_art = (kp**underArraySize_k)+*underArraySize_k;
										printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k);
									}
								}
							}
						}
					}
				}else{
					for(ip=0; ip<*partsCount_i+1; ip++){
						for(jp=0; jp<*partsCount_j+1; jp++){
							for(kp=0; kp<*partsCount_k+1; kp++){
								//По i-не норм, по j-не норм, по k-не норм
								if(ip==*partsCount_i){
									if(jp==*partsCount_j){
										if(kp==*partsCount_k){
											//остаточный по i и j и k
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}else{
											//остаточный по i и j
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArraySize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}
									}else{
										if(kp==*partsCount_k){
											//остаточный по i и k
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArraySize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}else{
											//остаточный по i
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArrayLastSize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArraySize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArraySize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}
									}
								}else{
									if(jp==*partsCount_j){
										if(kp==*partsCount_k){
											//остаточный по j и k
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArraySize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}else{
											//остаточный по j
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArraySize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArrayLastSize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArraySize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *** *** *\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}
									}else{
										if(kp==*partsCount_k){
											//остаточный по k
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArraySize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArraySize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArrayLastSize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d)[%d/%d/%d]*** *** *** *** *** *** *** *** **\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}else{
											//обычный
											block_id = ip+jp*(*partsCount_i+1)+kp*(*partsCount_j+1)*(*partsCount_i+1);
											i_arf = ip**underArraySize_i;
											i_art = (ip**underArraySize_i)+*underArraySize_i;
											j_arf = jp**underArraySize_j;
											j_art = (jp**underArraySize_j)+*underArraySize_j;
											k_arf = kp**underArraySize_k;
											k_art = (kp**underArraySize_k)+*underArraySize_k;
											printf("block_id :%d; i_ar:(%d:%d); j_ar:(%d:%d); k_ar:(%d:%d); (%d/%d/%d):[%d/%d/%d]*** *** *** *** *** *** *** *** ***\n", block_id, i_arf, i_art, j_arf, j_art, k_arf, k_art, ip, jp, kp, *partsCount_i+1, *partsCount_j+1, *partsCount_k+1);
										}
									}
								}
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
void processMpiCheckArrays(int **firstArray,  int **firstUnderArray,  int *firstPartsCount_i,  int *firstPartsCount_j,  int *firstPartsCount_k,  int *firstUnderArraySize_i,  int *firstUnderArraySize_j,  int *firstUnderArraySize_k,  int *firstUnderArrayLastSize_i,  int *firstUnderArrayLastSize_j,  int *firstUnderArrayLastSize_k,  bool *firstRemainUnderArray_i,  bool *firstRemainUnderArray_j,  bool *firstRemainUnderArray_k,
		                   int **secondArray, int **secondUnderArray, int *secondPartsCount_i, int *secondPartsCount_j, int *secondPartsCount_k, int *secondUnderArraySize_i, int *secondUnderArraySize_j, int *secondUnderArraySize_k, int *secondUnderArrayLastSize_i, int *secondUnderArrayLastSize_j, int *secondUnderArrayLastSize_k, bool *secondRemainUnderArray_i, bool *secondRemainUnderArray_j, bool *secondRemainUnderArray_k,
		                   int sizei, int sizej, int sizek, int world_rank, int world_size){
	if(world_rank==0){
		IMPI_Split_Array(firstArray,firstUnderArray, firstPartsCount_i, firstPartsCount_j, firstPartsCount_k, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArraySize_k, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j, firstUnderArrayLastSize_k, sizei, sizej, sizek, world_rank, world_size);

		//Не правильно формируется!!! underLastArray [0,0,32767]

//		IMPI_Determine_UnderArraysStripes(firstArray, firstUnderArray, firstPartsCount_i, firstPartsCount_j, firstPartsCount_k, true, false, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArraySize_k, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j, firstUnderArrayLastSize_k, firstRemainUnderArray_i, firstRemainUnderArray_j, firstRemainUnderArray_k, sizei, sizej, sizek, world_size);
//		IMPI_PrintArraysInfo(firstArray, firstUnderArray, firstPartsCount_i, firstPartsCount_j, firstPartsCount_k, firstUnderArraySize_i, firstUnderArraySize_j, firstUnderArraySize_k, firstUnderArrayLastSize_i, firstUnderArrayLastSize_j, firstUnderArrayLastSize_k, firstRemainUnderArray_i, firstRemainUnderArray_j, firstRemainUnderArray_k, sizei, sizej, sizek, world_rank);

		//----------------------Для второго массива------------------------------------------------
		// Для второго массива необходоимо будет потом доделать!!!!!!!!
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
	int firstPartsCount_k;
	int *firstUnderArray;
	int firstUnderArraySize_i;
	int firstUnderArraySize_j;
	int firstUnderArraySize_k;
	int firstUnderArrayLastSize_i;
	int firstUnderArrayLastSize_j;
	int firstUnderArrayLastSize_k;
	bool firstRemainUnderArray_i;
	bool firstRemainUnderArray_j;
	bool firstRemainUnderArray_k;

	int *secondArray;
	int secondPartsCount_i;
	int secondPartsCount_j;
	int secondPartsCount_k;
	int *secondUnderArray;
	int secondUnderArraySize_i;
	int secondUnderArraySize_j;
	int secondUnderArraySize_k;
	int secondUnderArrayLastSize_i;
	int secondUnderArrayLastSize_j;
	int secondUnderArrayLastSize_k;
	bool secondRemainUnderArray_i;
	bool secondRemainUnderArray_j;
	bool secondRemainUnderArray_k;

	int sizei=502;
	int sizej=503;
	int sizek=504;
	int *resultArray;

	IMPI_Init(&world_size, &world_rank, &processor_name, &name_len);
	processSimpleCheckArrays(&firstArray, &secondArray, &resultArray, sizei, sizej, sizek, world_rank);

	processMpiCheckArrays(&firstArray,  &firstUnderArray,  &firstPartsCount_i,  &firstPartsCount_j,  &firstPartsCount_k,  &firstUnderArraySize_i,  &firstUnderArraySize_j,  &firstUnderArraySize_k,  &firstUnderArrayLastSize_i,  &firstUnderArrayLastSize_j,  &firstUnderArrayLastSize_k,  &firstRemainUnderArray_i,  &firstRemainUnderArray_j,  &firstRemainUnderArray_k,
			              &secondArray, &secondUnderArray, &secondPartsCount_i, &secondPartsCount_j, &secondPartsCount_k, &secondUnderArraySize_i, &secondUnderArraySize_j, &secondUnderArraySize_k, &secondUnderArrayLastSize_i, &secondUnderArrayLastSize_j, &secondUnderArrayLastSize_k, &secondRemainUnderArray_i, &secondRemainUnderArray_j, &secondRemainUnderArray_k,
			              sizei, sizej, sizek, world_rank, world_size);

	MPI_Finalize();
}
