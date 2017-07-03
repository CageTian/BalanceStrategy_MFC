#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif
//�������ͣ��̳���shape ƽ������   ��ת�Ƕ�+����    ���ű���

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
  //glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());//��ת������п��� Ĭ�ϵ�λ����
  glScaled(scale_.getX(), scale_.getY(), scale_.getZ());//�������ű��� �����Ǽܻ�����¥�ݺ�ƽ���


//  double* C = new double[16];
 // glGetDoublev(GL_MODELVIEW_MATRIX, C);
	//  cout << "draw�仯�����" << endl << endl;
	//  for (int i = 0; i < 16; i++)
	  //{

		//  cout << C[i] << "   ";
		  //if ((i + 1) % 4 == 0) cout << endl;
	  //}


  glBegin(GL_POLYGON);
    glNormal3fv(normals[facenum]);//����6������ x�� -x�� y�� -y�� z�� -z��   ���ڹ���
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
{//����һ��������
	

  polygon(0,1,2,3,0);
  polygon(5,4,7,6,1);
  polygon(4,0,3,7,2);
  polygon(1,5,6,2,3);
  polygon(1,0,4,5,4);
  polygon(3,2,6,7,5);
}
