
// Amar Patel
// CSCI 4229 - Computer Graphics
// Import the Header Files
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// In order to replicate the Lorenz System you need these Parameters

 
/*
void get_parameters(){ 

	double s, b, r;
	printf("Enter a Value for S");
	scanf(%e, &s);
}
*/

/*
// Lorenz Parameters  
double s  = 10; // sigma
double b  = 2.6666; // beta
double r  = 28; // rho
*/

int totalPts = 50000; // this was a step given by the intial Lorenz.c file
float pts [50000][3];

static GLfloat xRotation = 25.0;
static GLfloat yRotation = 25.0;
static GLfloat zRotation = 0.0;
static GLfloat zPosition = 60.0;


int iteration = 0;
int iter2 = 10;

int axis_plot =  1;

#define LEN 8192
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;

   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);

   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*ch++);
}

void display()
{
    const double len=100;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glPushMatrix();

    glRotatef(xRotation, 1.0, 0.0, 0.0);
    glRotatef(yRotation, 0.0, 1.0, 0.0);
    glRotatef(zRotation, 0.0, 0.0, 1.0);

    glBegin(GL_LINE_STRIP);
    glColor3fv(pts[30000]);
    int i = 0;

    while( i < iteration  && i < totalPts ) {
        glVertex3fv(pts[i++]);
    }
    glEnd();

    if( iteration  < totalPts ) {
        if( iteration  + iter2 > totalPts ) iteration  = totalPts;
        else iteration +=iter2;
    }

   if (axis_plot)
   {
      glBegin(GL_LINES);
      glColor3f(24, 247, 221);
          glVertex3d(0.0,0.0,0.0);
          glVertex3d(len,0.0,0.0);

          glVertex3d(0.0,0.0,0.0);
          glVertex3d(0.0,len,0.0);

          glVertex3d(0.0,0.0,0.0);
          glVertex3d(0.0,0.0,len);

      glEnd();


      glRasterPos3d(len,0.0,0.0);
      glRasterPos3d(0.0,len,0.0);
      glRasterPos3d(0.0,0.0,len);

   }


   glWindowPos2i(25,25);

   Print("Step=%d",iteration );


   glutSwapBuffers();
   glFlush();

   glPopMatrix();
}

static void reshape(int winHeight, int winWidth)
{
  GLfloat h = (GLfloat) winHeight / (GLfloat) winWidth;
  glViewport(0, 0, (GLint) winWidth, (GLint) winHeight);

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glFrustum(-5.0, 5.0, -h*2, h*2, 1.0, 300.0);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glTranslatef(0.0, 0.0, -60.0);
}


static void key(unsigned char k, int x, int y)
{
       switch (k) {
           case 27:
               exit(0);
               break;

           case 'v':
               xRotation = yRotation = 25.0;
               break;


           case 'r':
               iteration  = 0;
               break;

           case 'q':
               iteration  = totalPts;
               break;

           case 'a':
               iter2 += 5;
               break;

           case 's':
               if( iter2 - 5 >- 0 ) iter2 -= 5;
               break;

           case '+':
               glMatrixMode(GL_MODELVIEW);
               glLoadIdentity();
               zPosition -= 1;
               gluLookAt(0,0,zPosition,0.0,0.0,0.0,0.0,1.0,0.0);
               break;

      
           case '-':
               glMatrixMode(GL_MODELVIEW);
               glLoadIdentity();
               zPosition += 1;
               gluLookAt(0,0,zPosition,0.0,0.0,0.0,0.0,1.0,0.0);
               break;
           default:
               return;
       }
       glutPostRedisplay();
}


static void special(int k, int x, int y)
{
       switch (k) {
           case GLUT_KEY_UP:
               xRotation += 15.0;
               break;

           case GLUT_KEY_DOWN:
               xRotation -= 15.0;
               break;

           case GLUT_KEY_LEFT:
               yRotation += 15.0;
               break;

           case GLUT_KEY_RIGHT:
               yRotation -= 15.0;
               break;

           default:
               return;
       }
     glutPostRedisplay();
}


static void idle(void) {
       iteration += iter2;
       glutPostRedisplay();
}

static void lorenzSystem(double s, double b, double r)
{
    int count;

    float x = pts[0][0] = 1;
    float y = pts[0][1] = 1;
    float z = pts[0][2] = 1;

    // Time step
    float dt = 0.001;

    for (count=0;count < totalPts-1;count++){

      // directly found from Lorenz System Article
      // https://en.wikipedia.org/wiki/Lorenz_system
        float dx = s*(y-x);
        float dy = x*(r-z)-y;
        float dz = x*y - b*z;

        x += dt*dx;
        y += dt*dy;
        z += dt*dz;

        pts[count + 1][0] = x;
        pts[count + 1][1] = y;
        pts[count + 1][2] = z;

      }
}




int main(int argc, char *argv[])
{
	double s, b, r;
	printf("Default Lorenz Parameters: Sigma:10 Beta:2.6666 Rho:28 \n");

	printf("Enter a Value for Sigma \n");
	scanf("%lf", &s);
	printf("Enter a Value for Beta \n ");
	scanf("%lf", &b);
	printf("Enter a Value for Rho \n");
	scanf("%lf", &r);

    lorenzSystem(s,b,r);

      glutInit(&argc,argv);
      glutInitWindowPosition(50, 50);

      glutInitWindowSize(700, 700);
      glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

      glutCreateWindow("Patel Lorenz_system");

      glutIdleFunc(idle);
      glutKeyboardFunc(key);
      glutDisplayFunc(display);
      glutSpecialFunc(special);
      glutReshapeFunc(reshape);

      glutMainLoop();

      return 0;
}
