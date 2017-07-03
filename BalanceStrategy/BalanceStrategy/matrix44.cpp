#include "stdafx.h"
#include <math.h>
#include "matrix44.h"

#ifndef DEG2RAD
#define DEG2RAD  0.017453292519943295769236907684886
#endif

void Matrix44::Transform(const Vector3 &in, Vector3 &out) const
{
  double x, y, z, denom;
  x = in.getX() * matrix_[0][0] + 
      in.getY() * matrix_[1][0] + 
      in.getZ() * matrix_[2][0] +
      matrix_[3][0];

  y = in.getX() * matrix_[0][1] + 
      in.getY() * matrix_[1][1] + 
      in.getZ() * matrix_[2][1] +
      matrix_[3][1];

  z = in.getX() * matrix_[0][2] + 
      in.getY() * matrix_[1][2] + 
      in.getZ() * matrix_[2][2] +
      matrix_[3][2];

  out.set(x, y, z);

  denom = matrix_[0][3] + matrix_[1][3] + matrix_[2][3] + matrix_[3][3];
  if(denom != 1.0)
    out /= denom;
}

void Matrix44::SetTranslation(double x, double y, double z)
{
  SetIdentity();

  matrix_[3][0] = x;
  matrix_[3][1] = y;
  matrix_[3][2] = z;
}

void Matrix44::SetRotation(double angle, const Vector3& axis)
{
  Vector3 u(axis);
  double Cosine = cos(angle * DEG2RAD);
  double Sine = sin(angle * DEG2RAD);
  double oneMinus = 1.0 - Cosine;

  SetIdentity();

  u.Normalize();

  matrix_[0][0] = oneMinus * u.getX() * u.getX() + Cosine;
  matrix_[1][1] = oneMinus * u.getY() * u.getY() + Cosine;
  matrix_[2][2] = oneMinus * u.getZ() * u.getZ() + Cosine;
  matrix_[0][1] = oneMinus * u.getX() * u.getY() + Sine * u.getZ();
  matrix_[1][0] = oneMinus * u.getX() * u.getY() - Sine * u.getZ();
  matrix_[0][2] = oneMinus * u.getX() * u.getZ() - Sine * u.getY();
  matrix_[2][0] = oneMinus * u.getX() * u.getZ() + Sine * u.getY();
  matrix_[1][2] = oneMinus * u.getY() * u.getZ() + Sine * u.getX();
  matrix_[2][1] = oneMinus * u.getY() * u.getZ() - Sine * u.getX();
}

void Matrix44::SetRotationX(double angle)
{
  double Cosine = cos(angle * DEG2RAD);
  double Sine = sin(angle * DEG2RAD);

  SetIdentity();

  matrix_[1][1] = Cosine;
  matrix_[2][1] = -Sine;
  matrix_[1][2] = Sine;
  matrix_[2][2] = Cosine;
}

void Matrix44::SetRotationY(double angle)
{
  double Cosine = cos(angle * DEG2RAD);
  double Sine = sin(angle * DEG2RAD);

  SetIdentity();

  matrix_[0][0] = Cosine;
  matrix_[2][0] = -Sine;
  matrix_[0][2] = Sine;
  matrix_[2][2] = Cosine;
}

void Matrix44::SetRotationZ(double angle)
{
  double Cosine = cos(angle * DEG2RAD);
  double Sine = sin(angle * DEG2RAD);

  SetIdentity();

  matrix_[0][0] = Cosine;
  matrix_[1][0] = -Sine;
  matrix_[0][1] = Sine;
  matrix_[1][1] = Cosine;
}

void Matrix44::SetScale(double x, double y, double z)
{
  SetIdentity();

  matrix_[0][0] = x;
  matrix_[1][1] = y;
  matrix_[2][2] = z;
}

void Matrix44::SetView(const Vector3& LookAt, const Vector3& Viewer, const Vector3& UpL)
{
  int i;
  Matrix44 M, T;
  Vector3 U, V;
  Vector3 Up = UpL;
  Vector3 N = LookAt - Viewer;

  M.SetIdentity();
  T.SetTranslation(-Viewer[0], -Viewer[1], -Viewer[2]);
  
  Up.Normalize();
  N.Normalize();

  V = Up - (N * Up.Dot(N));
  V. Normalize();

  U.Cross(N, V);
  U.Normalize();

  for (i = 0; i < 3; i++)
  {
    M.matrix_[i][0] = U[i]; 
    M.matrix_[i][1] = V[i]; 
    M.matrix_[i][2] = N[i]; 
  }

  Mult(T, M);
}

//Do perspective projection with near and far clipping planes and window height
void Matrix44::SetPerspective(double D, double F, double H)
{
  SetIdentity();

  matrix_[2][2] = (H * F) / (D * (F - D));
  matrix_[2][3] = H/D;
  matrix_[3][2] = ((-1.0 * H) * F) / (F - D);
  matrix_[3][3] = 0.0;
}

//Project with focal length
void Matrix44::SetPerspective(double depth)
{
  SetIdentity();

  matrix_[2][3] = 1.0/depth;
  matrix_[3][3] = 0.0;
}
