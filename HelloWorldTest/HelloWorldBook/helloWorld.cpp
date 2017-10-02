#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include "StringNew.h"

using namespace std;

const int maxLen = 255;

class myString{
public: 
	explicit myString(int n){
		s = new char[n + 1];
		len = n;
	}
	explicit myString(){
		s = new char[1];
		len = 0;
	}
	friend myString& operator+(const myString& a, const myString& b);
	~myString(){
		delete []s;
	}
	void assign(const char* str){
		delete [] s;
		len = strlen(str);
		s = new char[len + 1];
		strcpy(s, str);
	}
	int length() const { 
		return len;
	}
	void print() const {
		std::cout << s << "\nLength: " << len << std::endl;
	}
	void print (int n)const{
		for(int i=0; i<n; ++i){
			cout << s << endl;
		}
	}
private:
	char* s;
	int len;
};

//Перегрузка
myString& operator+(const myString& a, const myString& b){

	myString* temp = new myString(a.len + b.len);
	strcpy(temp->s, a.s);
	strcat(temp->s, b.s);
	return *temp;
}
inline void prMessage(string s = "Hello World!!!"){
	//Hello World!
	cout <<s << endl;
}
/*
int main(){
	prMessage("Hello fucking world!:(");

	myString firstString(40), 
		secondString(100), 
		bothString(50),
		thirdString(40), 
		nullString();
	char charArray[40] = {
		"My name is Charles Babydge"
	};
	firstString.assign("My name is Sergo");
	secondString.assign(charArray);
	thirdString.assign("Hello String Three!");
	
	bothString = firstString + secondString;

	bothString.print(5);

	if(firstString.length() < secondString.length())
		firstString.print();
	else
		secondString.print();

}
	*/
