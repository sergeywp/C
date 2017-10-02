#include <iostream>
const char BELL = '\a';

void ring(){
	std::cout << BELL;
}

int main(){
	ring();
}