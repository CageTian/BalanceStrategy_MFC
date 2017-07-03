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

#define NUM_STAIRS 4//楼梯数量 

BasicTerrain::BasicTerrain()//基本地形 构造函数 画楼梯
{
  ShapeCube* flat;
  ShapeRhomboid* incline;

  // Stairs
  for(int i = 0; i < NUM_STAIRS; i++)
  {
    flat = new ShapeCube();
    flat->setScale(4.0, 0.5, 0.54);//-0.25 代表的意思：此时立方体底面y在原点下方-0.25处 把第一块儿彻底搬到x轴下面
    
	flat->setTranslation(0.0, -0.25 + (-0.39 * i), -2.23+ (-0.54 * i));
    shapes_.push_back(flat);
    path_.push_back(new Line2D(-2.5 + (-0.54 * i), (-0.39 * i), -1.96 + (-0.54 * i), (-0.39 * i)));

  }
  //new line2D 的x1 x2 表示这块板的上面 or 底面 的 边长为4的两条线的z轴坐标
  //y1,y2表示这块板上面 的y轴坐标   立方体上面距离原点的y距离
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
    if(z <= (*pli)->x2_)//找到 root 停留在那个板上
      break;
  }

  if(pli != path_.end())
  {
    // Get the slope
    Line2D* l = *pli;
	//除了斜坡那个面以外 y1=y2 所以除了斜坡那一面 其余调用getHeight都是返回y1 也就是这个板子上面的y轴坐标y1
	//若小人此刻在斜面上 则 m便是斜面的斜率  
	// 返回结果是  斜率*（root当前高度-斜面底部高度）+（斜面底部高度）！！！！！！！！！！！！！
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
    if(z <= (*pli)->x2_)//找到root停留的那个板
      break;
  }

  if(pli != path_.end())
  {
    // Get the angle
    Line2D* l = *pli;
    double xx = (l->x2_ - l->x1_); 
    double yy = (l->y2_ - l->y1_);//除了斜面以外 y1=y2=0
    double zz = sqrt((xx * xx) + (yy * yy)); //除了斜面以外 zz=xx
	//acos() 函数：给出余弦  求出角度  
	//所以只有斜面有角度  其余的面返回角度0度
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
	
	  (*sli)->draw();//调用的是shapecube的数据



  }


  glPopMatrix();
}
