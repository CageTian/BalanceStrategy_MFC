#ifndef SKELETON_H
#define SKELETON_H

#include <map>
#include <vector>
#include <string>

#include "vector3.h"
#include "matrix44.h"

class Shape;

/// Class defining a single degree of freedom joint
class SkelDOF
{
protected:
	/// Minimum value for DOF
	double min_;
	/// Maximum value for DOF
	double max_;
	/// Current value for DOF
	double value_;
public:
	/// Constructor
	SkelDOF(double value = 0.0, double min = -360.0, double max = 360.0);

	/// Copy constructor
	SkelDOF(const SkelDOF& sdof) : min_(sdof.min_), max_(sdof.max_), value_(sdof.value_) { }

	/// Get DOF value
	double getValue() const { return value_; }

	/// Set DOF value
	void setValue(double value);

	/// Get minimum DOF value
	double getMin() const { return min_; }

	/// Get maximum DOF value
	double getMax() const { return max_; }

	/// Set minimum DOF value
	void setMin(double min) { min_ = min; }

	/// Get maximum DOF value
	void setMax(double max) { max_ = max; }

	/// Get minimum and maximum DOF values
	void setMinMax(double min, double max) { min_ = min; max_ = max; }
};


class SkelArc;
/// Vector containing joints for a skeleton
typedef std::vector<SkelArc*> skelarcs_t;

/// Class defining the skeleton link/bone
class SkelNode//骨架节点
{
protected:
	/// Name of link/bone
	std::string name_;
	/// Shape to represent the bone when drawing the skeleton
	Shape* shape_;//平移 缩放 旋转
				  /// Joints attached to the link
	skelarcs_t arcs_;//关节
public:
	/// Constructor
	SkelNode(const std::string& name = "Link", Shape* shape = NULL) : name_(name), shape_(shape) { }

	/// Get bone name
	std::string getName() const { return name_; }

	/// Get bone shape
	Shape* getShape() const { return shape_; }

	/// Set bone name
	void setName(const std::string& name) { name_ = name; }

	/// Set bone shape
	void setShape(Shape* shape) { shape_ = shape; }

	/// Attach a joint to the bone
	void addArc(SkelArc* arc) { arcs_.push_back(arc); }

	/// Get begining of joint list
	skelarcs_t::iterator begin() { return arcs_.begin(); }

	/// Get begining of joint list
	skelarcs_t::const_iterator begin() const { return arcs_.begin(); }

	/// Get end of joint list
	skelarcs_t::iterator end() { return arcs_.end(); }

	/// Get end of joint list
	skelarcs_t::const_iterator end() const { return arcs_.end(); }
};

/// Class defining the skeleton joint
class SkelArc//关节
{
protected:
	/// Name of joint
	std::string name_;
	/// Joint angle
	SkelDOF rotAngle_;
	/// Joint axis of rotation
	Vector3 rotAxis_;
	/// Joint translation
	Vector3 trans_;
	/// Node attached to joint
	SkelNode* node_;
public:
	/// Constructor
	SkelArc(const std::string& name = "Joint", SkelNode* node = NULL) : name_(name), node_(node) { }

	/// Get joint name
	std::string getName() const { return name_; }

	/// Get node attached to joint
	SkelNode* getNode() const { return node_; }

	/// Get joint's angle of rotation
	SkelDOF* getRotationAngle() const { return (SkelDOF*)&rotAngle_; }

	/// Get axis of rotation
	Vector3* getRotationAxis() const { return (Vector3*)&rotAxis_; }

	/// Get joint's translation
	Vector3* getTranslation() const { return (Vector3*)&trans_; }

	/// Set the name of the joint
	void setName(const std::string& name) { name_ = name; }

	/// Attach node to joint
	void setNode(SkelNode* node) { node_ = node; }

	/// Set angle of rotation as double
	void setRotationAngle(double angle) { rotAngle_.setValue(angle); }

	/// Set angle of rotation as SkelDOF
	void setRotationAngle(const SkelDOF& rotAngle) { rotAngle_ = rotAngle; }

	/// Set axis of rotation as 3 doubles
	void setRotationAxis(double x, double y, double z) { rotAxis_.set(x, y, z); }

	/// Set axis of rotation as Vector3
	void setRotationAxis(const Vector3& rotAxis) { rotAxis_ = rotAxis; }

	/// Set translation as 3 doubles
	void setTranslation(double x, double y, double z) { trans_.set(x, y, z); }

	/// Set translation as Vector3
	void setTranslation(const Vector3& trans) { trans_ = trans; }
};

/// Class defining the skeleton for animation control
class Skeleton
{
public:
	/// Map containing world space positions for skletal joints
	typedef std::map<std::string, Vector3> ArcPositionVector_t;
	/// Map containint world space transofrmations for skeletal joints
	typedef std::map<std::string, Matrix44> ArcTransformVector_t;
public:
	/// Class to process a node while traversing the skeletal tree
	class NodeVisitor
	{
	public:
		/// Operation to apply to the node
		virtual void operator()(SkelNode* node) = 0;

		/// Destructor
		virtual ~NodeVisitor() { }
	};
public:
	/// Class to process an arc while traversing the skeletal tree
	class ArcVisitor
	{
	public:
		/// Operation to apply to the arc
		virtual void operator()(SkelArc* arc) = 0;

		/// Destructor
		virtual ~ArcVisitor() { }
	};
protected:
	/// Enable/disable drawing of skeleton
	bool drawSkeleton_;
	/// Root of the skeleton
	SkelArc* root_;
protected:
	/// Recursive node finding function
	SkelNode* findNextNode(SkelArc* arc, const std::string& name) const;
	/// Recursive arc finding function
	SkelArc* findNextArc(SkelArc* arc, const std::string& name) const;
	/// Recursive node processing function
	void visitNextNode(SkelArc* arc, NodeVisitor& visitor);
	/// Recursive arc processing function
	void visitNextArc(SkelArc* arc, ArcVisitor& visitor);
	/// Recursive function to find arc world space positions
	void arcPositions(const SkelArc* arc, const Matrix44& G, ArcPositionVector_t& positions);
	/// Recursive function to find arc world space transformations
	virtual void arcTransforms(const SkelArc* arc, const Matrix44& G, ArcTransformVector_t& transforms);
	/// Recursive function to evalute (draw) skeleton
	void evalArc(const SkelArc* arc) const;
public:
	/// Constructor
	Skeleton(SkelArc* root = NULL, bool drawSkeleton = true) : drawSkeleton_(drawSkeleton), root_(root) { }

	/// Get draw skeleton mode
	bool getDrawSkeleton() const { return drawSkeleton_; }

	/// Retrieve skeleton's root joint
	SkelArc* getRootArc() const { return root_; }

	/// Set draw skeleton mode
	void setDrawSkeleton(bool drawSkeleton) { drawSkeleton_ = drawSkeleton; }

	/// Set the skeleton's root joint
	void setRootArc(SkelArc* root) { root_ = root; }
	/// Find the node (link or bone) with the specified name
	SkelNode* findNode(const std::string& name) const { return findNextNode(root_, name); }

	/// Find the arc (joint) with the specified name
	SkelArc* findArc(const std::string& name) const { return findNextArc(root_, name); }

	/// Apply the operation specified by the NodeVisitor functor to each skeletal node
	void visitNodes(NodeVisitor& visitor) { visitNextNode(root_, visitor); }

	/// Apply the operation specified by the ArcVisitor functor to each skeletal arc
	void visitArcs(ArcVisitor& visitor) { visitNextArc(root_, visitor); }

	/// Retrieve the world space positions of all skeletal arcs
	void getArcPositions(ArcPositionVector_t& positions) { Matrix44 G; G.SetIdentity(); if (root_) arcPositions(root_, G, positions); }

	/// Retrieve the world space corrdinates for all skeletal arcs
	void getArcTransforms(ArcTransformVector_t& transforms) { Matrix44 G; G.SetIdentity(); if (root_) arcTransforms(root_, G, transforms); }


	/// Evalute (draw) the skeleton
	void evaluate() const { if (root_) evalArc(root_); }

	/// Destructor
	virtual ~Skeleton() { }
};

#endif
