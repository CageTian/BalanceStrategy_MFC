#ifndef SHAPE_H
#define SHAPE_H

#include "vector3.h"

/// Base class defining a shape object
class Shape
{
protected:
  /// Scale to apply to shape
  Vector3 scale_;
  /// Rotation to apply to shape
  double rotAngle_;
  /// Axis of rotation for shape
  Vector3 rotAxis_;
  /// Translation to apply to shape
  Vector3 trans_;
public:
  /// Constructor
  Shape() : scale_(1.0, 1.0, 1.0), rotAngle_(0.0) { }

  /// Destructor
  virtual ~Shape() { }

  /// Draw the shape
  virtual void draw() = 0;

  /// Get scale value
  Vector3* getScale() const { return (Vector3*)&scale_; }

  /// Get rotation angle
  double getRotationAngle() const { return rotAngle_; }

  /// Get rotation axis
  Vector3* getRotationAxis() const { return (Vector3*)&rotAxis_; }

  /// Get translation value
  Vector3* getTranslation() const { return (Vector3*)&trans_; }

  /// Set scale value as 3 doubles
  void setScale(double x, double y, double z) { scale_.set(x, y, z); }

  /// Set scale value as Vector3
  void setScale(const Vector3& scale) { scale_ = scale; }

  /// Set rotation angle
  void setRotationAngle(double angle) { rotAngle_ = angle; }

  /// Set rotation axis as 3 doubles
  void setRotationAxis(double x, double y, double z) { rotAxis_.set(x, y, z); }

  /// Set rotation axis as Vector3
  void setRotationAxis(const Vector3& rot) { rotAxis_ = rot; }

  /// Set translation axis as 3 doubles
  void setTranslation(double x, double y, double z) { trans_.set(x, y, z); }

  /// Set translation axis as Vector3
  void setTranslation(const Vector3& trans) { trans_ = trans; }
};

#endif
