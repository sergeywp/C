
//для компиляции кода нужно набрать g++ main.c -lglut -lGL -o progGL

#include <GL/glut.h> 
#include <stdio.h>
#include <time.h>

int WindW, WindH;
int alpha;

void quad_draw(float du, float nx, float ny){
   glBegin(GL_POLYGON);
   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*(ny-1), 2.0);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*(ny-1), 2.0);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*ny, 2.0);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*ny, 2.0);

   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*ny, 2.0);
   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*(ny-1), 2.0);
   glEnd();
}

void Draw() //вот и та функция дисплея 
{

   glClear(GL_COLOR_BUFFER_BIT);//очищаем буфер цвета
   glPointSize(2);//устанавливаем размер точки (так как если не установим, то она будет очень маленькой)
//   glBegin(GL_POLYGON);//начинаем рисовать точку (также есть и другие флаги как GL_LINES GL_TRAINGLE (вроде так =)) ) если вы рисуете линии то вам надо указать 2 точки и нарисуются линии
   glColor3f(0.0, 0.0, 0.0);// устанавливаем цвет которым будем рисовать (если вы рисуете много точек или линий то можно пере тем как нарисовать что- либо установить для него цвет, если вам не устраивает цвет то 0 до 1 то можно установить glColor3ub и тогда будут 255 цвета
  /* glVertex2f(0.05, 0.05);
   glVertex2f(0.05, 0.3);
   glVertex2f(0.05, 0.3);
   glVertex2f(0.3, 0.3);
   glVertex2f(0.3, 0.3);   glVertex2f(0.3, 0.05);
   glVertex2f(0.05, 0.05);

			quad_draw(0.5, 1.0, 1.0);
			quad_draw(0.5, 2.0, 1.0);
			quad_draw(0.5, 1.0, 2.0);
			quad_draw(0.5, 2.0, 2.0);
	
*/
	for(int i=-3; i<3; i++){
		for(int j=-3; j<3; j++){
			quad_draw(0.05, i-1, j-1);
		}
	}	
//рисуем точку 2f означает, что мы будем рисовать в 2 координатах и там координаты (а теперь можно пояснить glOrtho если бы мы установили размер экрана 100х100 то и поставили в glOrtho значение glOrtho(0.0, 100, 0.0, 100, -1.0, 1.0); то эту точку можно было бы нарисовать так glVertex2f(25, 25); и она была бы там же (да и если мы написали бы вместо 100 480 она была бы там ) надеюсь разобрались
   //если вам надо много точек то не надо писать заново все с glBegin и тд можно просто написать сюда множество точек и  они нарисуются
   //glEnd();//заканчиваем рисовать
   glFlush();//отрисовываем все что нарисовали

}

void timf(int value) // Timer function
{
  glutPostRedisplay();  // Redraw windows
  glutTimerFunc(40, timf, 0); // Setup next timer
}

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

void Visibility(int state) // Visibility function
{
  if (state == GLUT_NOT_VISIBLE) printf("Window not visible!\n");
  if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

int main(int argc, char **argv)//точка входа приложения
{
WindW = 400;
  WindH = 300;
  alpha = 0; 

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  (void)glutCreateWindow("Rotation Demo");

  glutReshapeFunc(Reshape); // Set up reshape function
  glutDisplayFunc(Draw);    // Set up redisplay function
   glClearColor(1.0, 1.0, 1.0, 1.0);//очищаем окно в белый цвет
   glMatrixMode(GL_PROJECTION);//устанавливаем матрицу проекции
   glLoadIdentity();//сбрасываем матрицу (устанавливаем нулевую)
   glOrtho(0.0, 50.0, 0.0, 50.0, 10.0, 10.0);//устанавливаем ортогональный вид (или как он там называется в скобках значения)
   // которым будет все измеряться то есть первые 2 - это по Х, вторые 2 - это про У и третьи 2 - это по Z. для более подробного понятия 
   // значения от нуля до единицы по Х это будет само окно, то есть  480 (не знаю как объяснить) значение Z не имеет значения так как мы рисуем 2D
   
  glutTimerFunc(40, timf, 0); // Set up timer for 40ms, about 25 fps
  glutVisibilityFunc(Visibility);
   glutMainLoop();//цикл обработки всякого (типа нажатия на клавишу и тд)
}
