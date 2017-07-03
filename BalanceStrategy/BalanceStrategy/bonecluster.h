#ifndef BONECLUSTER_H
#define BONECLUSTER_H

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <vector>
#include "vector3.h"
#include "shape.h"
#include "shapebone.h"

/// A cluster of bones for skeletal nodes that may require more than one bone for visual representation
class BoneCluster : public Shape
{
public:
  /// Vector containing the bones for the lcuster
  typedef std::vector<ShapeBone*> BoneList_t;
protected:
  /// List of bones for the cluster
  BoneList_t bones_;
public:
  /// Constructor
  BoneCluster() { }

  /// Destructor
  ~BoneCluster();

  /// Add a bone with specified type, length, and position/translation
  void addBone(ShapeBone::EBoneType type, double length, const Vector3& trans = Vector3(0.0, 0.0, 0.0));

  /// Draw the bones
  virtual void draw();
};

#endif
