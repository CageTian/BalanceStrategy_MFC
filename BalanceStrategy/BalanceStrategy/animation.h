#ifndef ANIMATION_H
#define ANIMATION_H

#include <map>
#include <string>
#include "skeleton.h"

class Framer;

/// A class for skeletal animation with a specified type of framer
class Animation
{
public:
  /// Map fo angles for skeletal joints
  typedef std::map<std::string, SkelDOF> angles_t;
public:
  /// Class containing an actor to animate
  class Actor
  {
  public:
    /// Actor skeleton
    Skeleton* skeleton_;
    /// Framer for posing/animating skeleton
    Framer* framer_;     //rate 比率
    /// The angles fot the skeleton  
    angles_t angles_;  //骨架的角度
  public:
    /// Constructor
    Actor(Skeleton* skeleton, Framer* framer) : skeleton_(skeleton), framer_(framer) { }
	Actor(Skeleton* skeleton) : skeleton_(skeleton) {}

    /// Destructor
    virtual ~Actor() { }
  };
public:
  /// Vector containing a list of animable actors
  typedef std::vector<Actor*> actors_t;
protected:
  /// Functor to obtain the angles of a skeleton's joints
  class ActorInitializer : public Skeleton::ArcVisitor
  {
  protected:
    /// Actor to process
    Actor* a_;
  public:
    /// Constructor
    ActorInitializer(Animation::Actor* a) : a_(a) { }

    /// Operation to retrieve joint angle
    virtual void operator()(SkelArc* arc)
    {
      a_->angles_.insert(std::make_pair(arc->getName(), *arc->getRotationAngle()));
    }
  };
protected:
  /// Functor to set the angles of a skeleton's joints
  class ActorUpdater : public Skeleton::ArcVisitor
  {
  protected:
    /// Actor to process
    Actor* a_;
  public:
    /// Constructor
    ActorUpdater(Actor* a) : a_(a) { }

    /// Operation to set joint angle
    virtual void operator()(SkelArc* arc)
    {
      angles_t::const_iterator iter = a_->angles_.find(arc->getName());
      if(iter != a_->angles_.end())
        arc->setRotationAngle(iter->second);
    }
  };
protected:
  /// The actors for the animation
  actors_t actors_;
protected:
  /// Add an actor to the animation
  void addActor(Actor* actor);
public:
  /// Constructor
  Animation() { }

  /// Destructor
  virtual ~Animation();

  /// Add an actor to the animation
  void addActor(Skeleton* skeleton, Framer* framer) { addActor(new Actor(skeleton, framer)); }

  /// update the frame
  virtual void update();

  /// draw the current frame
  virtual void draw();

  /// Retrieve begining of vector
  actors_t::iterator begin() { return actors_.begin(); }

  /// Retrieve begining of vector
  actors_t::const_iterator begin() const { return actors_.begin(); }

  /// Retrieve ending of vector
  actors_t::iterator end() { return actors_.end(); }

  /// Retrieve ending of vector
  actors_t::const_iterator end() const { return actors_.end(); }
};

#endif
