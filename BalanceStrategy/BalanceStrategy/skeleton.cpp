#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include "shape.h"
#include "skeleton.h"
#include<iostream>
#include "names.h"
using namespace std;
SkelDOF::SkelDOF(double value, double min, double max)
	: min_(min),
	max_(max)
{
	setValue(value);
}

void SkelDOF::setValue(double value)
{
	// if(value < min_)
	//  value_ = min_;
	// else if(value > max_)
	//  value_ = max_;
	// else
	value_ = value;
}


SkelNode* Skeleton::findNextNode(SkelArc* arc, const std::string& name) const
{
	SkelNode* node = arc->getNode();

	if (node->getName() == name)
		return node;

	//Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		SkelNode* result = findNextNode(*i, name);
		if (result != NULL)
			return result;
	}

	return NULL;
}

SkelArc* Skeleton::findNextArc(SkelArc* arc, const std::string& name) const
{
//	cout <<endl<<"  "<< arc->getName() << "  " << name << endl;
	if (arc->getName() == name)
		return arc;

	//Process the children
	SkelNode* node = arc->getNode();
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		SkelArc* result = findNextArc(*i, name);
		if (result != NULL)
			return result;
	}

	return NULL;
}

void Skeleton::visitNextNode(SkelArc* arc, NodeVisitor& visitor)
{
	SkelNode* node = arc->getNode();

	// Visit the node
	visitor(node);

	//Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		visitNextNode(*i, visitor);
	}
}

void Skeleton::visitNextArc(SkelArc* arc, ArcVisitor& visitor)
{
	// Visit the arc
	visitor(arc);    //先赋值根关节root的自由度

					 //Process the children
	SkelNode* node = arc->getNode();//然后获得关节所关联的骨架
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)//找到该骨架所绑定的其他关节
	{
		visitNextArc(*i, visitor);//递归 赋值自由度
	}
}

void Skeleton::arcPositions(const SkelArc* arc, const Matrix44& G, ArcPositionVector_t& positions)
{
	SkelNode* node = arc->getNode();
	Vector3 p;
	Matrix44 t, r;
	Matrix44 L = G;

	t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
	r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

	t *= r;
	L *= t;

	p = L * p;
	positions[arc->getName()] = p;

	//Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		arcPositions(*i, L, positions);
	}
}

void Skeleton::arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms)
{
	SkelNode* node = arc->getNode();
	Vector3 p;
	Matrix44 t, r;
	Matrix44 L = G;

	t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
	r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

	t *= r;
	L *= t;

	transforms[arc->getName()] = L;

	//Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		arcTransforms(*i, L, transforms);
	}
}

void Skeleton::evalArc(const SkelArc* arc) const
{
	SkelNode* node = arc->getNode();

	glPushMatrix();
	Matrix44 t, r;

	t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
	r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));
	t *= r;




	//////////////////////////////////////////////////////////////////////////////	

	//double* C = new double[16];
	//glGetDoublev(GL_MODELVIEW_MATRIX, C);

	//if (flag == 0)
	//{
	//	cout <<endl << endl<< "最初矩阵"<<endl; 
	//	for (int i = 0; i < 16; i++)
	//	{

	//	cout << C[i] << "   ";
	//if ((i + 1) % 4 == 0) cout << endl;
	//}
	//flag = 1;
	//}
	/////////////////////////////////////////////////////////////////////////////////

	glMultMatrixd(t[0]);//变换矩阵*原始单位矩阵  获得当前关节的最终坐标  
						//t[0]是地址 该函数会从地址中读取4*4 个double类型的数据 作为变换举证 与原始单位矩阵相乘 获得最终坐标


						/////////////////////////////////////////////////////////////////////////////////////////////////

						//  glGetDoublev(GL_MODELVIEW_MATRIX, C);

						//if (flag == 1)
						//{
						//  cout << endl << endl<< "t矩阵" << endl;
						//  for (int i = 0; i < 4; i++)
						//  {
						//	  for (int j = 0; j < 4; j++)
						//  cout <<(t[i])[j] << "   ";
						//	  cout << endl;
						//  }

						// cout << endl << endl << "最初矩阵*t矩阵"<<endl;
						//  for (int i = 0; i < 16; i++)
						//  {

						//	  cout << C[i] << "   ";
						//	  if ((i + 1) % 4 == 0) cout << endl;
						//  }
						//  flag = 2;
						// }
						///////////////////////////////////////////////////////////////////////////////////


						// Draw the shape
	if (drawSkeleton_)
		node->getShape()->draw();//调用shapecube的draw()
								 //draw()函数虽然是在画一个立方体 但是因为立方体0.5左右的大小 所以还是细木棍


								 //Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		evalArc(*i);// 画出  这个骨架节点的其他关联的关节对该骨架影响！！！！！
	}


	glPopMatrix();

}
