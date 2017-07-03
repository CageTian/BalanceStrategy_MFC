#ifndef FRAMER_H
#define FRAMER_H

#include "animation.h"
#include<iostream>
using namespace std;
/// Base class for animating an entity
class Framer
{
protected:
	/// Animation rate
	double rate_;
public:
	/// Get animation rate
	double getRate() const { return rate_; }

	/// Set anumation rate
	void setRate(double rate) { rate_ = rate; }

	/// Update the animation
	virtual void update(Animation::angles_t& angles) = 0;
	virtual void addFrame(Animation::angles_t angles_) {};
	/// Destructor
	virtual ~Framer() { }
};

#endif
