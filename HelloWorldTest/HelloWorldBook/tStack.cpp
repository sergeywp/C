#include <iostream>
#include <string>
#include "StringNew.h"

using namespace std;

template  <class TYPE>
class stack{
public:
	explicit stack(int n){
		if(n<1)
			throw(n);
		char* p = new char[n];
		if(p==0)
			throw ("MEMORY OVER");
		top = EMPTY;
		maxLen = n;
	}
	
	~stack(){
		delete []s;
	}
	void reset(){
		top = EMPTY;
	}
	void push(TYPE c){
		s[++top] = c;
	}
	TYPE pop(){ 
		return s[top--];
	}
	TYPE topOf(){
		return s[top];
	}
	bool empty(){
		return (top == EMPTY);
	}
	bool full(){
		return (top == maxLen -1);
	}
private:
	
	enum{EMPTY = -1};
	TYPE* s;
	int maxLen;
	int top;

};

int main(){

	try{
		stack<int> a(3), b(5);
		cout << "READY!"<<endl;
	}catch(int n){
		cout << "ERROR MOTHERFUCKER: "<< n << endl;
	}catch(char* error){
		cout << error << endl;
	}
}

