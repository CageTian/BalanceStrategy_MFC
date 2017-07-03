#ifndef HUMANSKELETON_H
#define HUMANSKELETON_H

#include <map>
#include "skeleton.h"
#include<iostream>
#include "HumanSimulation.h"
using namespace std;

/// Class extending the skeleton class to create a human skeleton
class HumanSkeleton : public Skeleton
{
	HumanData humanData;
public:
	std::map<string, double> getAngles();
	/// Functor to initialize the angles of the skeletal joints
	class AngleInitializer : public Skeleton::ArcVisitor
	{
	public:
		/// Map of angles for each named joint
		std::map<std::string, SkelDOF> angles;//dof是自由度
	public:
		/// Operation to apply angle to joint
		virtual void operator()(SkelArc* arc)//找到指定关节预先初始化的自由度 并赋值给这个关节
		{
			std::map<std::string, SkelDOF>::const_iterator iter = angles.find(arc->getName());
			if (iter != angles.end())
			{
				arc->setRotationAngle(iter->second);

			}
		}
	};
protected:
	/// Set initial skeletal pose

	/// Create the skeleton
	virtual void buildSkeleton();
	/// Recursively destroy all of the descendents of a joint
	void destroyChildren(const SkelArc* arc);
	/// Destroy the entire skeleton
	virtual void destroySkeleton() { destroyChildren(root_); delete root_; root_ = NULL; }
public:
	/// Constructor
	HumanSkeleton(HumanData h);
	void initializeAngles();
	void setAngles(std::map<std::string, double>angles_);
	void setArcAngle(std::string name, double angle);
	void setArcPosition(std::string name, Vector3 v);
	double getArcAngle(std::string name);
	/// Destructor
	virtual ~HumanSkeleton();

	/// Initialize the skeleton
	virtual void create();
};

#endif
