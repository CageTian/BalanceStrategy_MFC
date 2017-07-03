#ifndef SHAPEBONE_H
#define SHAPEBONE_H

#include "shapecube.h"

/// The shape to represent a bone - a thin rectangle extended in the direction of either 
/// the y (vertical) or z (horizontal) direction
class ShapeBone : public ShapeCube
{
public:
  /// Bone orientation
  enum EBoneType
  {
    /// A vertical bone
    VERTICAL,
    /// A horizontal bone aligned with x axis
    HORIZONTALX,
    /// A horizontal bone aligned with z axis
    HORIZONTALZ
  };
public:
  /// The fixed width of the bone
  static const double BONEWIDTH;
  /// The width of the bone divided by two
  static const double BONEWIDTH_2;     // Bonewidth / 2.0
protected:
  /// Type of bone
  EBoneType type_;
  /// Length of bone
  double length_;
public:
  /// Constructor
  ShapeBone(EBoneType type, double length);

  /// Get the bone type
  EBoneType getType() const { return type_; }

  /// Get the bone length
  double getLength() const { return length_; }

  /// Set the bone length
  void setLength(double length);
};

#endif
