#ifndef IKFRAMER_H
#define IKFRAMER_H

#include <string>
#include <map>

#include "skeleton.h"
#include "matrix.h"
#include "matrix44.h"
#include "vector3.h"
#include "framer.h"

/// Class defining an inverse kinematic solver//逆运动学
class IKSolver
{
public:
  /// Functor to retrieve information for skeletal joints//检索 骨骼单元信息
  class InfoGetter : public Skeleton::ArcVisitor
  {
  public:
    /// Total number of joints
    unsigned int total_;
    /// World transformation for skeleton
    Matrix44 G_;
    /// World space positions of joints
    std::vector<Vector3> positions_;
    /// Joint angles
    std::vector<SkelDOF> angles_;
    /// Joint rotation axes
    std::vector<Vector3> axes_;
    /// Joint names
    std::vector<std::string> names_;
  public:
    /// Constructor
    InfoGetter();

    /// Clear values
    void reset();

    /// Function to process a joint
    void operator()(SkelArc* arc);
  };
protected:
  /// NxM Jacobian matrix
  Matrix J_;
  /// NxM Jacobian matrix transpose
  Matrix JT_;
  /// JT(J.JT)^-1
  Matrix JTJJTI_;
  /// Goal position for end effector
  Vector3 goal_;
  /// Skeletal chain to process
  Skeleton skel_;
  /// Size for iterative step with Jacobian
  double stepScale_;
protected:
  /// Compute the Jacobian for the chain
  void computeJacobian(const InfoGetter& ig);
public:
  /// Constructor
  IKSolver(const Skeleton* skel, const std::string& root, const std::string& endeffector);

  /// Set the step size
  void setStepScale(double s) { stepScale_ = s; }

  /// Set the goal
  void setGoal(double x, double y, double z) { goal_.set(x, y, z); }

  /// Attempt to determine joint angles that will move end effector to goal
  void solve(Animation::angles_t& angles);
};

/// Class defining the framer to position a set of skeletal chains with inverse kinematics
class IKFramer : public Framer
{
public:
  /// Map containing a set of inverse kinematic solvers, each processing a single skeletal chain
  typedef std::map<std::string, IKSolver> iksolvers_t;
protected:
  /// The inverse kinematic solvers for the chains managed by the IK framer
  iksolvers_t iksolvers_;
public:
  /// Constructor
  IKFramer() { }

  /// Add a skeletal chain with a unique name to the IK framer
  void addChain(const std::string& name, const Skeleton* skel, const std::string& root, const std::string& endeffector);

  /// Set the goal of the skeletal chain specified by name
  void setGoal(const std::string& name, double x, double y, double z);

  /// Compute joint angles that achieve the specified goal position for each chain's end effector
  void update(Animation::angles_t& angles);
};

#endif
