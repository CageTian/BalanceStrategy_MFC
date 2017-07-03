#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif
//数据类型：继承了shape 平移向量   旋转角度+向量    缩放比例

#include <GL/gl.h>
#include "shapecube.h"
#include<iostream>
using namespace std;




static GLfloat vertices[][3] = { {0.5f, 0.5f, 0.5f},  {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},
			  {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
                          {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}
                        };

static GLfloat normals[][3] = { {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f},
			 {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
			 {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
			};

int flag2 = 0;
void ShapeCube::polygon(int a, int b, int c, int d, int facenum)
{
  glPushMatrix();





	

  glTranslated(trans_.getX(), trans_.getY(), trans_.getZ());
  //glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());//旋转矩阵可有可无 默认单位矩阵
  glScaled(scale_.getX(), scale_.getY(), scale_.getZ());//调整缩放比例 来画骨架或者是楼梯和平面等


//  double* C = new double[16];
 // glGetDoublev(GL_MODELVIEW_MATRIX, C);
	//  cout << "draw变化后矩阵" << endl << endl;
	//  for (int i = 0; i < 16; i++)
	  //{

		//  cout << C[i] << "   ";
		  //if ((i + 1) % 4 == 0) cout << endl;
	  //}


  glBegin(GL_POLYGON);
    glNormal3fv(normals[facenum]);//法线6个方向 x轴 -x轴 y轴 -y轴 z轴 -z轴   用于光照
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

void ShapeCube::draw()
{//花了一个立方体
	

  polygon(0,1,2,3,0);
  polygon(5,4,7,6,1);
  polygon(4,0,3,7,2);
  polygon(1,5,6,2,3);
  polygon(1,0,4,5,4);
  polygon(3,2,6,7,5);
}
