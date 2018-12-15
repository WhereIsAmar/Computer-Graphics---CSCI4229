// Amar Patel
//CSCI 4229 - Computer Graphics
// Homework Six with Textures

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

#include "CSCIx229.h"

/*
-------------  m          Switch the Perspective Mode (Perspective or Orthogonal or First Person)

-------------  +/-          Controls our field of view

-------------  Arrow Keys    Move around the scene and chaging angles as you go
-------------- G,V,B,N keys  When in first person will allow one to move around
-------------  i/j          Increase or Decrease the Dim
-------------  0            Reset overall view, will clear the view matrix
-------------  ESC          Leave the program
-------------- X            Will display the axes

Light Controls
-------------- a/A - decrease & increase ambient light
-------------- d/D - decrease & increase diffusion light
-------------- s/S - decrease & increase specular light
-------------- e/E - decrease & increase emitted light
-------------- n/N - decrease & increase shininess
-------------- k/K - decrease & increase the elevation of the light
-------------- < > move the light once q is pressed


Toggling Light:
-------------- q Pause the light and then be able to move it
-------------- l Enable and disable light
*/

int axes = 0;       //  Display axes
int mode = 0;       //  Projection mode
int move = 1;       //  Light movement
int th = 0;         //  Azimuth of view angle
int ph = 10;        //  Elevation of view angle
int fov = 55;       //  Field of view (for perspective)
int lightSwitch = 1;      //  Whether light is on or off
double asp = 1;     //  Aspect ratio
double dim = 20.0;   // Size of World



// Light values taken from Example 15, slightly modified to better reflect my environmenet

int one       =   1;  // Unit value
int distance  =   8;  // Light distance
int inc       =  8;  // speed of ball movement
int smooth    =   1;  // shading variable
int local     =   0;  // Local Viewer Model
int emission  =   5;  // Emission intensity (%)
int ambient   =  15;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh =  90;  // Light azimuth
float ylight  =  3;  // Elevation of light


unsigned int texture[7]; // Texture names



float corX, corY, corZ;   // Current Position
float currX, currY, currZ; // Keep track of this while running the program
float rot1, rot2;
float degreeLx, degreeLz;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))




static void crossHelper(double v1_i, double v1_j, double v1_k,
                         double v2_i, double v2_j, double v2_k)
{
  double product_i = v1_j * v2_k - v1_k * v2_j;
  double product_j = v1_k * v2_i - v1_i * v2_k;
  double product_k = v1_i * v2_j - v1_j * v2_i;
  glNormal3d(product_i,product_j,product_k);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =  Sin(ph);
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void ball(double x,double y,double z,double r) {
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};

  glPushMatrix();

  glTranslated(x,y,z);
  glScaled(r,r,r);


  glColor3f(1,1,1);   // Make the white ball
  glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);


  for (ph=-90;ph<90;ph+=inc) {
    glBegin(GL_QUAD_STRIP);

  for (th=0;th<=360;th+=2*inc) {
    Vertex(th,ph);
    Vertex(th,ph+inc);
  }
  glEnd();
  }

  glPopMatrix();
}

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
      gluPerspective(fov,asp,.001/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
  // Whatever the mode is currently in lets update the matrix so we can view the scene in that display
   glMatrixMode(GL_MODELVIEW);

   glLoadIdentity();
}


static void buildingFrame(double x,double y,double z,double dx,double dy,double dz,double th)

{

 float white[] = {1,1,1,1};
 float black[] = {0,0,0,1};

 //Have to implement these so that the objects are actually seen by the lighting
 // If you do not implement lighting properties the shading will be off
 glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  const double var1 = 1.5;
  const double var2 = 1;
  const double varW = 1;
  const double var4 = 1;

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[0]);

   glBegin(GL_QUADS);

   // front face
   //glColor3f(26,71,148);
   //glColor3ub(245,222,179);


   crossHelper(-2,0,0,0,2,0);
   glTexCoord3f(0,0,0); glVertex3f(1,-1,-1);
   glTexCoord3f(1,0,0); glVertex3f(-1,-1,-1);
   glTexCoord3f(1,1,0); glVertex3f(-1,1,-1);
   glTexCoord3f(0,1,0); glVertex3f(1,1,-1);

   // back face
   //glColor3f(0,77,255);
   //glColor3f(26,71,148);

   glNormal3f(0, 0, 1);
   glTexCoord3f(0,0,0); glVertex3f(-1,-1,1);
   glTexCoord3f(1,0,0); glVertex3f(1,-1,1);
   glTexCoord3f(1,1,0); glVertex3f(1,1,1);
   glTexCoord3f(0,1,0); glVertex3f(-1,1,1);

   // right face
   //glColor3f(244, 238, 65);
   //glColor3f(26,71,148);
   glNormal3f(+1, 0, 0);

   glTexCoord3f(0,0,0); glVertex3f(1,-1,1);
   glTexCoord3f(1,0,0); glVertex3f(1,-1,-1);
   glTexCoord3f(1,1,0); glVertex3f(1,1,-1);
   glVertex3f(1,1,1);

   // left face
   //glColor3f(244, 238, 65);
   //glColor3f(26,71,148);

   glNormal3f(-1, 0, 0);
   glTexCoord3f(0,0,0); glVertex3f(-1,-1,-1);
   glTexCoord3f(1,0,0); glVertex3f(-1,-1,1);
   glTexCoord3f(1,1,0); glVertex3f(-1,1,1);
   glTexCoord3f(0,1,0); glVertex3f(-1,1,-1);


   // top face
   //glColor3f(0,77,255);
   //glColor3f(26,71,148);
   glColor3ub(245,222,179);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,1,1);
   glVertex3f(1,1,1);
   glVertex3f(1,1,-1);
   glVertex3f(-1,1,-1);
   glEnd();

   //bottom
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-one, 0);
   glTexCoord3f(0,0,0); glVertex3f(-1,-1,-1);
   glTexCoord3f(1,0,0); glVertex3f(1,-1,-1);
   glTexCoord3f(1,1,0); glVertex3f(1,-1,1);
   glTexCoord3f(0,1,0); glVertex3f(-1,-1,1);

   glEnd();


   // the other building
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  //front
  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0); glVertex3f(2.5,-1,-0.8);
  glTexCoord3f(1,0,0); glVertex3f(1,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(1,1,-0.8);
  glTexCoord3f(0,1,0); glVertex3f(2.5,1,-0.8);

  //back
  glNormal3f( 0, 0, 1);
  glTexCoord3f(0,0,0); glVertex3f(1,-1,0.8);
  glTexCoord3f(1,0,0); glVertex3f(2.5,-1,0.8);
  glTexCoord3f(1,1,0); glVertex3f(2.5,1,0.8);
  glTexCoord3f(0,1,0); glVertex3f(1,1,0.8);

  //outer

  glNormal3f(+1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(2.5,-1,0.8);
  glTexCoord3f(1,0,0); glVertex3f(2.5,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(2.5,1,-0.8);
  glTexCoord3f(0,1,0); glVertex3f(2.5,1,0.8);

  //top

   glNormal3f(0,+1, 0);
   glTexCoord3f(0,0,0); glVertex3f(2.5,1,0.8);
   glTexCoord3f(1,0,0); glVertex3f(1,1,0.8);
   glTexCoord3f(1,1,0); glVertex3f(1,1,-0.8);
   glTexCoord3f(0,1,0); glVertex3f(2.5,1,-0.8);
   glEnd();

  // lower
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  glColor3ub(244,164,96);
  glNormal3f( 0,-one, 0);
  glTexCoord3f(0,0,0); glVertex3f(2.5,-1,-0.8);
  glTexCoord3f(1,0,0); glVertex3f(1,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(1,-1,0.8);
  glTexCoord3f(0,1,0); glVertex3f(2.5,-1,0.8);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[0]);

  //Secondary Structure
  glBegin(GL_QUADS);
  //front

  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0); glVertex3f(-2.5,-1,-0.8);
  glTexCoord3f(1,0,0); glVertex3f(-1,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(-1,1,-0.8);
  glTexCoord3f(0,1,0); glVertex3f(-2.5,1,-0.8);
  //back

  glNormal3f( 0, 0, 1);
  glTexCoord3f(0,0,0); glVertex3f(-1,-1,0.8);
  glTexCoord3f(1,0,0); glVertex3f(-2.5,-1,0.8);
  glTexCoord3f(1,1,0); glVertex3f(-2.5,1,0.8);
  glTexCoord3f(0,1,0); glVertex3f(-1,1,0.8);
  //outer

  glNormal3f(-1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(-2.5,-1,0.8);
  glTexCoord3f(1,0,0); glVertex3f(-2.5,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(-2.5,1,-0.8);
  glTexCoord3f(0,1,0); glVertex3f(-2.5,1,0.8);

  //top


  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glTexCoord3f(0,0,0); glVertex3f(-2.5,1,0.8);
  glTexCoord3f(1,0,0); glVertex3f(-1,1,0.8);
  glTexCoord3f(1,1,0); glVertex3f(-1,1,-0.8);
  glTexCoord3f(0,1,0); glVertex3f(-2.5,1,-0.8);
  glEnd();


  //bottom 
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);
  glNormal3f( 0,-one, 0);
  glTexCoord3f(0,0,0); glVertex3f(-2.5,-1,-0.8);
  glTexCoord3f(1,0,0); glVertex3f(-1,-1,-0.8);
  glTexCoord3f(1,1,0); glVertex3f(-1,-1,0.8);
  glTexCoord3f(0,1,0); glVertex3f(-2.5,-1,0.8);
  glEnd();

  //Roof structure
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_QUADS);

   //front

  glNormal3f( 0, 1, -1);
  glTexCoord3f(0,0,0); glVertex3f(var2,var4,-varW);
  glTexCoord3f(1,0,0); glVertex3f(-var2,var4,-varW);
  glTexCoord3f(1,1,0); glVertex3f(-var2,var1,0);
  glTexCoord3f(0,1,0); glVertex3f(var2,var1,0);

  //Back

  glNormal3f(0, 1, 1);
  glTexCoord3f(0,0,0); glVertex3f(var2,var4,varW);
  glTexCoord3f(1,0,0); glVertex3f(-var2,var4,varW);
  glTexCoord3f(1,1,0); glVertex3f(-var2,var1,0);
  glTexCoord3f(0,1,0); glVertex3f(var2,var1,0);
  glEnd();

  // Sides of Roof

  // right roof
  glBindTexture(GL_TEXTURE_2D,texture[1]);

  glBegin(GL_TRIANGLES);
  glNormal3f(+1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(var2,var4,-varW);
  glTexCoord3f(1,0,0); glVertex3f(var2,var4,varW);
  glTexCoord3f(1,0.5,0); glVertex3f(var2,var1,0);

  //left roof

  glNormal3f(-1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(-var2,var4,-varW);
  glTexCoord3f(1,0,0); glVertex3f(-var2,var4,varW);
  glTexCoord3f(1,0.5,0);glVertex3f(-var2,var1,0);
  glEnd();

  // Primary support

  glBindTexture(GL_TEXTURE_2D,texture[6]);
  glBegin(GL_QUADS);

  // left side
  glNormal3f(-1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(0.5,1,0.25);
  glTexCoord3f(1,0,0); glVertex3f(0.5,1,-.25);
  glTexCoord3f(1,1,0); glVertex3f(0.5,2,-.25);
  glTexCoord3f(0,1,0); glVertex3f(0.5,2,0.25);

  // right side
  glNormal3f(+1, 0, 0);
  glTexCoord3f(0,0,0); glVertex3f(1.01,1,0.25);
  glTexCoord3f(1,0,0); glVertex3f(1.01,1,-.25);
  glTexCoord3f(1,1,0); glVertex3f(1.01,2,-.25);
  glTexCoord3f(0,1,0); glVertex3f(1.01,2,0.25);

  // back side
  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0);glVertex3f(0.5,1,-0.25);
  glTexCoord3f(1,0,0);glVertex3f(0.5,2,-0.25);
  glTexCoord3f(1,1,0);glVertex3f(1,2,-0.25);
  glTexCoord3f(0,1,0);glVertex3f(1,1,-0.25);

  // front side
  glNormal3f( 0, 0,1);
  glTexCoord3f(0,0,0); glVertex3f(0.5,1,.25);
  glTexCoord3f(1,0,0); glVertex3f(0.5,2,.25);
  glTexCoord3f(1,1,0); glVertex3f(1,2,.25);
  glTexCoord3f(0,1,0); glVertex3f(1,1,.25);

  // top side
  glNormal3f( 0,+1, 0);
  glVertex3f(0.5,2,0.25);
  glVertex3f(0.5,2,-0.25);
  glVertex3f(1,2,-0.25);
  glVertex3f(1,2,0.25);
  glEnd();

  glBindTexture(GL_TEXTURE_2D,texture[6]);
  // Secondary Support
  glBegin(GL_QUADS);
  // left

  crossHelper(0,0,.5,0,-1,0);
  glTexCoord3f(0,0,0); glVertex3f(-0.5,1,0.25);
  glTexCoord3f(1,0,0); glVertex3f(-0.5,1,-.25);
  glTexCoord3f(1,1,0); glVertex3f(-0.5,2,-.25);
  glTexCoord3f(0,1,0); glVertex3f(-0.5,2,0.25);

  //right

  crossHelper(0,0,-.5,0,-1,0);
  glTexCoord3f(0,0,0); glVertex3f(-1.01,1,0.25);
  glTexCoord3f(1,0,0); glVertex3f(-1.01,1,-.25);
  glTexCoord3f(1,1,0); glVertex3f(-1.01,2,-.25);
  glTexCoord3f(0,1,0); glVertex3f(-1.01,2,0.25);

  //back

  //crossHelper(.5,0,0,0,1,0);
  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0); glVertex3f(-0.5,1,-0.25);
  glTexCoord3f(1,0,0); glVertex3f(-0.5,2,-0.25);
  glTexCoord3f(1,1,0); glVertex3f(-1,2,-0.25);
  glTexCoord3f(0,1,0); glVertex3f(-1,1,-0.25);


  //front

  glNormal3f( 0, 0,1);
  glTexCoord3f(0,0,0); glVertex3f(-0.5,1,.25);
  glTexCoord3f(1,0,0); glVertex3f(-0.5,2,.25);
  glTexCoord3f(1,1,0); glVertex3f(-1,2,.25);
  glTexCoord3f(0,1,0); glVertex3f(-1,1,.25);



  //top

  crossHelper(0,0,-.5,.5,0,0);
  glTexCoord3f(0,0,0); glVertex3f(-0.5,2,0.25);
  glTexCoord3f(1,0,0); glVertex3f(-0.5,2,-0.25);
  glTexCoord3f(1,1,0); glVertex3f(-1,2,-0.25);
  glTexCoord3f(0,1,0); glVertex3f(-1,2,0.25);
  glEnd();

  // main door of building
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);


  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0); glVertex3f(0.2,-1,-1.01);
  glTexCoord3f(1,0,0); glVertex3f(-0.2,-1,-1.01);
  glTexCoord3f(1,1,0); glVertex3f(-0.2,0,-1.01);
  glTexCoord3f(0,1,0); glVertex3f(0.2,0,-1.01);
  glEnd();

  //big side door for second building
  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUADS);

  glNormal3f( 0, 0,-1);
  glTexCoord3f(0,0,0); glVertex3f(2.3,-1,-0.81);
  glTexCoord3f(1,0,0); glVertex3f(1.2,-1,-0.81);
  glTexCoord3f(1,1,0); glVertex3f(1.2,0.5,-0.81);
  glTexCoord3f(0,1,0); glVertex3f(2.3,0.5,-0.81);
  glEnd();

  // Entry behind
  glBindTexture(GL_TEXTURE_2D,texture[4]);
  glBegin(GL_QUADS);

  glNormal3f( 0, 0, 1);
  glTexCoord3f(0,0,0); glVertex3f(0.5,-1,1.01);
  glTexCoord3f(1,0,0); glVertex3f(-0.5,-1,1.01);
  glTexCoord3f(1,1,0); glVertex3f(-0.5,0.5,1.01);
  glTexCoord3f(0,1,0); glVertex3f(0.5,0.5,1.01);
  glEnd();

  glPopMatrix();
}

void display()
{
   const double len=10;

   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   glEnable(GL_DEPTH_TEST);

   glLoadIdentity();

   if(mode == 2){
      // First Person
      currX = cos(rot1) * cos(rot2);
      currY = sin(rot2);
      currZ = sin(rot1) * cos(rot2);

      degreeLx = cos(rot1 - M_PI_2);
      degreeLz = sin(rot1 - M_PI_2);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(corX, corY, corZ, corX + currX, corY + currY, corZ + currZ, 0.0,1.0,0.0);
   }

   //  Perspective Orientation
   else if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   //  Orthogonal Orientation
   else if (mode == 0)
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

//  Light switch
if (lightSwitch)
{
  //  Translate intensity to color vectors
  float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
  float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
  float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
  //  Light position
  float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
  ball(Position[0],Position[1],Position[2] , 0.1);
  //  Draw light position as ball (still no lighting here)
  glColor3f(1,1,1);
  ball(Position[0],Position[1],Position[2] , 0.1);
  //  OpenGL should normalize normal vectors
  glEnable(GL_NORMALIZE);
  //  Enable lighting
  glEnable(GL_LIGHTING);
  //  Location for viewing specular changes
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
  //  glColor sets ambient and diffuse color materials
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  //  Enable light 0
  glEnable(GL_LIGHT0);
  //  Set ambient, diffuse, specular components and position of light 0
  glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
  glLightfv(GL_LIGHT0,GL_POSITION,Position);
}

else
  glDisable(GL_LIGHTING);

   buildingFrame(-3,-0.5,2,1,0.5,1,60);

   buildingFrame(5,-0.5,5,0.6,0.6,0.6,270);

   buildingFrame(2,-0.5,1,0.2,0.6,0.2,270);

   buildingFrame(0,0.5,-5, 3,1.5,2, 180);

   buildingFrame(7,-0.5,5,0.3,1,0.3,90);

   //buildingFrame(3,0,2, 2,1,1, 0);
   //buildingFrame(-3,-0.5,2, 1,0.5,1, 90);

   //glutSolidSphere(1.0,.2,.2);
   /*
   glBindTexture(GL_TEXTURE_2D,texture[6]);

   glBegin(GL_QUADS);
   glVertex3f(-100,-1,-100);
   glVertex3f(-100,-1,100);
   glVertex3f(100,-1,100);
   glVertex3f(100,-1,-100);
   */

   glEnd();

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


   //  Display parameters

   glWindowPos2i(5,550);

   if(mode == 0)
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=Orthogonal",th,ph,dim,fov);
   if(mode == 1)
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=Perspective",th,ph,dim,fov);
   if(mode == 2)
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=FirstPerson",th,ph,dim,fov);
   //  Render the scene and make it visible
   glFlush();
   Project(); // This line is the intial projection
   glutSwapBuffers();
}

//NEW
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/2000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
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

  float angle = .05;
  float diff = .05;
  // Leave the program
   if (ch == 27)
      exit(0);
   // Reset overall view
   else if (ch == '0')
      th = ph = 0;
     // change perspective mode
   else if (ch == 'm' || ch == 'M')
      if (mode > 2){
        mode = 0;
      }
      else {
        mode++;
      }

  //  Get rid of the light
  else if (ch == 'l' || ch == 'L')
      lightSwitch = 1-lightSwitch;

// Pause the light
  else if (ch == 'q' || ch == 'Q')
      move = 1-move;

// Move the light  ball
  else if (ch == '<' || ch == ',')
      zh += 3;

  else if (ch == '>' || ch == '.')
      zh -= 3;

   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;

  else if(ch == 'x' || ch == 'X')
      axes = 1 - axes;

   else if (ch == 'i')
      dim += 0.2;

   else if (ch == 'j')
      dim -= 0.2;

  //  Light elevation
  else if (ch=='k')
   ylight -= 1;
  else if (ch=='K')
   ylight += 1;

  //  Ambient level
  else if (ch=='a' && ambient>0)
   ambient -= 5;

  else if (ch=='A' && ambient<100)
   ambient += 5;

  //  Diffuse level
  else if (ch=='d' && diffuse>0)
   diffuse -= 5;

  else if (ch=='D' && diffuse<100)
   diffuse += 5;


  //  Specular level
  else if (ch=='s' && specular>0)
   specular -= 5;


  else if (ch=='S' && specular<100)
   specular += 5;


  //  Emission level
  else if (ch=='e' && emission>0)
   emission -= 5;


  else if (ch=='E' && emission<100)
   emission += 5;


  //  Shininess level
  else if (ch=='r' && shininess>-1)
   shininess -= 1;


  else if (ch=='R' && shininess<7)
   shininess += 1;


  //  Translate shininess power to value (-1 => 0)
  shinyvec[0] = shininess < 0 ? 0 : pow(2.0,shininess);

  if(mode == 2){
      if(ch == 'g' || ch == 'G') {
         float lx = cos(rot1)*cos(rot2);
         float ly = sin(rot2);
         float lz = sin(rot1)*cos(rot2);
         corX = corX + diff*lx;
         corY = corY + diff*ly;
         corZ = corZ + diff*lz;
     }

     else if(ch == 'b' || ch == 'B') {
         float lx = cos(rot1)*cos(rot2);
         float ly = sin(rot2);
         float lz = sin(rot1)*cos(rot2);
         corX = corX - diff*lx;
         corY = corY - diff*ly;
         corZ = corZ - diff*lz;
     }

     else if(ch == 'v' || ch == 'V'){
         rot1 -= angle;
     }

     else if(ch == 'n' || ch == 'N'){
         rot1 += angle;
     }

}
   Project();
   glutIdleFunc(move?idle:NULL);
   glutPostRedisplay();
}

void reshape(int width,int height){

   asp = (height>0) ? (double)width/height : 1;

   glViewport(0,0, width,height);

}



int main(int argc,char* argv[])
{

   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

   glutInitWindowSize(600,600);
   glutCreateWindow("Amar Patel 3D Scene w/Textures");

   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   glutIdleFunc(move?idle:NULL);

   //Find and intialize textures
   texture[0] = LoadTexBMP("Texture Lib/brickwall.bmp");
   texture[1] = LoadTexBMP("Texture Lib/roof.bmp");
   texture[2] = LoadTexBMP("Texture Lib/garage.bmp");
   texture[3] = LoadTexBMP("Texture Lib/wooddoor.bmp");
   texture[4] = LoadTexBMP("Texture Lib/back-door.bmp");
   texture[5] = LoadTexBMP("Texture Lib/double_door.bmp");
   texture[6] = LoadTexBMP("Texture Lib/rocky.bmp");
   glutMainLoop();

   return 0;
}
