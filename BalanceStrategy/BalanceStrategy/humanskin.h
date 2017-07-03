#ifndef HUMANSKIN_H
#define HUMANSKIN_H

#include <string>
#include <vector>
#include "vector3.h"
#include "skin.h"
#include "HumanSimulation.h"
/// Class defining a skin for the human skeleton
class HumanSkin : public Skin
{
public:
	HumanData humanData;
  /// Structure defining a limb for the skin mesh
  struct Limb//Ö«£¬±Û£»·ÖÖ§£»Ö¦¸É
  {
  public:
    /// Type of OpenGL geometry (GL_QUAD_STRIP, etc)
    int type_;
    /// Color of the limb
    Vector3 color_;
    /// List of vertex normals
    VertexList_t normals_;
    /// List of vertices
    VertexList_t verts_;
  public:
    /// Constructor
    Limb(int type = GL_QUAD_STRIP, const Vector3& color = Vector3(1.0, 1.0, 1.0)) : type_(type), color_(color) { }
  };
public:
  /// List containing limbs
  typedef std::vector<Limb*> LimbList_t;
protected:
  /// List of limbs for the skin
  LimbList_t limbs_;
protected:
  /// Make an arm, attaching the vertices to the specified bones
  void makeArm(const std::string& shoulder, const std::string& elbow);
  /// Make a leg, attaching the vertices to the specified bones
  void makeLeg(const std::string& hip, const std::string& knee);
  /// Make a foot, attaching the vertices to the specified bones
  void makeFoot(const std::string& ankle);
  /// Make the torso
  void makeTorso();
  /// Make the neck
  void makeNeck();
  /// Make the head
  void makeHead();
  /// Make the left arm
  void makeLeftArm() { makeArm("LShoulderZ", "LElbow"); }
  /// Make the right arm
  void makeRightArm() { makeArm("RShoulderZ", "RElbow"); }
  /// Make the left leg
  void makeLeftLeg() { makeLeg("LHipZ", "LKnee"); }
  /// Make the right leg
  void makeRightLeg() { makeLeg("RHipZ", "RKnee"); }
  /// Make the left foot
  void makeLeftFoot() { makeFoot("LAnkle"); }
  /// Make the right foot
  void makeRightFoot() { makeFoot("RAnkle"); }
public:
  /// Constructor
	HumanSkin(HumanData h) { humanData = h; }

  /// Destructor
  virtual ~HumanSkin();

  /// Initialize the skin
  virtual void create();

  /// Draw the skin
  virtual void draw();
};

#endif
