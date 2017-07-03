#ifndef SHAPERHOMBOID_H
#define SHAPERHOMBOID_H

#include "shape.h"

/// Class to draw a 3D rhombus
class ShapeRhomboid : public Shape
{
protected:
  /// Draw a single quad
  void polygon(int a, int b, int c, int d, int facenum);
public:
  /// Draw the rhomboid
  virtual void draw();
};

#endif
