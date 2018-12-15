/*
 *  Set projection
 */
#include "CSCIx229.h"

static void Project() {
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   if (mode == 1) {
         //  Perspective transformation
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else if (mode == 2) {
         //  First Person Projection
      gluPerspective(fov,asp,dim/4,4*dim);
   }
   else
        //  Orthogonal projection
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}
