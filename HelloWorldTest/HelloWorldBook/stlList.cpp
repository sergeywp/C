#include <iostream>
#include <list>
#include <numeric>

using namespace std;

void print(const list<double> &lst){
	list<double>::const_iterator where;

	for(where = lst.begin(); where !=lst.end(); ++where)
		cout << *where << '/t'<<endl;;
	cout << endl;
}

int main(){
	double doubleArray[4] = {0.9, 0.8, 88, -99.99};
	list<double> doubleList;
	for(int i=0; i<4; ++i)
		doubleList.push_front(doubleArray[i]);
	print(doubleList);
	doubleList.sort();
	print(doubleList);
	cout << "Summ equals "<< accumulate(doubleList.begin(), doubleList.end(), 0.0) << endl;
}