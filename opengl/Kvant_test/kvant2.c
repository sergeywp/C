#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctime>
#include <unistd.h>

int WindW, WindH;
int i,id,jd;
int alpha=0;
float alph=0.0;
float wight, yy, zz, xxd, yyd, zzd;
float xx = 1.0;
float pi = 3.1415926535897932384626433832795;
FILE *file;
char r[]="";
char ind[]="123456789";
int result[9]={0,0,0,0,0,0,0,0,0};
char res[500]="";
int second=0;
int allNum=0;
clock_t t0, t1;

float x1_ = 0.;
float y1_ = 0.;
float x2_ = 20.;
float y2_ = 0.;
std::string str;
std::string tim;
std::string to_string(int val) {
	char buff[32];
	sprintf(buff,"%d",val);
	return std::string(buff);
}

std::string to_stringf(float val) {
	char buff[32];
	sprintf(buff,"%0.2f",val); //"%0*.*f\n
	return std::string(buff);
}

std::string sText()
{
	std::string sx1,sx2,sy1,sy2;
	sx1 = to_string(x1_);
	sx2 = to_string(x2_); 
	sy1 = to_string(y1_);
	sy2 = to_string(y2_);
	str = str.empty();
	str += sx1;
	str += ",";
	//str += sx2;
	//str += ",";	
	str += sy1;
	//str += ",";
	//str += sy2;
	str += ";";
	return str;
}

void Visibility(int state) // Visibility function
{
	 if (state == GLUT_NOT_VISIBLE) {printf("Window not visible!\n");
		for(int i=0; i<9; i++)
	  		printf("%s , %d\n",res, result[i]);	}
			if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

int FindMax(int* mass,int count) 
{
	if(count <  1) return 0;
	int max = mass[0];
	for (int i = 1; i < count; i++)
		if (mass[i] > max)
			max = mass[i];
	return max;
}

int FindMaxInd(int* mass,int count) 
{
	if(count <  1) return 0;
	int max = mass[0];
	int ind = 0;
	for (int i = 1; i < count; i++)
		if (mass[i] > max){
			max = mass[i];
			ind = i;
		}
	return ind;
}


void quad_draw(float du, float nx, float ny){
	xx=2.0f;
	glBegin(GL_POLYGON);
	glVertex3f(du*nx+200*(nx-1)+100+x1_, du*ny+200*(ny-1)+100+y1_, y1_);
	glVertex3f(du*nx+200*nx+100+x1_, du*ny+200*(ny-1)+100+y1_, y1_);
	glVertex3f(du*nx+200*nx+100+x1_, du*ny+200*ny+100+y1_, y1_);
	glVertex3f(du*nx+200*nx+100+x1_, du*ny+200*ny+100+y1_, y1_);

	glVertex3f(du*nx+200*(nx-1)+100+x1_, du*ny+200*ny+100+y1_, y1_);
	glVertex3f(du*nx+200*(nx-1)+100+x1_, du*ny+200*(ny-1)+100+y1_, y1_);
	glEnd();
}

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,400,0,400,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i=0; i<length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);

}

//функция рисования
void display()
{
	glClearColor(0.0, 0, sin(alph)/2, 0.0);
	// glClearColor(alph, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	alpha+=1;
	alph+=0.01;
	  
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	  
	//включить вращение
	//1glRotatef(alpha, 0.5f, 0.5f, 0.5f);

	int rand_num = rand() % 9 + 1;
	int ix=0, iy=0;
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
				quad_draw(15, i-1, j-1);
		}
	}

	glColor3f(0.0, 1.0, 1.0);
	id=rand() % 3-1;
	jd=rand() % 3-1;
	allNum++;
	sprintf(r, "%d", id+2+(jd+1)*3);
	fputs(r, file);
	result[id+2+(jd+1)*3-1]++;
	quad_draw(15, id, jd);

	glPointSize(5);
	/*glBegin(GL_POINTS);
	glVertex2f(x1_,y1_);
	glVertex2f(x2_,y2_);
	glEnd();*/
	str = sText();


	//drawText(str.data(),str.size(),10, 10);
	//cout << "time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << endl;
	drawText(str.data(),str.size(),10, 10);

	//tim = to_stringf( (float)(clock() - t0)/100000);
	//drawText(tim.data(),tim.size(),10, 25);
	drawText(to_string(second).data(),to_string(second).size(),10,25);
	drawText("sec",3,50, 25);
	drawText("Press F1 to stop",16,10, 380);
	drawText(to_string(allNum).data(),to_string(allNum).size(), 350, 10);
	drawText("Quantity of generated numbers:",30, 110, 25);

	for(int i=1; i<4; i++){
		for(int j=1; j<4; j++){
			char buffer[]=" ";
			sprintf(buffer, "%d", (j-1)*3+(i-1)+1);
			glColor3f(0.0, 0.5, 0.5);
			drawText(buffer,2,i*100+15, j*100+15);
		}
	}
	//drawText("1",1,115, 115);
	//drawText("1",1,230, 230);
	glEnd();
	glPopMatrix();
	glFlush();  
	glutSwapBuffers();  
}


void writefile(int value)
{
	printf("прошла 1 минута");
	for(int i=0; i<9; i++){
 		sprintf(res, "%s , %d",res, result[i]);}
		printf("\n%s ",res);
	fputs(res, file);	
	sprintf(res, "\nМаксимальное число: %d встречалось %d раз.",FindMaxInd(result,9)+1, FindMax(result,9));
	fputs(res, file);
	system("gedit log.txt");
	exit(0);
}

void timeplus(int value)
{
	second++;
	glutTimerFunc(1000, timeplus, 0); 
}


void keyEvent(int key, int, int)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		x1_ -= 5.;
		x2_ -= 5.;
		break;
		case GLUT_KEY_UP:
		y1_ += 5;
		y2_ += 5; 
		break;
		case GLUT_KEY_RIGHT:
		x1_ += 5.;
		x2_ += 5.;
		break;
		case GLUT_KEY_DOWN:
		y1_ -= 5;
		y2_ -= 5;    
		break;
		case GLUT_KEY_F1:
		for(int i=0; i<9; i++){
			sprintf(res, "%s , %d",res, result[i]);
		}
		fputs(res, file);	
		sprintf(res, "\nМаксимальное число: %d встречалось %d раз.",FindMaxInd(result,9)+1, FindMax(result,9));
		fputs(res, file);
		fclose(file);
		system("gedit log.txt");
		exit(0);
   		break;

	}
}

void timer(int = 0)
{
	glutPostRedisplay();
	glutTimerFunc(0.04, timer, 0);
}

int main(int argc, char **argv)
{
	//подготовка
	t0 = clock();
	srand (time(NULL));
	file = fopen("log.txt", "w+");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(20, 1050 - 480 - 20);
	glutCreateWindow("Djedai test");

	glClearColor(0, 0, 0, 1.0);
	glLoadIdentity();
	glOrtho (-400, 400, -400, 400, -400, 400);
	glutDisplayFunc(display);
	glutSpecialFunc(keyEvent);
	glutVisibilityFunc(Visibility); // Set up visibility funtion
	//timer();
	glutTimerFunc(0.04, timer, 0); 
	glutTimerFunc(20000, writefile, 0); 
	glutTimerFunc(1000, timeplus, 0); 

	glutMainLoop();
	fclose(file);
}
