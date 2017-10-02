#include <iostream>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float x1_ = -20.;
float y1_ = 0.;
float x2_ = 20.;
float y2_ = 0.;
std::string str;
std::string to_string(int val) {
 char buff[32];
 sprintf(buff,"%d",val);
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
	str += sx2;
	str += ",";	
	str += sy1;
	str += ",";
	str += sy2;
	str += ";";
	return str;
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

void display()
{

  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex2f(x1_,y1_);
  glVertex2f(x2_,y2_);
  glEnd();
  str = sText();
  drawText(str.data(),str.size(),10, 10);
  glutSwapBuffers();
  
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
}
}

void timer(int = 0)
{
  display();
  glutTimerFunc(10, timer, 0);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(20, 1050 - 480 - 20);
  glutCreateWindow("Points");
  glClearColor(0, 0, 0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-400, 400, -400, 400, -400, 400);
  glutDisplayFunc(display);
  glutSpecialFunc(keyEvent);
  timer();
  glutMainLoop();
}
