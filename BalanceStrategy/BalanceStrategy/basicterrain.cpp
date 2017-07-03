#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <math.h>
#include "shape.h"
#include "shapecube.h"
#include "shaperhomboid.h"
#include "basicterrain.h"
#include<iostream>
using namespace std;
#define RAD_TO_DEG 57.2957795

#define NUM_STAIRS 4//¥������ 

BasicTerrain::BasicTerrain()//�������� ���캯�� ��¥��
{
  ShapeCube* flat;
  ShapeRhomboid* incline;

  // Stairs
  for(int i = 0; i < NUM_STAIRS; i++)
  {
    flat = new ShapeCube();
    flat->setScale(4.0, 0.5, 0.54);//-0.25 �������˼����ʱ���������y��ԭ���·�-0.25�� �ѵ�һ������װᵽx������
    
	flat->setTranslation(0.0, -0.25 + (-0.39 * i), -2.23+ (-0.54 * i));
    shapes_.push_back(flat);
    path_.push_back(new Line2D(-2.5 + (-0.54 * i), (-0.39 * i), -1.96 + (-0.54 * i), (-0.39 * i)));

  }
  //new line2D ��x1 x2 ��ʾ��������� or ���� �� �߳�Ϊ4�������ߵ�z������
  //y1,y2��ʾ�������� ��y������   �������������ԭ���y����
  // First flat segment  
  flat = new ShapeCube();
  flat->setScale(4.0, 0.1, 5.0 - 0.54);
  flat->setTranslation(0.0, -0.05, 0.27);
  shapes_.push_back(flat);
  path_.push_back(new Line2D(-1.96, 0.0, 2.5, 0.0));

  // Incline segment
  incline = new ShapeRhomboid();
  incline->setScale(4.0, 0.1, 3.0);
  incline->setTranslation(0.0, 0.0, 2.5 + 1.5);
  shapes_.push_back(incline);
  path_.push_back(new Line2D(2.5, 0.0, 5.5, 1.0));

  // Second flat segment
  flat = new ShapeCube();
  flat->setScale(4.0, 0.1, 5.0);
  flat->setTranslation(0.0, 0.95, 2.5 + 3.0 + 2.5);
  shapes_.push_back(flat);
  path_.push_back(new Line2D(5.5, 1.0, 10.5, 1.0));
}

BasicTerrain::~BasicTerrain()
{
  ShapeList_t::iterator sli;
  for(sli = shapes_.begin(); sli != shapes_.end(); ++sli)
    delete *sli;

  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
    delete *pli;
}

double BasicTerrain::getHeight(double x, double z)
{
  // Find line segment that contains current z position
  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
  {
    if(z <= (*pli)->x2_)//�ҵ� root ͣ�����Ǹ�����
      break;
  }

  if(pli != path_.end())
  {
    // Get the slope
    Line2D* l = *pli;
	//����б���Ǹ������� y1=y2 ���Գ���б����һ�� �������getHeight���Ƿ���y1 Ҳ����������������y������y1
	//��С�˴˿���б���� �� m����б���б��  
	// ���ؽ����  б��*��root��ǰ�߶�-б��ײ��߶ȣ�+��б��ײ��߶ȣ���������������������������
    double m = (l->y2_ - l->y1_) / (l->x2_ - l->x1_);
    return (m * (z - l->x1_)) + l->y1_;
  }

  return 0.0;
}

double BasicTerrain::getAngle(double x, double z)
{
  // Find line segment that contains current z position
  LineList_t::iterator pli;
  for(pli = path_.begin(); pli != path_.end(); ++pli)
  {
    if(z <= (*pli)->x2_)//�ҵ�rootͣ�����Ǹ���
      break;
  }

  if(pli != path_.end())
  {
    // Get the angle
    Line2D* l = *pli;
    double xx = (l->x2_ - l->x1_); 
    double yy = (l->y2_ - l->y1_);//����б������ y1=y2=0
    double zz = sqrt((xx * xx) + (yy * yy)); //����б������ zz=xx
	//acos() ��������������  ����Ƕ�  
	//����ֻ��б���нǶ�  ������淵�ؽǶ�0��
    return acos((xx < 0.0) ? -xx : xx / zz) * RAD_TO_DEG;
  }

  return 0.0;
}
int flag3 = 0;
void BasicTerrain::draw()
{
  glPushMatrix();

  glTranslated(trans_.getX(), trans_.getY(), trans_.getZ());
  glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());
  glScaled(scale_.getX(), scale_.getY(), scale_.getZ());




  ShapeList_t::iterator sli;
  int i = 0;
  for (sli = shapes_.begin(); sli != shapes_.end(); ++sli)
  {
	  /*glPushMatrix();
	  glTranslated((*sli)->getTranslation()->getX(), (*sli)->getTranslation()->getY(), (*sli)->getTranslation()->getZ());
	  glRotated((*sli)->getRotationAngle(), (*sli)->getRotationAxis()->getX(), (*sli)->getRotationAxis()->getY(), (*sli)->getRotationAxis()->getZ());
	  glTranslated((*sli)->getScale()->getX(), (*sli)->getScale()->getY(), (*sli)->getScale()->getZ());
	  glBegin(GL_LINES);
	  glColor3f(1.0f, 0.0f, 0.0f);


	  // glVertex2d(lines[i]->x1_, lines[i]->y1_);
	  //glVertex2d(lines[i]->x2_, lines[i]->y2_);
	  i++;
	  glEnd();


	  glPopMatrix();
	  glColor3d(0.0f, 0.0f, 1.0f);*/
	
	  (*sli)->draw();//���õ���shapecube������



  }


  glPopMatrix();
}
