enum support { ta, ra, fellowship, other }; // тип финансовой поддержки
enum year { frsh, soph, junior, senior, grad }; //курс обучения

class student{
public:
	student(char* name, int id, double g, year x);
	void print() const;
private:
	int studentId;
	double gpa;
	year y;
	char name[30];
};

class gradStudent : public student { //Класс аспирантов
public: 
	gradStudent(char* name, int id, double g, year x, support t, char*d, char* th);
	void print() const;
private:
	support s;
	char dept[10];
	char thesis[80];
};