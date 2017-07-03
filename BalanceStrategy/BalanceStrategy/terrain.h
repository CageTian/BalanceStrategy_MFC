#ifndef TERRAIN_H
#define TERRAIN_H

#include "shape.h"

/// Class to define a terrain over which an entity may walk
class Terrain : public Shape
{
public:
  /// Constructor
  Terrain() { }

  /// Destructor
  virtual ~Terrain() { }

  /// Get y corrdinate for specified (x,z) to obtain terrain height at given position
  virtual double getHeight(double x, double z) = 0;

  /// Get the angle of the terrain at the specified (x,z) position
  virtual double getAngle(double x, double z) = 0;

  /// Draw the terrain
  virtual void draw() = 0;
};

#endif
