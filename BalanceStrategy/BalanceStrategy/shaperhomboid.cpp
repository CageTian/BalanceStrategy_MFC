#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include "shaperhomboid.h"

static GLfloat vertices[][3] = { {0.5f, 10.0f, 0.5f},  {-0.5f, 10.0f, 0.5f}, {-0.5f, 9.0f, 0.5f},
			         {0.5f, 9.0f, 0.5f}, {0.5f, 0.0f, -0.5f}, {-0.5f, 0.0f, -0.5f},
                                 {-0.5f, -1.0f, -0.5f}, {0.5f, -1.0f, -0.5f}
                               };

static GLfloat normals[][3] = { {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f},
			 {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
			 {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
			};

void ShapeRhomboid::polygon(int a, int b, int c, int d, int facenum)
{
  glPushMatrix();

  glTranslated(trans_.getX(), trans_.getY(), trans_.getZ());
  glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());
 glScaled(scale_.getX(), scale_.getY(), scale_.getZ());

  glBegin(GL_POLYGON);
    glNormal3fv(normals[facenum]);
    glVertex3fv(vertices[a]);

    glNormal3fv(normals[facenum]);
    glVertex3fv(vertices[b]);

    glNormal3fv(normals[facenum]);
    glVertex3fv(vertices[c]);

    glNormal3fv(normals[facenum]);
    glVertex3fv(vertices[d]);
  glEnd();

  glPopMatrix();
}

void ShapeRhomboid::draw()
{
  polygon(0,1,2,3,0);
  polygon(5,4,7,6,1);
  polygon(4,0,3,7,2);
  polygon(1,5,6,2,3);
  polygon(1,0,4,5,4);
  polygon(3,2,6,7,5);
}
