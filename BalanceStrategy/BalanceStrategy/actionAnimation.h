#ifndef ACTIONANIMATION_H
#define ACTIONANIMATION_H

#include <map>
#include<GL/glut.h>
#include "ikframer.h"
#include "skeleton.h"
#include "humanskeleton.h"
#include "actionFramer.h"
#include "animation.h"



/// Class to animate a human character walking across terrain
class actionAnimation : public Animation
{
public:

	std::map<double, Point> Ankle;
	/// Class containg the actor to animate with IK framers to adjust leg position when walking on uneven terrain
	class actionActor : public Actor
	{
	public:
		/// IKFramer to adjust left leg's elevation on uneven terrain
		IKFramer leftleg_;
		/// IKFramer to adjust right leg's elevation on uneven terrain
		IKFramer rightleg_;
	public:
		/// Constructor
		actionActor(HumanSkeleton* skeleton, actionFramer* framer) : Actor(skeleton, framer) {}
		actionActor(HumanSkeleton* skeleton) : Actor(skeleton) {}
		void setFramer(actionFramer* framer);
		
		/// Destructor
		virtual ~actionActor() { }
	};
protected:
	/// Terrain over which actors will walk
	
public:
	/// Constructor
	actionAnimation(){ }

	/// Destructor
	~actionAnimation() { }
	void setAnklePoints(std::map<double, Point> a) { Ankle = a; }
	/// New add actor which will create inverse kinematic framers for the legs
	void addActor(HumanSkeleton* skeleton, actionFramer* framer);
	void setAngles(char * src, GLdouble angle);
	void setJacobi(char * name,Vector3 v);
	void  updateJacobi();
	void clearActor();
	/// Update the frame
	virtual void update();
};

#endif

