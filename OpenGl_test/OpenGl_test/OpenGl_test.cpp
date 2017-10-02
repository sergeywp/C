// OpenGl_test.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <windows.h>
#include <glut.h>   

using namespace std;

void Initialize()
{
	//Выбрать фоновый (очищающий) цвет
	glClearColor(1.0,0.0,1.0,1.0);
	//Установить проекцию
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
}

void Draw()
{
	//Очищаем экран 
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Отрисовка квадрата 
	glColor3f(1.0,1.0,1.0); //Выбираем белый цвет
	glBegin(GL_POLYGON);
	glVertex3f(0.25,0.25,0.0); //Координаты квадрата
	glVertex3f(0.75,0.25,0.0); 
	glVertex3f(0.75,0.75,0.0);
	glVertex3f(0.25,0.75,0.0);
	glEnd();
	glFlush();
}

int main(int argc, char **argv)
{
	printf("Hello welcome");
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(400,400);            //Указываем размер окна
	glutInitWindowPosition(100,100);        //Позиция окна
	glutCreateWindow("Polygon");            //Имя окна
	Initialize();                                           //Вызов функции Initialize
	glutDisplayFunc(Draw);                          //Вызов функции отрисовки
	
	glutMainLoop();
	return 0;
}

