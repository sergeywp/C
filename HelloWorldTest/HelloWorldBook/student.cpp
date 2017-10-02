enum support { ta, ra, fellowship, other }; // ��� ���������� ���������
enum year { frsh, soph, junior, senior, grad }; //���� ��������

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

class gradStudent : public student { //����� ����������
public: 
	gradStudent(char* name, int id, double g, year x, support t, char*d, char* th);
	void print() const;
private:
	support s;
	char dept[10];
	char thesis[80];
};