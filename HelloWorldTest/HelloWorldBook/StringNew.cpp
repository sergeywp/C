#include <iostream>
const int maxLen = 255;

class myString{
public: 
		void assign(const char* st);
		int length() const { 
			return len;
		}
		void print() const {
			std::cout << s << "\nДлинна: " << len << std::endl;
		}
private:
	char s[maxLen];
	int len;
};