#ifndef FKFRAMER_H
#define FKFRAMER_H

#include <map>
#include <vector>
#include <string>
#include "animation.h"
#include "timer.h"
#include "framer.h"

/// Class defining a key frame pose for a skeleton
class FKFrame
{
public:
  /// Map containing the angles for a set of joints, defining key frame pose for a skeleton
  typedef std::map<std::string, double> frameAngles_t;
protected:
  /// Duration of the frame
  double duration_; //获得帧动画的时间
  /// Key frame pose for skeleton
  frameAngles_t frameAngles_;

public:
  /// Constructor
  FKFrame() : duration_(0.0) { }

  /// Get the duration
  double getDuration() const { return duration_; }

  /// Set the duration
  void setDuration(double duration) { duration_ = duration; }
  
  /// Add an angle for a skeletal joint
  void addAngle(const std::string& name, double angle) { frameAngles_.insert(std::make_pair(name, angle)); }

  /// Get begining of joint angle map
  frameAngles_t::iterator begin() { return frameAngles_.begin(); }

  /// Get begining of joint angle map
  frameAngles_t::const_iterator begin() const { return frameAngles_.begin(); }

  /// Get end of joint angle map
  frameAngles_t::iterator end() { return frameAngles_.end(); }

  /// Get end of joint angle map
  frameAngles_t::const_iterator end() const { return frameAngles_.end(); }

  /// Get angle for specifed joint
  double operator[](const std::string& s) { return frameAngles_[s]; }
};

/// Class to perform forward kinematic based animation
class FKFramer : public Framer
{
public:
  /// Vector containing frames for the forward kinematic animation
  typedef std::vector<FKFrame> frames_t;
protected:
  /// Frames for the animation
  frames_t frames_;
protected:
  /// Number of animation cylces that have been executed (animation will loop)
  unsigned int cycles_;
  /// The current frame of the animation

  /// Total number of frames for the animation
  unsigned int totalFrames_=0;
  /// Time that the current frame started
  double       frameStart_;
  /// Time management object
  Timer        timer_;
public: 
	unsigned int currentFrame_;
  /// Constructor
  FKFramer();

  /// Destructor
  virtual ~FKFramer() { }

  /// Get the number of elapsed cycles
  unsigned int getCycles() const { return cycles_; }

  /// Get the current frame
  unsigned int getCurrentFrame() const { return currentFrame_; }

  /// Update the animation
  virtual void update(Animation::angles_t& angles);
};

#endif
