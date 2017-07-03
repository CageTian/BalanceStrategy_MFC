#ifndef BASICTERRAIN_H
#define BASICTERRAIN_H

#include <vector>
#include "terrain.h"

class Shape;

/// Class defining the basic terrain, with stairs and an incline, for the human skeleton animation
class BasicTerrain : public Terrain
{
public:
  /// Structure defining 2D representation of terrain surface for determining terrain height at a specific position
  struct Line2D
  {
    /// Line endpoint
    double x1_, y1_, x2_, y2_;
    /// Constructor
    Line2D(double x1, double y1, double x2, double y2) : x1_(x1), y1_(y1), x2_(x2), y2_(y2) { }
  };
public:
  /// Vector containing lines defining terrain path
  typedef std::vector<Line2D*> LineList_t;
  /// Vector containg shapes that represent the terrain
  typedef std::vector<Shape*> ShapeList_t;
  LineList_t path_;
protected:
  /**< The path along which the actor will walk; 
                                  for obtaining height - only consider (z, y) coordinate;
                                  no change in height wrt x */
  ShapeList_t shapes_;       /**< Shapes that form terrain */
public:
  /// Constructor
  BasicTerrain();

  /// Destructor
  virtual ~BasicTerrain();

  /// Get height at specified (x,z) position
  virtual double getHeight(double x, double z);

  /// Get the angle of the terrain at the specified (x,z) position
  virtual double getAngle(double x, double z);

  /// Draw the terrain
  virtual void draw();
};

#endif
