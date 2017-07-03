#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class Vector3
{
protected:
  double x_;
  double y_;
  double z_;
public:
  Vector3() : x_(0.0), y_(0.0), z_(0.0) { }
  Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) { }
  Vector3(const Vector3& v) : x_(v.x_), y_(v.y_), z_(v.z_) { }
  double getX() const { return x_; }
  double getY() const { return y_; }
  double getZ() const { return z_; }

  void setX(double x) { x_ = x; }
  void setY(double y) { y_ = y; }
  void setZ(double z) { z_ = z; }
  void set(double x = 0.0, double y = 0.0, double z = 0.0) { x_ = x; y_ = y; z_ = z; }

  double Norm() const { return ((x_*x_) + (y_*y_) + (z_*z_)); }
  double Mag() const { return sqrt(Norm()); }
  Vector3& Normalize() { double m = Mag(); if(m != 0.0) *this /= m; return *this; }

  double Dot(const Vector3& v) const { return (x_ * v.x_) + (y_ * v.y_) + (z_ * v.z_); }
  Vector3& Cross(const Vector3& a, const Vector3& b) { x_ = a.y_ * b.z_ - a.z_ * b.y_; y_ = a.z_ * b.x_ - a.x_ * b.z_; z_ = a.x_ * b.y_ - a.y_ * b.x_; return *this; }

  //Unary operators
  Vector3 operator+() const { return *this; }
  Vector3 operator-() const { return Vector3(-x_, -y_, -z_); }

  //Assignment operators
  Vector3& operator=(const Vector3& v) { x_ = v.x_; y_ = v.y_; z_ = v.z_; return *this; }
  Vector3& operator+=(const Vector3& v) { x_ += v.x_; y_ += v.y_; z_ += v.z_; return *this; }
  Vector3& operator-=(const Vector3& v) { x_ -= v.x_; y_ -= v.y_; z_ -= v.z_; return *this; }
  Vector3& operator*=(const Vector3& v) { x_ *= v.x_; y_ *= v.y_; z_ *= v.z_; return *this; }
  Vector3& operator*=(double s) { x_ *= s; y_ *= s; z_ *= s; return *this; }
  Vector3& operator/=(const Vector3& v) { x_ /= v.x_; y_ /= v.y_; z_ /= v.z_; return *this; }
  Vector3& operator/=(double s) { x_ /= s; y_ /= s; z_ /= s; return *this; }

  //Binary operators
  Vector3 operator+(const Vector3& v) const { return Vector3(x_ + v.x_, y_ + v.y_, z_ + v.z_); }
  Vector3 operator-(const Vector3& v) const { return Vector3(x_ - v.x_, y_ - v.y_, z_ - v.z_); }
  Vector3 operator*(const Vector3& v) const { return Vector3(x_ * v.x_, y_ * v.y_, z_ * v.z_); }
  Vector3 operator*(double s) const { return Vector3(x_ * s, y_ * s, z_ * s); }
  Vector3 operator/(const Vector3& v) const { return Vector3(x_ / v.x_, y_ / v.y_, z_ / v.z_); }
  Vector3 operator/(double s) const { return Vector3(x_ / s, y_ / s, z_ / s); }

  bool operator==(const Vector3& v) const { return (x_ == v.x_) && (y_ == v.y_) && (z_ == v.z_); }
  bool operator<(const Vector3& v) const { return (x_ < v.x_) && (y_ < v.y_) && (z_ < v.z_); }

  double& operator[](int i) { return ((double*)(&x_))[i]; }
  const double& operator[](int i) const { return ((double*)(&x_))[i]; }
};

#endif
