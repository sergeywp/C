class shape{
public:
	virtual double area() = 0;
};

class rectangle : public shape {
public: 
	rectangle(double h, double w) :
		height(h), width(w){}
	double area(){
		return (height * width);
	}
private:
	double height, width;
};

class circle : public shape {
public: 
	circle(double r) : radius(r) { }
	double area() {
		return (3.14159 * radius * radius );
	}
private:
	double radius;
};

class square : public shape {
public:
	square(double sideLengthConstr) : sideLength(sideLengthConstr) {}
	double area() {
		return sideLength * sideLength;
			
	}
private:
	double sideLength;
};