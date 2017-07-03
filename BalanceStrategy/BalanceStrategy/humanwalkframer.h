#ifndef HUMANWALKFRAMER_H
#define HUMANWALKFRAMER_H

#include "fkframer.h"

/// Class extending the forward kinematic framer to define a walking animation
class HumanWalkFramer : public FKFramer
{
protected:
  /// Create the key skeletal positions for the walk animation
  void setupFrames();

public:
  /// Constructor
  HumanWalkFramer();
  

};

#endif
