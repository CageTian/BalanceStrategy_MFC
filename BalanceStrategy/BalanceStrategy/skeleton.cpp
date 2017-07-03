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
	visitor(arc);    //�ȸ�ֵ���ؽ�root�����ɶ�

					 //Process the children
	SkelNode* node = arc->getNode();//Ȼ���ùؽ��������ĹǼ�
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)//�ҵ��ùǼ����󶨵������ؽ�
	{
		visitNextArc(*i, visitor);//�ݹ� ��ֵ���ɶ�
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
	//	cout <<endl << endl<< "�������"<<endl; 
	//	for (int i = 0; i < 16; i++)
	//	{

	//	cout << C[i] << "   ";
	//if ((i + 1) % 4 == 0) cout << endl;
	//}
	//flag = 1;
	//}
	/////////////////////////////////////////////////////////////////////////////////

	glMultMatrixd(t[0]);//�任����*ԭʼ��λ����  ��õ�ǰ�ؽڵ���������  
						//t[0]�ǵ�ַ �ú�����ӵ�ַ�ж�ȡ4*4 ��double���͵����� ��Ϊ�任��֤ ��ԭʼ��λ������� �����������


						/////////////////////////////////////////////////////////////////////////////////////////////////

						//  glGetDoublev(GL_MODELVIEW_MATRIX, C);

						//if (flag == 1)
						//{
						//  cout << endl << endl<< "t����" << endl;
						//  for (int i = 0; i < 4; i++)
						//  {
						//	  for (int j = 0; j < 4; j++)
						//  cout <<(t[i])[j] << "   ";
						//	  cout << endl;
						//  }

						// cout << endl << endl << "�������*t����"<<endl;
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
		node->getShape()->draw();//����shapecube��draw()
								 //draw()������Ȼ���ڻ�һ�������� ������Ϊ������0.5���ҵĴ�С ���Ի���ϸľ��


								 //Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		evalArc(*i);// ����  ����Ǽܽڵ�����������ĹؽڶԸùǼ�Ӱ�죡��������
	}


	glPopMatrix();

}
