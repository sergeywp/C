#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int WindW, WindH;
int i;
int alpha;
float wight, xx, yy, zz, xxd, yyd, zzd;
float pi = 3.1415926535897932384626433832795;

void Reshape(int width, int height) // Reshape function
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(-1, 1, -1, 1);
	glOrtho(-1, 1, -1, 1, 1, 1);
  glMatrixMode(GL_MODELVIEW);
  
  WindW = width;
  WindH = height;
}

//TODO 0.1)дополнить куб недостающими сторонами
//TODO 0.2)посмотреть как можно осветить сцену
//TODO 1)задать координаты точек углов куба в переменных.
//TODO 2)проверить все ли верно отображается со стандартным поворотом "glRotatef(alpha, 1.5f, 1.5f, 1.5f);"
//TODO 3.1)затем создать функцию поворота с помощью матрицы поворота в трехмерном пространстве 
//       ссылка на источник "https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82%D0%B0  "
//       статья "матрица поворота"
//TODO 3.2)обязательно проверить правильность реализации поворота!!!!!
//TODO 4.1)создать функцию поворота с помощью углов Эйлера в трехмерном пространстве
//       ссылка на источник 1) "https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82%D0%B0  "
//       статья "матрица поворота"
//			 ссылка на источник 2) "https://ru.wikipedia.org/wiki/%D0%A3%D0%B3%D0%BB%D1%8B_%D0%AD%D0%B9%D0%BB%D0%B5%D1%80%D0%B0"
//       статья "углы Эйлера"
//TODO 4.2)обязательно проверить правильность реализации поворота!!!!!
//TODO 4.3)сравнить реализацию поворота через матрицу поворота и через углы Эйлера
//TODO 5.1)изучить повороты через кватерионы
//       ссылка на источник 1) "https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%BF%D0%BE%D0%B2%D0%BE%D1%80%D0%BE%D1%82%D0%B0  "
//       статья "матрица поворота"
//			 ссылка на источник 2) "https://ru.wikipedia.org/wiki/%CA%E2%E0%F2%E5%F0%ED%E8%EE%ED%FB_%E8_%E2%F0%E0%F9%E5%ED%E8%E5_%EF%F0%EE%F1%F2%F0%E0%ED%F1%F2%E2%E0 "
//       статья "кватерионы и вращение пространства"
//TODO 5.2)реализовать функцию поворота через кватерироны
//TODO 5.3)проверить проверить правильность реализации поворота!!!!!
//TODO 5.4)сравнить реализацию поворота через матрицу поворота, через углы Эйлера и через кватерионы
//TODO 6.1)протестировать все три реализации поворотов с различными значениями pi. Задокументировать. Сделать выводы
//TODO 7.2)попытаться разбить куб на множество маленьких кубов для того, чтобы посмотреть каким образом будут себя вести боковые стороны куба.




void Draw(void) // Window redraw function
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glClear(GL_COLOR_BUFFER_BIT);
  //glLineWidth(3);
  //glColor3f(0.0f, 0.6f, 0.9f);

	// рассчет освещения

  //glEnable(GL_LIGHTING);

  // двухсторонний расчет освещения

  //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// автоматическое приведение нормалей к
	// единичной длине
 // glEnable(GL_NORMALIZE);

  glPushMatrix();
  glRotatef(alpha, 0.5f, 0.5f, 0.5f);
  alpha += 1;
  if (alpha > 359) alpha = 0;
  glBegin(GL_QUADS);
  
	glColor3f(0.0f, 0.6f, 0.9f);
	glVertex3f(0.0f, wight, wight);
	glColor3f(0.6f, 0.6f, 0.9f);	
	glVertex3f(0.0f, 0.0f, wight);
	glColor3f(0.9f, 0.6f, 0.9f);	
	//glVertex3f(0.0f, 0.0f, wight);  	
	//glVertex3f(wight, 0.0f, wight);
	glColor3f(0.9f, 0.6f, 0.0f);	 
	glVertex3f(wight, 0.0f, wight); 
	glVertex3f(wight, wight, wight);	

	glColor3f(0.6f, 0.6f, 0.9f);	  
	glVertex3f(0.0f, wight, 0.0f);	
	glColor3f(0.0f, 0.6f, 0.9f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(0.9f, 0.6f, 0.0f);  
	//glVertex3f(0.0f, 0.0f, 0.0f);  
	glVertex3f(wight, 0.0f, 0.0f);
	//glVertex3f(wight, 0.0f, 0.0f);
	glVertex3f(wight, wight, 0.0f);	
  
  	glColor3f(0.0f, 0.6f, 0.9f);
	glVertex3f(0.0f, wight, 0.0f);
	//glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);    
	glColor3f(0.9f, 0.6f, 0.0f);
	//glVertex3f(0.0f, 0.0f, wight);
	glVertex3f(0.0f, 0.0f, wight);
	glVertex3f(0.0f, wight, wight);

	glColor3f(0.6f, 0.0f, 0.9f);
	glVertex3f(wight, wight, 0.0f);
	//glVertex3f(wight, 0.0f, 0.0f); 
	glVertex3f(wight, 0.0f, 0.0f);    
  	glColor3f(0.0f, 0.6f, 0.9f);
	glVertex3f(wight, 0.0f, wight);
	//glVertex3f(wight, 0.0f, wight);
	glVertex3f(wight, wight, wight);

	glColor3f(0.6f, 0.0f, 0.9f);	  
	glVertex3f(wight, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); 
  	glColor3f(0.0f, 0.6f, 0.9f);
	//glVertex3f(0.0f, 0.0f, 0.0f);    
	//glVertex3f(0.0f, 0.0f, wight);
	glVertex3f(0.0f, 0.0f, wight);
	glVertex3f(wight, 0.0f, wight);

	glColor3f(0.6f, 0.0f, 0.9f);	  
	glVertex3f(wight, wight, 0.0f);
	//glVertex3f(0.0f, wight, 0.0f);  
	glVertex3f(0.0f, wight, 0.0f);  
  	glColor3f(0.0f, 0.6f, 0.9f);
	glVertex3f(0.0f, wight, wight);
	//glVertex3f(0.0f, wight, wight);
	glVertex3f(wight, wight, wight);
	//glVertex3f(wight, wight, wight);
	glVertex3f(wight, wight, 0.0f);


  glEnd();
  glPopMatrix();

  glFlush();  
  glutSwapBuffers();
}

void Visibility(int state) // Visibility function
{
  if (state == GLUT_NOT_VISIBLE) printf("Window not visible!\n");
  if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

void timf(int value) // Timer function
{
  glutPostRedisplay();  // Redraw windows
  glutTimerFunc(40, timf, 0); // Setup next timer
}

int main(int argc, char *argv[])
{
  WindW = 400;
  WindH = 300;
  alpha = 0; 
	wight = 0.5f;
 
	

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  (void)glutCreateWindow("Rotation Demo");

  glutReshapeFunc(Reshape); // Set up reshape function
  glutDisplayFunc(Draw);    // Set up redisplay function 
  glutTimerFunc(40, timf, 0); // Set up timer for 40ms, about 25 fps
  glutVisibilityFunc(Visibility); // Set up visibility funtion
  glClearColor(0, 0, 0, 0);

  glutMainLoop();

  return 0;
}
