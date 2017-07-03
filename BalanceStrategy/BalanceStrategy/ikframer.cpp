#include "stdafx.h"
#include "shape.h"
#include "ikframer.h"
#include<iostream>
using namespace std;
#ifndef M_PI
#define M_PI     3.1415926535897932384626433832795
#endif
#ifndef M_PI_2
#define M_PI_2   1.5707963267948966192313216916398
#endif
#ifndef DEG2RAD
#define DEG2RAD  0.017453292519943295769236907684886
#endif

#define EPSILON  0.01

IKSolver::InfoGetter::InfoGetter()
: total_(0)
{ 
  G_.SetIdentity();
}

void IKSolver::InfoGetter::reset()
  {
    total_ = 0;
    G_.SetIdentity();
    positions_.clear();
    names_.clear();
    angles_.clear();
    axes_.clear();
  }

void IKSolver::InfoGetter::operator()(SkelArc* arc)//计算 关节的旋转轴变换后的位置
{
  Vector3 p, a;
  Matrix44 t, r;
  //cout << "wxtest" << arc->getName() << endl;
  // Update the global matrix
  t.SetTranslation(arc->getTranslation()->getX(), arc->getTranslation()->getY(), arc->getTranslation()->getZ());
  r.SetRotation(arc->getRotationAngle()->getValue(), Vector3(arc->getRotationAxis()->getX(), arc->getRotationAxis()->getY(), arc->getRotationAxis()->getZ()));

  t *= r;
  G_ *= t;

  // Calculate joint position
  p = G_ * p;

  positions_.push_back(p);//获取此时关节的坐标

  // Transform axis of rotation
  Matrix hg(4,1); //旋转轴
  hg[0][0] = arc->getRotationAxis()->getX(); 
  hg[1][0] = arc->getRotationAxis()->getY(); 
  hg[2][0] = arc->getRotationAxis()->getZ(); 
  hg[3][0] = 0.0; 

  Matrix hh;
  hh.Mult(G_, hg);

  a.set(hg[0][0], hg[1][0], hg[2][0]); //获得旋转轴变换后的新轴
  a.Normalize();//归一化
  axes_.push_back(a);

  angles_.push_back(*arc->getRotationAngle());
  names_.push_back(arc->getName());

 // cout << "look!!!!  " << arc->getName()<<"   " << p.getX() << " " <<p.getY()
	//  << "  " << p.getZ()<<endl;
  total_++;
}

class AngleSetter : public Skeleton::ArcVisitor
{
protected:
  Animation::angles_t * angles_;
public:
  AngleSetter(Animation::angles_t* angles) : angles_(angles) { }
  virtual void operator()(SkelArc* arc)
  {
    Animation::angles_t::const_iterator iter = angles_->find(arc->getName());
    if(iter != angles_->end())
      arc->setRotationAngle(iter->second);
  }
};

IKSolver::IKSolver(const Skeleton* skel, const std::string& root, const std::string& endeffector)
: stepScale_(0.1)
{
  // Find the "root" of our skeleton
  SkelArc* oldArc = skel->findArc(root);
  if(oldArc)
  {
    SkelNode* node = new SkelNode(oldArc->getNode()->getName(), NULL);

    // The new root will be at the origin, so the translations will not be copied
    SkelArc* arc = new SkelArc(oldArc->getName(), node);
    arc->setRotationAngle(*oldArc->getRotationAngle());
    arc->setRotationAxis(*oldArc->getRotationAxis());
//    arc->setTranslation(*oldArc->getTranslation());
    skel_.setRootArc(arc);

    SkelArc* parent = oldArc;
    SkelArc* lastArc = arc;

    skelarcs_t::iterator iter = parent->getNode()->begin();
    while(iter != parent->getNode()->end() && parent->getName() != endeffector)
    {
      SkelArc* oldArc = *iter;
      node = new SkelNode(oldArc->getNode()->getName(), NULL);
      arc = new SkelArc(oldArc->getName(), node);
      arc->setRotationAngle(*oldArc->getRotationAngle());
      arc->setRotationAxis(*oldArc->getRotationAxis());
      arc->setTranslation(*oldArc->getTranslation());

      lastArc->getNode()->addArc(arc);

      parent = oldArc;
      lastArc = arc;

      iter = parent->getNode()->begin();
    }
  }
}

void IKSolver::computeJacobian(const InfoGetter& ig)
{
  // The endeffector is at the last position in the InfoGetter's arc vector
  unsigned int i, endeffector = ig.total_ - 1;
  std::vector<Vector3> vs;

  // Set the values for v in v = Jt
  std::vector<Vector3>::const_iterator citer;
  for (i = 0; i < endeffector; i++)
  {
	  vs.push_back(ig.positions_[endeffector] - ig.positions_[i]);
	 
  }


  // Get longest distance from origin
  std::vector<Vector3>::iterator iter = vs.begin();
  double length = (*iter).Mag();
  iter++;
  for(; iter != vs.end(); ++iter)
  {
    double l = (*iter).Mag();
    if(l > length)
      length = l;
  }


  //以下为源码 但是被屏蔽  因为不知道具体什么用 而且屏蔽后无太大影响
  //////////////////////////////////////////////////////////
  // Scale all of the vectors to (0..1) range
  //for(iter = vs.begin(); iter != vs.end(); ++iter)
   // (*iter) /= length;


  Vector3 temp;
  J_.Resize(3, endeffector);
  for(i = 0; i < endeffector; i++)
  {
    temp.Cross(ig.axes_[i], vs[i]);
    J_[0][i] = temp.getX();
    J_[1][i] = temp.getY();
    J_[2][i] = temp.getZ();
  }//雅可比矩阵已经求出 即J_


  //以下在求J_的逆矩阵  也就是雅可比矩阵的逆矩阵
  ///////////////////////////////////////////////////////////////////
  JT_.Copy(J_);
  JT_.Transpose();
  Matrix JJTI;
  JJTI.Mult(J_, JT_);//矩阵*转置矩阵
  JJTI.Invert();//仿射矩阵 
  JTJJTI_.Mult(JT_, JJTI);
/////////////////////////////////////////////////////////////////////

  //验证结果  可看出 相乘结果为单位矩阵
 // cout << "look 看看JTJJTI_ * J_的结果哈~~~~~~~~~~~~~" << endl;
  Matrix haha;
  haha.Mult(J_, JTJJTI_);
  /*for (i = 0; i< haha.getN(); i++)
  {
	  for (int j = 0; j <haha.getM(); j++)

		  cout << haha[i][j] << "  ";
	  cout << endl;
  }*/

}

void IKSolver::solve(Animation::angles_t& angles)
{
  unsigned int i, j, endeffector;
  Vector3 v;

  InfoGetter ig;
  cout << endl << endl;
  // Update the angles of the chain that was copied from the initial skeleton
  AngleSetter update(&angles);
  skel_.visitArcs(update);//利用update 的angles的值  给skel骨架中各个关节赋值 更新skel骨架关节

  for(i = 0; i < 50; i++)
  {
    // Get joint angles and positions
    skel_.visitArcs(ig);    //更新旋转轴  记录 关节的坐标 旋转角度 旋转轴

    // Endeffector is the last item in the InfoGetter's arc vector
    endeffector = ig.total_ - 1;

    // Calculate the end effector   //计算末端效应器
    v = goal_ - ig.positions_[endeffector];   //目标-末端效应器的坐标   所有的坐标计算都是基于root的
	//v.setZ(0);
/*
	cout << "末端效应器的坐标为  " << ig.names_[endeffector] << ig.positions_[endeffector].getX() << " " << ig.positions_[endeffector].getY()
		<< "  " << ig.positions_[endeffector].getZ() << endl
	<< goal_.getX() << "  " << goal_.getY() << "  " << goal_.getZ() << endl;
	cout <<"hi dear  "<< v.getX() << "  " << v.getY() << "   " << v.getZ() << endl;
	cout << "z的差值  " << goal_.getZ() - ig.positions_[endeffector].getZ() << endl;*/
	computeJacobian(ig);

    if(v.Mag() < EPSILON) //到目标的距离 小于0.01调整就结束
      break;

	v *= 10; //速度太慢会显得比较卡 所以这边的速度应该控制一下


//////////////////////////////////////////////////////////////
	//以下是源代码
   // v *= stepScale_;//速度？？？？ 根本没有影响哈   =[n*(x,y,z)]/(根号((xn)^2+(yn)^2+(zn)^2))
    //v.Normalize();//标准化                          =n*(x,y,z)/(n*(根号x^2+y^2+z^2))
	                                            //  =(x,y,z)/(根号x^2+y^2+z^2)
/////////////////////////////////////////////////////////////////////////////////////



  // cout << "look the speed "<<v.getX() << "  " << v.getY() << "  " << v.getZ() << endl;
    
	
	Matrix V(3, 1);
    V[0][0] = v.getX();
    V[1][0] = v.getY();
    V[2][0] = v.getZ();

    Matrix ThetaDot;
    ThetaDot.Mult(JTJJTI_, V);

    double temp = ThetaDot[2][0];
    for(j = 0; j < ig.total_ - 1; j++)
    {
      angles[ig.names_[j]].setValue(ig.angles_[j].getValue() + ThetaDot[j][0]);//每个关节应该旋转的角度
    }

    // Apply the angles and recalculate end effector position
    AngleSetter as(&angles);
    skel_.visitArcs(as);

    ig.reset();
  }
}

void IKFramer::addChain(const std::string& name, const Skeleton* skel, const std::string& root, const std::string& endeffector)
{
  iksolvers_.insert(std::make_pair(name, IKSolver(skel, root, endeffector)));
}

void IKFramer::setGoal(const std::string& name, double x, double y, double z)
{
  iksolvers_t::iterator iter = iksolvers_.find(name);
  if(iter != iksolvers_.end())
    iter->second.setGoal(x, y, z);
}

void IKFramer::update(Animation::angles_t& angles)
{
  iksolvers_t::iterator iter;
  for(iter = iksolvers_.begin(); iter != iksolvers_.end(); ++iter)
  {
    iter->second.solve(angles);
  }
}
