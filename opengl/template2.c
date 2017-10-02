//для компиляции кода нужно набрать g++ main.c -lglut -lGL -o progGL

#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

//функция перевода int в string
std::string to_string(int val) {
	char buff[32];
	sprintf(buff,"%d",val);
	return std::string(buff);
}

//функция перевода float в string
std::string to_stringf(float val) {
	char buff[32];
	sprintf(buff,"%0.2f",val); //"%0*.*f\n
	return std::string(buff);
}

//функция вызывающаяся при смене видимости окна приложения
void Visibility(int state) // Visibility function
{
	 if (state == GLUT_NOT_VISIBLE) {printf("Window not visible!\n");
		for(int i=0; i<9; i++)
	  		printf("%s , %d\n",res, result[i]);	}
			if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

//функция рисования текста
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
	glRotatef(alpha, 0.5f, 0.5f, 0.5f);

	//рисование текста здесь
	drawText(to_string(second).data(),to_string(second).size(),10,25);

	glEnd();
	glPopMatrix();
	glFlush();  
	glutSwapBuffers();  
}

//Запись в файл
void writefile(int value)
{
	
}

//функция по таймеру
void timeplus(int value)
{
	/* код функции*/
	glutTimerFunc(1000, timeplus, 0); 
}

//функция обработчика нажатия клавиш
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(20, 1050 - 480 - 20);
	glutCreateWindow("Djedai test");

	glClearColor(0, 0, 0, 1.0);
	glLoadIdentity();
	glOrtho (-400, 400, -400, 400, -400, 400);
	glutDisplayFunc(display);
	
	//функция вызова функций - в данном случае обработчик нажатия клавиш
	glutSpecialFunc(keyEvent);
	//функция видимости
	glutVisibilityFunc(Visibility); // Set up visibility funtion
	//функция вызывающаяся по таймеру
	glutTimerFunc(1000, timeplus, 0); 

	glutMainLoop();
}
