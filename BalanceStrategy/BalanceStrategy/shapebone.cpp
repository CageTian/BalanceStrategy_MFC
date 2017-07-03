#include "stdafx.h"
#include "shapebone.h"

const double ShapeBone::BONEWIDTH = 0.01;
const double ShapeBone::BONEWIDTH_2 = 0.005;     // Bonewidth / 2.0

ShapeBone::ShapeBone(EBoneType type, double length)
: type_(type),
  length_(length)
{
  if(type_ == HORIZONTALX)
    scale_.set(length, BONEWIDTH, BONEWIDTH);
  else if(type_ == HORIZONTALZ)
    scale_.set(BONEWIDTH, BONEWIDTH, length);
  else
    scale_.set(BONEWIDTH, length, BONEWIDTH);
}

void ShapeBone::setLength(double length)
{
  length_ = length;
  if(type_ == HORIZONTALX)
    scale_.set(length, BONEWIDTH, BONEWIDTH);
  else if(type_ == HORIZONTALZ)
    scale_.set(BONEWIDTH, BONEWIDTH, length);
  else
    scale_.set(BONEWIDTH, length, BONEWIDTH);
}
