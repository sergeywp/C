// OpenGl_test.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <windows.h>
#include <glut.h>   

using namespace std;

void Initialize()
{
	//������� ������� (���������) ����
	glClearColor(1.0,0.0,1.0,1.0);
	//���������� ��������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
}

void Draw()
{
	//������� ����� 
	glClear(GL_COLOR_BUFFER_BIT);
	
	//��������� �������� 
	glColor3f(1.0,1.0,1.0); //�������� ����� ����
	glBegin(GL_POLYGON);
	glVertex3f(0.25,0.25,0.0); //���������� ��������
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
	glutInitWindowSize(400,400);            //��������� ������ ����
	glutInitWindowPosition(100,100);        //������� ����
	glutCreateWindow("Polygon");            //��� ����
	Initialize();                                           //����� ������� Initialize
	glutDisplayFunc(Draw);                          //����� ������� ���������
	
	glutMainLoop();
	return 0;
}

