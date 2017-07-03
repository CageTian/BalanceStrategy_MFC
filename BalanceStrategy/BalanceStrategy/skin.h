#ifndef SKIN_H
#define SKIN_H

#include <string>
#include <vector>
#include "matrix44.h"
#include "skeleton.h"
#include "vector3.h"

/// Class defining a skin mesh to be attached to the skeleton entity
class Skin
{
public:
  /// Structure defining a vertex to be attached to one or more bones of a skeleton
  struct Vertex
  {
  public:
    /// Structure defining attachment of a skin vertex to a single bone
    struct Attachment
    {
    public:
      /// Name of the bone to which vertex is attached
      std::string name_;
      /// Position of vertex relative to the bone
      Vector3 displacement_;
      /// Weight to adjust effect of bone motion on the vertex
      double weight_;
    public:
      /// Constructor
      Attachment(const std::string& name, const Vector3& displacement, double weight) : name_(name), displacement_(displacement), weight_(weight) { }
    };
  public:
    /// Vector to store vertex attachments
    typedef std::vector<Attachment*> AttachmentList_t;
  public:
    /// List of bone attachments for the vertex
    AttachmentList_t attachments_;
    /// World space coordinate of vertex computed from bone attachments
    Vector3 position_;
  public:
    /// Compute world space coordinate of vertex
    void compute(Skeleton::ArcTransformVector_t& transforms);
  };
public:
  /// Vector to stor skin vertices
  typedef std::vector<Vertex*> VertexList_t;
protected:
  /// Map containing world space transformations of the bones from skeleton to which the skin is attached
  Skeleton::ArcTransformVector_t transforms_;
  /// List of vertices that make up the skin
  VertexList_t verts_;
public:
  /// Constructor
  Skin() { }

  /// Destructor
  virtual ~Skin() { }

  /// Initialize the skin
  virtual void create() = 0;

  /// Retrieve the list of vertices
  VertexList_t* getVertices() const { return (VertexList_t*)&verts_; }

  /// Update the list of world space transformations for the bones from skeleton to which the skin is attached
  virtual void update(Skeleton* skel);

  /// Draw the skin
  virtual void draw() = 0;
};

#endif
