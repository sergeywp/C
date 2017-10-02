#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <iostream>

using namespace std;

int main(){
	cudaDeviceProp prop;

	int count;

	cudaGetDeviceCount(&count);

	for(int i=0; i<count; i++){
		cudaGetDeviceProperties(&prop,i);
		cout<<"---Some Information for the Device---"<<endl;
		cout<<"Name : " << prop.name << endl;
		cout<<"Compute capability : " << prop.major << "."<< prop.minor << endl;
		cout<<"Clock Rate : " << prop.clockRate << endl;
	}
}