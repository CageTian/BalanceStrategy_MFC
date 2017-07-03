#ifndef ACTIONFRAMER_H
#define  ACTIONFRAMER_H
#include<gl/glut.h>
#include "fkframer.h"
#include<iostream>
#include "StepStrategy.h"
using namespace std;

/// Class extending the forward kinematic framer to define a walking animation
class actionFramer : public FKFramer
{
protected:
	/// Create the key skeletal positions for the walk animation


public:
	/// Constructor
	actionFramer() {};
	void  addFrame(char* arc, GLdouble angles, GLdouble newangles);
	void  addFrame(Animation::angles_t angles_);
	void  addStepFrame(std::map<double, double> COMData, std::map<double, Offset> offset, double rate);
	void clearFrame();
	void  addFrame(std::map<string, double> angles_);
	void addEndFramer(std::map<string, double> angles_);
};

#endif

