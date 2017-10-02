// HelloWorldTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector> 


int _tmain(int argc, _TCHAR* argv[]){

	std::string line;
	std::ifstream myfile ("C:\\Users\\Сергей\\Documents\\visual studio 2012\\Projects\\HelloWorldTest\\HelloWorldTest\\data.dat");
	if (myfile.is_open()){

			double sum=0;
			std::vector<double>numberlist; 
			double number = 0 ;      
			while(myfile >> number){
				numberlist.push_back(number);
				sum+=number;
				std::cout <<number<<"/"<<sum<< '\n';
		}

	}

	return 0;
}

