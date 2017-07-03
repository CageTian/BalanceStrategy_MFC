#ifndef HUMANWALKANIMATION_H
#define HUMANWALKANIMATION_H

#include <map>
#include<GL/glut.h>
#include "ikframer.h"
#include "skeleton.h"
#include "humanskeleton.h"
#include "humanwalkframer.h"
#include "animation.h"

class Terrain;

/// Class to animate a human character walking across terrain
class HumanWalkAnimation : public Animation
{
public:
  /// Class containg the actor to animate with IK framers to adjust leg position when walking on uneven terrain
  class HumanWalkActor : public Actor
  {
  public:
    /// IKFramer to adjust left leg's elevation on uneven terrain
    IKFramer leftleg_;
    /// IKFramer to adjust right leg's elevation on uneven terrain
    IKFramer rightleg_;
  public:
    /// Constructor
    HumanWalkActor(HumanSkeleton* skeleton, HumanWalkFramer* framer) : Actor(skeleton, framer) {}

    /// Destructor
    virtual ~HumanWalkActor() { }
  };
protected:
  /// Terrain over which actors will walk
  Terrain* terrain_;
public:
  /// Constructor
  HumanWalkAnimation(Terrain* terrain) : terrain_(terrain) { }

  /// Destructor
  ~HumanWalkAnimation() { }

  /// New add actor which will create inverse kinematic framers for the legs
  void addActor(HumanSkeleton* skeleton, HumanWalkFramer* framer);
  void setAngles( char * src, GLdouble angle);
  /// Update the frame
  virtual void update();
};

#endif
