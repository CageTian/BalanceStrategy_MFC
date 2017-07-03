#ifndef SHAPECIRCLE_H
#define  SHAPECIRCLE_H

#include "shape.h"

/// Class defining a cube
class ShapeCircle : public Shape
{
protected:
	/// Draw a single quad
	void polygon(int a, int b, int c, int d, int facenum);
public:
	/// Draw the cube
	virtual void draw();
};

#endif
