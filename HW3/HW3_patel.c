// Amar Patel
//CSCI 4229 - Computer Graphics

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*
-------------  m          Switch the Perspective Mode (Perspective or Orthogonal)

-------------  +/-          Controls our field of view

-------------  Arrow Keys    Move around the scene and chaging angles as you go

-------------  i/d        Increase or Decrease the zoom perspective
-------------  0          Reset overall view, will clear the view matrix
-------------  ESC        Leave the program
*/

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim= 9.0;   //  Size of world

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
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

/*
 *  Set projection
 */
static void Project()
{
   //Update initial matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
  // Whatever the mode is currently in lets update the matrix so we can view the scene in that display
   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();
}


static void buildingFrame(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)

{

  const double var1 = 2.5;
  const double var2 = 2;
  const double varW = 2;
  const double var4 = 2;

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBegin(GL_QUADS);

   glColor3f(0,77,255);

   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);

   glColor3f(0,77,255);

   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);

   glColor3f(244, 238, 65);

   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);

   glColor3f(244, 238, 65);

   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);

   glColor3f(0,77,255);

   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);

   glColor3f(0,77,255);

   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);

   glEnd();


  glBegin(GL_QUADS);

  glColor3ub(244,164,96);

  glVertex3f(2.5,-1,-0.8);
  glVertex3f(1,-1,-0.8);
  glVertex3f(1,0.8,-0.8);
  glVertex3f(2.5,0.8,-0.8);

  glColor3ub(244,164,96);

  glVertex3f(1,-1,0.8);
  glVertex3f(2.5,-1,0.8);
  glVertex3f(2.5,1,0.8);
  glVertex3f(1,1,0.8);

  glColor3ub(218,165,32);

  glVertex3f(2.5,-1,0.8);
  glVertex3f(2.5,-1,-0.8);
  glVertex3f(2.5,0.8,-0.8);
  glVertex3f(2.5,1,0.8);

  glColor3ub(30,30,30);

  glVertex3f(2.5,1,0.8);
  glVertex3f(1,1,0.8);
  glVertex3f(1,0.8,-0.8);
  glVertex3f(2.5,0.8,-0.8);

  glColor3ub(244,164,96);

  glVertex3f(2.5,-1,-0.8);
  glVertex3f(1,-1,-0.8);
  glVertex3f(1,-1,0.8);
  glVertex3f(2.5,-1,0.8);
  glEnd();



  //Secondary Structure
  glBegin(GL_QUADS);

  glColor3ub(244,164,96);

  glVertex3f(-2.5,-1,-0.8);
  glVertex3f(-1,-1,-0.8);
  glVertex3f(-1,0.8,-0.8);
  glVertex3f(-2.5,0.8,-0.8);

  glColor3ub(244,164,96);


  glVertex3f(-1,-1,0.8);
  glVertex3f(-2.5,-1,0.8);
  glVertex3f(-2.5,1,0.8);
  glVertex3f(-1,1,0.8);

  glColor3ub(218,165,32);

  glVertex3f(-2.5,-1,0.8);
  glVertex3f(-2.5,-1,-0.8);
  glVertex3f(-2.5,0.8,-0.8);
  glVertex3f(-2.5,1,0.8);

  glColor3ub(30,30,30);

  glVertex3f(-2.5,1,0.8);
  glVertex3f(-1,1,0.8);
  glVertex3f(-1,0.8,-0.8);
  glVertex3f(-2.5,0.8,-0.8);

  glColor3ub(244,164,96);


  glVertex3f(-2.5,-1,-0.8);
  glVertex3f(-1,-1,-0.8);
  glVertex3f(-1,-1,0.8);
  glVertex3f(-2.5,-1,0.8);
  glEnd();


  glBegin(GL_QUADS);

  glColor3ub(222,184,135);

  glVertex3f(var2,var4,varW);
  glVertex3f(-var2,var4,varW);
  glVertex3f(-var2,var1,0);
  glVertex3f(var2,var1,0);

  glColor3ub(222,184,135);

  glVertex3f(var2,var4,-varW);
  glVertex3f(-var2,var4,-varW);
  glVertex3f(-var2,var1,0);
  glVertex3f(var2,var1,0);
  glEnd();

  // Sides of Roof
  glBegin(GL_TRIANGLES);

  glColor3ub(245,222,179);

  glVertex3f(var2,var4,-varW);
  glVertex3f(var2,var4,varW);
  glVertex3f(var2,var1,0);

  glColor3ub(245,222,179);

  glVertex3f(-var2,var4,-varW);
  glVertex3f(-var2,var4,varW);
  glVertex3f(-var2,var1,0);
  glEnd();

  // Primary support
  glBegin(GL_QUADS);

  glColor3ub(244,164,96);

  glVertex3f(0.5,1,0.25);
  glVertex3f(0.5,1,-.25);
  glVertex3f(0.5,2,-.25);
  glVertex3f(0.5,2,0.25);

  glColor3ub(244,164,96);

  glVertex3f(1.01,1,0.25);
  glVertex3f(1.01,1,-.25);
  glVertex3f(1.01,2,-.25);
  glVertex3f(1.01,2,0.25);

  glColor3ub(100,0,0);

  glVertex3f(0.5,1,-0.25);
  glVertex3f(0.5,2,-0.25);
  glVertex3f(1,2,-0.25);
  glVertex3f(1,1,-0.25);

  glColor3ub(100,0,0);

  glVertex3f(0.5,1,.25);
  glVertex3f(0.5,2,.25);
  glVertex3f(1,2,.25);
  glVertex3f(1,1,.25);

  glColor3ub(10,10,10);

  glVertex3f(0.5,2,0.25);
  glVertex3f(0.5,2,-0.25);
  glVertex3f(1,2,-0.25);
  glVertex3f(1,2,0.25);
  glEnd();


  // Secondary Support
  glBegin(GL_QUADS);

  glColor3ub(244,164,96);

  glVertex3f(-0.5,1,0.25);
  glVertex3f(-0.5,1,-.25);
  glVertex3f(-0.5,2,-.25);
  glVertex3f(-0.5,2,0.25);

  glColor3ub(244,164,96);

  glVertex3f(-1.01,1,0.25);
  glVertex3f(-1.01,1,-.25);
  glVertex3f(-1.01,2,-.25);
  glVertex3f(-1.01,2,0.25);

  glColor3ub(100,0,0);

  glVertex3f(-0.5,1,-0.25);
  glVertex3f(-0.5,2,-0.25);
  glVertex3f(-1,2,-0.25);
  glVertex3f(-1,1,-0.25);

  glColor3ub(100,0,0);

  glVertex3f(-0.5,1,.25);
  glVertex3f(-0.5,2,.25);
  glVertex3f(-1,2,.25);
  glVertex3f(-1,1,.25);

  glColor3ub(10,10,10);

  glVertex3f(-0.5,2,0.25);
  glVertex3f(-0.5,2,-0.25);
  glVertex3f(-1,2,-0.25);
  glVertex3f(-1,2,0.25);
  glEnd();


  glBegin(GL_QUADS);

  glColor3ub(240,255,240);

  glVertex3f(0.2,-1,-1.01);
  glVertex3f(-0.2,-1,-1.01);
  glVertex3f(-0.2,0,-1.01);
  glVertex3f(0.2,0,-1.01);

  glColor3ub(240,255,240);

  glVertex3f(2.3,-1,-0.81);
  glVertex3f(1.2,-1,-0.81);
  glVertex3f(1.2,0.5,-0.81);
  glVertex3f(2.3,0.5,-0.81);

  glColor3ub(186,85,211);

  glVertex3f(0.5,-1,1.01);
  glVertex3f(-0.5,-1,1.01);
  glVertex3f(-0.5,0.5,1.01);
  glVertex3f(0.5,0.5,1.01);
  glEnd();

   glPopMatrix();
}

void display()
{
   const double len=6;

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glEnable(GL_DEPTH_TEST);

   glLoadIdentity();
   //  Perspective Orientation
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   //  Orthogonal Orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }


   glColor3f(1,1,1);
   //  Draw axes
   if (axes){
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   buildingFrame(-3,-0.5,2,1,0.5,1,60);

   buildingFrame(5,-0.5,5,0.6,0.6,0.6,270);

   buildingFrame(2,-0.5,1,0.2,0.6,0.2,270);

   glColor3ub(0,100,0);
   glBegin(GL_QUADS);
   glVertex3f(-100,-1,-100);
   glVertex3f(-100,-1,100);
   glVertex3f(100,-1,100);
   glVertex3f(100,-1,-100);
   glEnd();


   //  Display parameters

   glWindowPos2i(5,550);

   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}


void special(int key,int x,int y)
{
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   else if (key == GLUT_KEY_UP)
      ph += 5;
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   th %= 360;
   ph %= 360;
   Project();
   glutPostRedisplay();
}

void key(unsigned char ch,int x,int y)
{
   if (ch == 27)
      exit(0);

   else if (ch == '0')
      th = ph = 0;

   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if(ch == 'a' || ch == 'A')
      axes = 1 - axes;

   // increase our dim variable
   else if (ch == 'i')
      dim += 0.2;

   // decrease our dim variable

   else if (ch == 'd' && dim>1)
      dim -= 0.2;
   Project();
   glutPostRedisplay();
}

void reshape(int width,int height)
{

   asp = (height>0) ? (double)width/height : 1;

   glViewport(0,0, width,height);

   Project();
}

int main(int argc,char* argv[])
{

   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   glutInitWindowSize(600,600);
   glutCreateWindow("Amar Patel First 3D Scene");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutMainLoop();

   return 0;
}
