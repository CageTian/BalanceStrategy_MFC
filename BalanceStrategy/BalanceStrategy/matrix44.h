#ifndef MATRIX44_H
#define MATRIX44_H

#include "vector3.h"
#include "matrix.h"

class Matrix44 : public Matrix
{
public:
  Matrix44() : Matrix(4, 4) { }

  // this = A * B - adjusted for column major ordering used for interoperability with OpenGL
  virtual bool Mult(const Matrix& A, const Matrix& B) { return Matrix::Mult(B, A); }

  void Transform(const Vector3 &in, Vector3 &out) const;

  void SetTranslation(double x, double y, double z);
  void SetTranslation(const Vector3& t) { SetTranslation(t.getX(), t.getY(), t.getZ()); }
  void SetRotation(double angle, double x, double y, double z) { SetRotation(angle, Vector3(x, y, z)); }
  void SetRotation(double angle, const Vector3& axis);
  void SetRotationX(double angle);
  void SetRotationY(double angle);
  void SetRotationZ(double angle);
  void SetScale(double x, double y, double z);
  void SetScale(const Vector3& s) { SetScale(s.getX(), s.getY(), s.getZ()); }

  // Set the camera position
  void SetView(const Vector3& LookAt, const Vector3& Viewer, const Vector3& UpL);

  //Do perspective projection with near and far clipping planes and window height
  void SetPerspective(double D, double F, double H);

  //Project with focal length
  void SetPerspective(double depth);

  Vector3 operator*(const Vector3& V) const { Vector3 out; Transform(V, out); return out; }
};

#endif
