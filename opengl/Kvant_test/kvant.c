#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int WindW, WindH;
int i,id,jd;
int alpha;
float wight, yy, zz, xxd, yyd, zzd;
float xx = 1.0;
float pi = 3.1415926535897932384626433832795;
FILE *file;
char r[]="";
int result[9]={0,0,0,0,0,0,0,0,0};
char res[]="";

void quad_draw(float du, float nx, float ny){
xx=0.002f;
   glBegin(GL_POLYGON);
   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*(ny-1), -sin(0.0f+xx)*100);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*(ny-1), -sin(0.0f+xx)*100);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*ny, sin(0.0f+xx)*100);
   glVertex3f(du*nx+0.3*nx, du*ny+0.3*ny, sin(0.0f+xx)*100);

   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*ny, sin(0.0f+xx)*100);
   glVertex3f(du*nx+0.3*(nx-1), du*ny+0.3*(ny-1), sin(0.0f+xx)*100);
   glEnd();
}


void Reshape(int width, int height) // Reshape function
{
  glViewport(0, 0, 450, 450);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluOrtho2D(-1, 1, -1, 1);
	glOrtho(-1, 1, -1, 1, -1, 1);
  glMatrixMode(GL_MODELVIEW);
}

void draw_string_bitmap(void *font, const char* string) 
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}

void Draw(void) // Window redraw function
{
glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

alpha+=1;
  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
	//Функции рисования здесь!!

	int rand_num = rand() % 9 + 1;
	int ix=0, iy=0;
	//glRotatef(sin(alpha), 0.2f, 0.2f, 0.2f);
	
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
				quad_draw(0.05, i-1, j-1);
		}
	}
	glColor3f(0.0, 1.0, 1.0);
	id=rand() % 3-1;
	jd=rand() % 3-1;
	sprintf(r, "%d", id+2+(jd+1)*3);
	fputs(r, file);
	result[id+2+(jd+1)*3-1]++;
	quad_draw(0.05, id, jd);
	
	glBegin(GL_POINTS);
	draw_string_bitmap(GLUT_BITMAP_HELVETICA_18, "Hello!");
	glRasterPos2i(100,150);
	char text[]="fsdff" ;
        for(int i=0; i<5; i++)
        {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)text[i]);
        }


	glEnd();
  glPopMatrix();

  glFlush();  
  glutSwapBuffers();
}

void Visibility(int state) // Visibility function
{
  if (state == GLUT_NOT_VISIBLE) {printf("Window not visible!\n");
		for(int i=0; i<9; i++)
  		printf("%s , %d\n",res, result[i]);	}
  if (state == GLUT_VISIBLE) printf("Window visible!\n");
}

void timf(int value) // Timer function
{
  glutPostRedisplay();  // Redraw windows
  glutTimerFunc(0.04, timf, 0); // Setup next timer
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

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(-400, 400, -400, 400, -400, 400);
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

void writefile(int value)
{
	printf("прошло 1 минута");
	for(int i=0; i<9; i++)
 		sprintf(res, "%s , %d",res, result[i]);
	fputs(res, file);	
	sprintf(res, "\nМаксимальное число: %d встречалось %d раз.",FindMaxInd(result,9)+1, FindMax(result,9));
	fputs(res, file);
	exit(0);



}

int main(int argc, char *argv[])
{
  WindW = 400;
  WindH = 400;
  alpha = 0; 
	wight = 0.1f;
	
	//подготовка
 	srand (time(NULL));
	file = fopen("log.txt", "w+");
	
	/*
	------------------------------
	для работы со строками:
	-------------------------------
	void writeIntoFile(char* input){
		FILE *file;
		file = fopen("log.txt", "a+");
		fputs(input, file);
		fclose(file);	
	}

	int id, jd;
	id=9;jd=10;
	char buf[3]="";
	char r[] = "FUCKYOU";

	sprintf(r, "%s, %d + %d ",r, id,jd);
	
	writeIntoFile(r);
	*/

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  (void)glutCreateWindow("Тест Джедая");

  glutReshapeFunc(Reshape); // Set up reshape function
  glutDisplayFunc(Draw);    // Set up redisplay function 
  glutTimerFunc(0.04, timf, 0);  // Set up timer for 40ms, about 25 fps
  glutTimerFunc(6000, writefile, 0); 
  glutVisibilityFunc(Visibility); // Set up visibility funtion
  glClearColor(0.0, 0.0, 0.0, 1.0f);	
  glutMainLoop();

		//for(int i=0; i<9; i++)
 		//sprintf(res, "%s , %d\n",res, result[i]);
		//sprintf(res, "\nророро%s , \n",res);
	  	//fputs(res, file);

  fclose(file);

  return 0;
}
