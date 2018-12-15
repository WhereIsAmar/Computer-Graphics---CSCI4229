/*
Bandar Albegmi

Demonstrates using lighting
*  Key bindings:
*  l          Toggle lighting on/off
* ,/.         left/ right  toggling light
*  m          changes mode
*  a/A        decrease/increase ambient light
*  d/D        decrease/increase diffuse light
*  s/S        decrease/increase specular light
*  e/E        decrease/increase emitted light
*  n/N        Decrease/increase shininess
*  []         Lower/rise light
*  y          Toggle axes
*  arrows     Change view angle
*  x/z        Zoom in and out
*  0          Reset view angle
* c/C         turn left in first person view
* b/B         turn right in first person view
* f/F         go farward in first person view
* v/V         go farward in first person view
*  ESC        Exit
*/
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

int axes = 1;     //  Display axes
int mode = 0;     //  Projection mode
int th = 0;       //  Azimuth of view angle
int ph = 11;       //  Elevation of view angle
int light=1;      //  Lighting
int rep=1;        //  Repitition
int fov= 55;      //  Field of view (for perspective) //dn
double asp = 1;   //  Aspect ratio //
double dim = 20.0;   //  Size of world
double Dt = 0.05;
int rotation = 0.0;    //rotation var for 1st person
// axis angles

GLfloat angle = 0.0;

// Eye coords
double curr_X = 0;
double curr_Y = 0;
double curr_Z = 0;
double curr_I = 0;
double curr_J = 0;

// Light values
int distance = 10;
int inc = 10;
int loc = 0;

int emission  =   5;    // Emission intensity (%)
int ambient   =  15;    // Ambient intensity (%)
int diffuse   = 100;    // Diffuse intensity (%)
int specular  =   0;    // Specular intensity (%)
int shininess =   0;    // Shininess (power of two)
int zh        =  90;    // Light azimuth
float ylight  =   3;    // Elevation of light
//unsigned int texture[7];// Texture names
float shiny[1];         // Shininess (value)

typedef struct {
   double x;
   double y;
   double z;
} points_t;

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

static void NormalCrossProduct(double p1x, double p1y, double p1z, double p2x, double p2y, double p2z,double p3x, double p3y, double p3z)
{
  double ax = p1x - p3x;
  double ay = p1y - p3y;
  double az = p1z - p3z;

  double bx = p1x - p2x;
  double by = p1y - p2y;
  double bz = p1z - p2z;

  double cx = ay * bz - az * by;
  double cy = az * bx - ax * bz;
  double cz = ax * by - ay * bx;

  glNormal3d(cx, cy, cz);
}
/*
 *  Draw vertex in polar coordinates with normal
 *  Adapted from ex13
 */
static void Vertex(double th,double ph) {
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z = Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

static void ball(double x,double y,double z,double r) {
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glScaled(r,r,r);
  //  White ball
  //glColor3f(1,1,1);
  glMaterialfv(GL_FRONT,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
  //  Bands of latitude
  for (ph=-90;ph<90;ph+=inc) {
    glBegin(GL_QUAD_STRIP);
  for (th=0;th<=360;th+=2*inc) {
    Vertex(th,ph);
    Vertex(th,ph+inc);
  }
  glEnd();
  }
  //  Undo transofrmations
  glPopMatrix();
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*ch++);
}

static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
   {
      gluPerspective(fov,asp,dim/4,4*dim);
    }
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void Car(double x, double y, double z, double dx, double dy, double dz, double th)
{
  //  Set specular color to white
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};

 glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  //  Car
  glBegin(GL_QUADS);
  NormalCrossProduct(0.2,0.4,0.6,   0.2,0.4,0.2,   0.6,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(0.2,0.4,0.6);
  glTexCoord3f(1,0,0); glVertex3f(0.2,0.4,0.2);//p4
  glTexCoord3f(1,1,0); glVertex3f(0.6,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(0.6,0.5,0.6);//p2
  glEnd();

  /* bottom of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.2,0.2,0.6,   0.2,0.2,0.2,    0.6,0.2,0.2);
  glTexCoord3f(0,0,0); glVertex3f( 0.2,0.2,0.6);
  glTexCoord3f(1,0,0); glVertex3f( 0.2,0.2,0.2);//p4
  glTexCoord3f(1,1,0); glVertex3f(0.6,0.2,0.2);
  glTexCoord3f(0,1,0); glVertex3f(0.6,0.2,0.6);//p2
  glEnd();
  /* front of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.2,0.2,0.6,   0.2,0.2,0.2,    0.2,0.4,0.2);
  glTexCoord3f(0,0,0); glVertex3f( 0.2,0.2,0.6);
  glTexCoord3f(1,0,0); glVertex3f( 0.2,0.2,0.2);//p4
  glTexCoord3f(1,1,0); glVertex3f(0.2,0.4,0.2);
  glTexCoord3f(0,1,0); glVertex3f(0.2, 0.4,0.6);//p2
  glEnd();
  /* back of cube.*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.6,0.2,0.6,   0.6,0.2,0.2,    0.6,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(0.6,0.2,0.6);
  glTexCoord3f(1,0,0); glVertex3f( 0.6,0.2,0.2);//p4
  glTexCoord3f(1,1,0); glVertex3f(0.6,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(0.6,0.5,0.6);//p2
  glEnd();
  /* left of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.2,0.2,0.6,   0.2,0.4,0.6,    0.6,0.5,0.6);
  glTexCoord3f(1,0,0); glVertex3f(0.2,0.2,0.6);
  glTexCoord3f(1,1,0); glVertex3f(0.2,0.4,0.6);//p4
  glTexCoord3f(0,1,0); glVertex3f(0.6,0.5,0.6);
  glTexCoord3f(0,0,0); glVertex3f(0.6,0.2,0.6);//p2
  glEnd();
  /* Right of cube */
  glBegin(GL_QUADS);
  NormalCrossProduct(0.2,0.2,0.2,   0.6,0.2,0.2,    0.6,0.5,0.2);
  glTexCoord3f(1,0,0); glVertex3f(0.2,0.2,0.2);
  glTexCoord3f(0,0,0); glVertex3f( 0.6,0.2,0.2);
  glTexCoord3f(0,1,0); glVertex3f( 0.6,0.5,0.2);
  glTexCoord3f(1,1,0); glVertex3f( 0.2,0.4,0.2);
  glEnd();
//____________________________________top cover________________________________________
  glBegin(GL_QUADS);
  NormalCrossProduct(0.7,0.65,0.6,    0.7,0.65,0.2,   1.7,0.65,0.2);
  glTexCoord3f(0,0,0); glVertex3f(0.7,0.65,0.6);
  glTexCoord3f(1,0,0); glVertex3f(0.7,0.65,0.2);
  glTexCoord3f(1,1,0); glVertex3f(1.7,0.65,0.2);
  glTexCoord3f(0,1,0);glVertex3f(1.7,0.65,0.6);
  glEnd();
//____________________________________back guard________________________________________
  // top of cube
  glBegin(GL_QUADS);
  NormalCrossProduct(1.8, 0.5,0.6,    1.8, 0.5,0.2,   2.1, 0.4,0.2);
  glTexCoord3f(0,0,0); glVertex3f( 1.8, 0.5,0.6);
  glTexCoord3f(1,0,0); glVertex3f(1.8, 0.5,0.2);
  glTexCoord3f(1,1,0); glVertex3f(2.1, 0.4, 0.2);
  glTexCoord3f(0,1,0); glVertex3f(2.1,0.4,0.6);
  glEnd();
  // bottom of cube
  glBegin(GL_QUADS);
  NormalCrossProduct(2.1,0.2,0.6,   2.1,0.2,0.2,    1.8,0.2,0.6);
  glTexCoord3f(0,0,0); glVertex3f( 2.1,0.2,0.6);
  glTexCoord3f(1,0,0); glVertex3f(2.1,0.2,0.2);
  glTexCoord3f(1,1,0); glVertex3f(1.8,0.2,0.6);
  glTexCoord3f(0,1,0); glVertex3f( 1.8,0.2,0.6);
  glEnd();
  /* back of cube.*/
  glBegin(GL_QUADS);
  NormalCrossProduct(2.1,0.4,0.6,   2.1,0.4,0.2,    2.1,0.2,0.2);
  glTexCoord3f(1,1,0); glVertex3f(2.1,0.4,0.6);
  glTexCoord3f(0,1,0); glVertex3f(2.1,0.4,0.2);
  glTexCoord3f(0,0,0); glVertex3f(2.1,0.2,0.2);
  glTexCoord3f(1,0,0); glVertex3f(2.1,0.2,0.6);
  glEnd();
  /* right of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(1.8,0.2,0.2,   2.1,0.2,0.2,    2.1,0.4,0.2);
  glTexCoord3f(1,0,0); glVertex3f(1.8,0.2,0.2);
  glTexCoord3f(0,0,0); glVertex3f(2.1,0.2,0.2);//p4
  glTexCoord3f(0,1,0); glVertex3f(2.1,0.4,0.2);
  glTexCoord3f(1,1,0); glVertex3f(1.8,0.5,0.2);//p2
  glEnd();
  /* left of cube */
  glBegin(GL_QUADS);
  NormalCrossProduct(1.8,0.2,0.6,   1.8,0.5,0.6,    2.1,0.4,0.6);
  glTexCoord3f(1,0,0); glVertex3f(1.8,0.2,0.6);
  glTexCoord3f(1,1,0); glVertex3f(1.8,0.5,0.6);
  glTexCoord3f(0,1,0); glVertex3f(2.1,0.4,0.6);
  glTexCoord3f(0,0,0); glVertex3f(2.1,0.2,0.6);
  glEnd();
//________________________MIDDLE BODY________________________________________________
/* Left of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.6, 0.5,0.6,    1.8,0.5,0.6,    1.8, 0.2, 0.6);
  glTexCoord3f(0,1,0); glVertex3f(0.6, 0.5,0.6);
  glTexCoord3f(1,1,0); glVertex3f(1.8,0.5,0.6);//p4
  glTexCoord3f(1,0,0); glVertex3f(1.8, 0.2, 0.6);
  glTexCoord3f(0,0,0); glVertex3f(0.6, 0.2,0.6);//p2
  glEnd();
  /* bottom of cube*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.6,0.2,0.6,   0.6,0.2,0.2,    1.8,0.2,0.2);
  glTexCoord3f(1,1,0); glVertex3f( 0.6,0.2,0.6);
  glTexCoord3f(1,0,0); glVertex3f(0.6,0.2,0.2);
  glTexCoord3f(0,0,0); glVertex3f(1.8,0.2,0.2);
  glTexCoord3f(0,1,0); glVertex3f( 1.8,0.2,0.6);
  glEnd();
  /* right of cube.*/
  glBegin(GL_QUADS);
  NormalCrossProduct(0.6,0.5,0.2,   0.6,0.2,0.2,    1.8,0.5,0.2);
  glTexCoord3f(1,1,0); glVertex3f(0.6,0.5,0.2);//110
  glTexCoord3f(1,0,0); glVertex3f(0.6,0.2,0.2);//100
  glTexCoord3f(0,0,0); glVertex3f(1.8,0.2,0.2);//000
  glTexCoord3f(0,1,0); glVertex3f(1.8,0.5,0.2);//010
  glEnd();
//____________________________WINDOW Right____________________________________________*
  glBegin(GL_QUADS);
  NormalCrossProduct(0.77, 0.63,0.2,    0.75, 0.5,0.2,    1.2, 0.5, 0.2);
  glTexCoord3f(0,0,0); glVertex3f( 0.77, 0.63,0.2);
  glTexCoord3f(1,0,0); glVertex3f(0.75, 0.5,0.2);        //quad front window
  glTexCoord3f(1,1,0); glVertex3f(1.2, 0.5, 0.2);
  glTexCoord3f(0,1,0); glVertex3f( 1.22,0.63,0.2);
  glEnd();

  glBegin(GL_QUADS);
  NormalCrossProduct(1.27,0.63,0.2,   1.25,0.5,0.2,   1.65,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(1.27,0.63,0.2);
  glTexCoord3f(1,0,0); glVertex3f(1.25,0.5,0.2);        //quad back window
  glTexCoord3f(1,1,0); glVertex3f(1.65,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(1.67,0.63,0.2);
  glEnd();
//____________________________line strip____________________________________________*
  glBegin(GL_QUADS);
  NormalCrossProduct(0.75, 0.65,0.2,    0.75, 0.63,0.2,   1.7, 0.63, 0.2);
  glTexCoord3f(0,0,0); glVertex3f( 0.75, 0.65,0.2);
  glTexCoord3f(1,0,0); glVertex3f(0.75, 0.63,0.2);        //line strip right
  glTexCoord3f(1,1,0); glVertex3f(1.7, 0.63, 0.2);
  glTexCoord3f(0,1,0); glVertex3f( 1.7,0.65,0.2);
  glEnd();

  glBegin(GL_QUADS);
  NormalCrossProduct(0.75, 0.65,0.6,    1.7,0.65,0.6,   1.7, 0.63, 0.6);
  glTexCoord3f(0,0,0); glVertex3f( 0.75, 0.65,0.6);
  glTexCoord3f(1,0,0); glVertex3f( 1.7,0.65,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(1.7, 0.63, 0.6);
  glTexCoord3f(0,1,0); glVertex3f(0.75, 0.63,0.6); //p2       //line strip left
  glEnd();
//____________________________WINDOW Left____________________________________________*
  glBegin(GL_QUADS);
  NormalCrossProduct(0.77, 0.63,0.6,    1.22,0.63,0.6,    1.2, 0.5, 0.6);
  glTexCoord3f(0,0,0); glVertex3f( 0.77, 0.63,0.6);
  glTexCoord3f(1,0,0); glVertex3f( 1.22,0.63,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(1.2, 0.5, 0.6);
  glTexCoord3f(0,1,0); glVertex3f(0.75, 0.5,0.6); //p2       //quad front window
  glEnd();
  glBegin(GL_QUADS);
  NormalCrossProduct(1.27,0.63,.6,    1.67,0.63,0.6,    1.65,0.5,0.6);
  glTexCoord3f(0,0,0); glVertex3f(1.27,0.63,.6);
  glTexCoord3f(1,0,0); glVertex3f(1.67,0.63,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(1.65,0.5,0.6);
  glTexCoord3f(0,1,0); glVertex3f(1.25,0.5,0.6); //p2       //quad back window
  glEnd();
//____________________________separation left____________________________________________*
  glBegin(GL_QUADS);
  NormalCrossProduct(0.7,0.65,0.6,    0.77,0.65,0.6,    0.75,0.5,0.6);
  glTexCoord3f(0,0,0); glVertex3f(0.7,0.65,0.6);
  glTexCoord3f(1,0,0); glVertex3f(0.77,0.65,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(0.75,0.5,0.6);
  glTexCoord3f(0,1,0); glVertex3f(0.7,0.5,0.6);  //p2     //first separation
  glEnd();
  glBegin(GL_QUADS);
  NormalCrossProduct(1.2,0.65,0.6,    1.27,0.65,0.6,    1.25,0.5,0.6);
  glTexCoord3f(0,0,0); glVertex3f(1.2,0.65,0.6);
  glTexCoord3f(1,0,0); glVertex3f(1.27,0.65,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(1.25,0.5,0.6);
  glTexCoord3f(0,1,0); glVertex3f(1.2,0.5,0.6); //p2      //second separation
  glEnd();
  glBegin(GL_QUADS);
  NormalCrossProduct(1.65,0.65,0.6,   1.7,0.65,0.6,   1.7,0.5,0.6);
  glTexCoord3f(0,0,0); glVertex3f(1.65,0.65,0.6);
  glTexCoord3f(1,0,0); glVertex3f(1.7,0.65,0.6);//p4
  glTexCoord3f(1,1,0); glVertex3f(1.7,0.5,0.6);
  glTexCoord3f(0,1,0); glVertex3f(1.65,0.5,0.6);//p2
  glEnd();
//____________________________separation right____________________________________________*
  glBegin(GL_QUADS);
  NormalCrossProduct(0.7,0.65,0.2,    0.7,0.5,0.2,    0.75,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(0.7,0.65,0.2);
  glTexCoord3f(1,0,0); glVertex3f(0.7,0.5,0.2);       //first separation
  glTexCoord3f(1,1,0); glVertex3f(0.75,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(0.77,0.65,0.2);
  glEnd();
  glBegin(GL_QUADS);
  NormalCrossProduct(1.2,0.65,0.2,    1.2,0.5,0.2,    1.25,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(1.2,0.65,0.2);
  glTexCoord3f(1,0,0); glVertex3f(1.2,0.5,0.2);       //second separation
  glTexCoord3f(1,1,0); glVertex3f(1.25,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(1.27,0.65,0.2);
  glEnd();
  glBegin(GL_QUADS);
  NormalCrossProduct(1.65,0.65,0.2,   1.65,0.5,.2,    1.7,0.5,0.2);
  glTexCoord3f(0,0,0); glVertex3f(1.65,0.65,0.2);
  glTexCoord3f(1,0,0); glVertex3f(1.65,0.5,.2);     //3d separation
  glTexCoord3f(1,1,0); glVertex3f(1.7,0.5,0.2);
  glTexCoord3f(0,1,0); glVertex3f(1.7,0.65,0.2);
  glEnd();
//..........................................................................
  glPushMatrix();

    glBegin(GL_LINE_STRIP);
  	  for(th=0;th<360;th=th+20)
  	  {
    glTexCoord3f(0,0,0); glVertex3f(0.6,0.2,0.62);
    glVertex3f(0.6+(0.08*(cos(((th+angle)*3.14)/180))),0.2+(0.08*(sin(((th+angle)*3.14)/180))),0.62);
  	  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  	  for(th=0;th<360;th=th+20)
  	  {
    glTexCoord3f(0,0,0); glVertex3f(0.6,0.2,0.18);
    glVertex3f(0.6+(0.08*(cos(((th+angle)*3.14)/180))),0.2+(0.08*(sin(((th+angle)*3.14)/180))),0.18);
  	  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for(th=0;th<360;th=th+20)
  	  {
    glTexCoord3f(0,0,0); glVertex3f(1.7,0.2,0.18);
    glVertex3f(1.7+(0.08*(cos(((th+angle)*3.14)/180))),0.2+(0.08*(sin(((th+angle)*3.14)/180))),0.18);
  	  }
  glEnd();
  glBegin(GL_LINE_STRIP);
  for(th=0;th<360;th=th+20)
  	  {
  glTexCoord3f(0,0,0); glVertex3f(1.7,0.2,0.62);
   glVertex3f(1.7+(0.08*(cos(((th+angle)*3.14)/180))),0.2+(0.08*(sin(((th+angle)*3.14)/180))),0.62);
  	  }
  glEnd();
    glTranslatef(0.6,0.2,0.6);
    //glColor3f(0,0,0);
    glutSolidTorus(0.025,0.07,10,25);
    glNormal3f(0.6,0.2,0.6);////////////

    glTranslatef(0,0,-0.4);
    glutSolidTorus(0.025,0.07,10,25);
    glNormal3f(0,0,-0.4);////////////

    glTranslatef(1.1,0,0);
    glutSolidTorus(0.025,0.07,10,25);
    glNormal3f(1.1,0,0);////////////

    glTranslatef(0,0,0.4);
    glutSolidTorus(0.025,0.07,10,25);
    glNormal3f(0,0,0.4);////////////
    glPopMatrix();
  //  Undo transofrmations
  glPopMatrix();
}
void display(){
   const double len = 7;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode == 1){
      double curr_X = -2*dim*Sin(th)*Cos(ph);
      double curr_Y = +2*dim*Sin(ph);
      double curr_Z = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(curr_X,curr_Y,curr_Z , 0,0,0 , 0,Cos(ph),0);
   }
   else if (mode == 2) {
  //  First Person Projection
   curr_I = +2*dim*Sin(rotation); //Ajust the camera vector based on rotation
   curr_J = -2*dim*Cos(rotation);
   gluLookAt(curr_X,curr_Y,curr_Z, curr_I+curr_X,curr_Y,curr_J+curr_Z, 0,1,0); //  Use gluLookAt, y is the up-axis
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }


   glShadeModel(GL_SMOOTH);
   //  Translate intensity to color vectors
   float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
   //  Light position
   float Position[] = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
   //  Draw light position as ball (still no lighting here)
   //glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 0.1);
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,loc);
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


   Car(-1,0,3,.3,.3,.3,0);
   Car(-1,0,5,.5,.5,.5,135);
   Car(-1,0,1,2,2,2,90);

   /*glColor3ub(36,82,91);
   glBegin(GL_QUADS);
   glVertex3f(-100,-1,-100);
   glVertex3f(-100,-1,100);
   glVertex3f(100,-1,100);
   glVertex3f(100,-1,-100);
   glEnd();*/
   glBegin(GL_QUADS);
   NormalCrossProduct(-8,0,-8,   8,0,-8,    8,0,8);
   glTexCoord3f(0,0,0); glVertex3f(-8,0,-8);
   glTexCoord3f(1,0,0); glVertex3f(-8,0,8);
   glTexCoord3f(1,1,0); glVertex3f(8,0,8);
   glTexCoord3f(0,1,0); glVertex3f(8,0,-8);
   glEnd();

   //  Draw axes
   //glColor3f(1,1,1);
   if (axes)
   {
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
   glWindowPos2i(5,5);

      if(mode == 0)
         Print("Angle =%d,%d  Dim=%.1f FOV=%d Projection=Orthogonal",th,ph,dim,fov);
      else if(mode == 1)
         Print("Angle =%d,%d  Dim=%.1f FOV=%d Projection=Perspective",th,ph,dim,fov);
      else if(mode == 2)
         Print("Angle =%d,%d  Dim=%.1f FOV=%d Projection=FirstPerson",th,ph,dim,fov);


   //Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible

   glFlush();
   Project();
   glutSwapBuffers();
}

void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   //rotation %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y){
   if (ch == 27){                       //  Exit on ESC
      exit(0);
    }
   else if (ch == '0'){                 //  Reset view angle
      th  = ph = 0;
    }
     else if (ch == 'm' || ch == 'M'){  //  Switch display mode
           if (mode > 2){
             mode = 0;
           }
           else {
             mode++;
           }
      }
  else if (ch == 'y' || ch == 'Y'){     //  Toggle axes
       axes = 1-axes;
    }
   else if (ch == 'l' || ch == 'L'){    //  Toggle lighting on/off
     light = 1-light;
   }
   else if (ch == '<' || ch == ','){    //  Move light left
     zh += 3;
   }
   else if (ch == '>' || ch == '.'){     //  Move light right
     zh -= 3;
   }
  else if (ch == 'f' || ch == 'F'){    // go forward
    curr_X += curr_I*Dt;
    curr_Z += curr_J*Dt;
  }
  else if (ch == 'c' || ch == 'C'){    // turn left
    rotation -= 3;
  }
  else if (ch == 'v' || ch == 'V'){    // go backward
    curr_X -= curr_I*Dt;
    curr_Z -= curr_J*Dt;
  }
  else if (ch == 'b' || ch == 'B'){    // turn right
    rotation += 3;
  }
   //  Change field of view angle
   //  w key - increase dim
   else if (ch == 'z' || ch == 'Z')    //  decrease dim
      dim += 0.1;

   else if ((ch == 'x' && dim>1) || (ch == 'X' && dim>1)){    //  decrease dim
      dim -= 0.1;
    }
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
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
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+'){
      rep++;
    }
   else if (ch=='-' && rep>1){
      rep--;
    }
   //  Translate shininess power to value (-1 => 0)
  // shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project();
   glutIdleFunc(light?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Albegmi's Lighting");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
