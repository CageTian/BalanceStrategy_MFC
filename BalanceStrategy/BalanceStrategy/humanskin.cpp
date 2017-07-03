#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include "humanskin.h"

#define DEG_TO_RAD 0.0174532925

#define ANGLE 6.0
#define ANGLERAD (ANGLE*DEG_TO_RAD)
#define EDGES (360.0/ANGLE)

#define HEADRADIUSZ 0.15
#define HEADRADIUSX 0.12
#define TORSORADIUSZ 0.12
#define TORSORADIUSX1 0.3
#define TORSORADIUSX2 0.25
#define NECKRADIUS 0.1
#define ARMRADIUS 0.07
#define LEGRADIUS 0.1



HumanSkin::~HumanSkin()
{
  // Delete vertices and attachments
  VertexList_t::iterator vli;
  for(vli = verts_.begin(); vli != verts_.end(); ++vli)
  {
    Vertex::AttachmentList_t::iterator ali;
    for(ali = (*vli)->attachments_.begin(); ali != (*vli)->attachments_.end(); ++ali)
    {
      delete *ali;
    }
    delete *vli;
  }
}

void HumanSkin::create()
{
  makeLeftLeg();
  makeRightLeg();
  makeLeftArm();
  makeRightArm();
  makeLeftFoot();
  makeRightFoot();
  makeTorso();
  makeNeck();
  makeHead();
}

void HumanSkin::makeArm(const std::string& shoulder, const std::string& elbow)
{
	VertexList_t temp1;
	VertexList_t temp2;

	// The quad strip vertices
	Limb* upper = new Limb(GL_QUAD_STRIP, Vector3(0.925, 0.925, 0.925));
	Limb* upperCap = new Limb(GL_TRIANGLE_FAN, Vector3(0.925, 0.925, 0.925));
	Limb* lower = new Limb(GL_QUAD_STRIP, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));
	Limb* lowerCap = new Limb(GL_TRIANGLE_FAN, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));
	Limb* hand = new Limb(GL_QUAD_STRIP, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));
	Limb* handCap = new Limb(GL_TRIANGLE_FAN, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));

	// y offsets for the upper arm cylinder, and lower arm cylinder
	double uy1 = 0.0, uy2 = -humanData.upperarm;
	double ly1 = 0.0, ly2 = -humanData.lowerarm;//下胳膊y坐标
	double hy1 = 0.0, hy2 = -humanData.lowerarm + 0.105;//手的坐标

														// Create center points and normals for caps
	Vertex* c1 = new Vertex();
	c1->attachments_.push_back(new Vertex::Attachment(shoulder, Vector3(0.0, uy1, 0.0), 1.0));
	verts_.push_back(c1);

	Vertex* n1 = new Vertex();
	n1->attachments_.push_back(new Vertex::Attachment(shoulder, Vector3(0.0, 1.0, 0.0), 1.0));
	verts_.push_back(n1);

	Vertex* c2 = new Vertex();
	c2->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(0.0, ly2, 0.0), 1.0));
	verts_.push_back(c2);

	Vertex* n2 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n2);

	Vertex* c3 = new Vertex();
	c3->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(0.0, hy2, 0.0), 1.0));
	verts_.push_back(c3);

	Vertex* n3 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n3);

	upperCap->verts_.push_back(c1);
	upperCap->normals_.push_back(n1);
	lowerCap->verts_.push_back(c2);
	lowerCap->normals_.push_back(n2);
	handCap->verts_.push_back(c3);
	handCap->normals_.push_back(n3);

	// The arm has two cylinders connected at the elbow
	// The elbow must be attached to both the upper and lower bones
	int i;
	for (i = 0; i <= EDGES; i++)
	{
		double x = ARMRADIUS * cos(ANGLERAD*(EDGES - i));
		double z = ARMRADIUS * sin(ANGLERAD*(EDGES - i));

		// Create the normals
		Vertex* nu = new Vertex();
		nu->attachments_.push_back(new Vertex::Attachment(shoulder, Vector3(2 * x / (ARMRADIUS * ARMRADIUS), 0.0, 2 * z / (ARMRADIUS * ARMRADIUS)), 1.0));
		verts_.push_back(nu);

		Vertex* nl = new Vertex();
		nl->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(2 * x / (ARMRADIUS * ARMRADIUS), 0.0, 2 * z / (ARMRADIUS * ARMRADIUS)), 1.0));
		verts_.push_back(nl);

		// Create top of upper arm
		Vertex* v1 = new Vertex();
		v1->attachments_.push_back(new Vertex::Attachment(shoulder, Vector3(x, uy1, z), 1.0));
		verts_.push_back(v1);

		// Create bottom of upper arm/top of lower arm
		Vertex* v2 = new Vertex();
		v2->attachments_.push_back(new Vertex::Attachment(shoulder, Vector3(x, uy2, z), 0.5));
		v2->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(x, ly1, z), 0.5));
		verts_.push_back(v2);

		// Create bottom of lower arm
		Vertex* v3 = new Vertex();
		v3->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(x, ly2, z), 1.0));
		verts_.push_back(v3);

		// Create top of hand
		Vertex* v4 = new Vertex();
		v4->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(2.0*x / 3.0, hy1, 2.0*z / 3.0), 1.0));
		verts_.push_back(v4);

		// Create bottom of hand
		Vertex* v5 = new Vertex();
		v5->attachments_.push_back(new Vertex::Attachment(elbow, Vector3(2.0*x / 3.0, hy2, 2.0*z / 3.0), 1.0));
		verts_.push_back(v5);

		// Add the vertices to the upper and lower arm quad strips
		upper->verts_.push_back(v1);
		upper->verts_.push_back(v2);
		upper->normals_.push_back(nu);
		upper->normals_.push_back(nu);

		lower->verts_.push_back(v2);
		lower->verts_.push_back(v3);
		lower->normals_.push_back(nl);
		lower->normals_.push_back(nl);

		hand->verts_.push_back(v4);
		hand->verts_.push_back(v5);
		hand->normals_.push_back(nl);
		hand->normals_.push_back(nl);

		upperCap->verts_.push_back(v1);
		upperCap->normals_.push_back(n1);

		temp1.push_back(v3);
		temp2.push_back(v5);
	}

	// Specify the lower cap vertices in reverse
	VertexList_t::reverse_iterator rti;
	//因为 手腕 和手的圆盖子  方向是相同的。 手臂上的圆盖子和他们方向相反。为了使得所画下来的圆盖子都是 正面（逆时针方向绘制的平面） 所以此处需要调转一下手腕和手的圆盖子
	for (rti = temp1.rbegin(); rti != temp1.rend(); ++rti)
	{
		lowerCap->verts_.push_back(*rti);
		lowerCap->normals_.push_back(n2);
	}

	for (rti = temp2.rbegin(); rti != temp2.rend(); ++rti)
	{
		handCap->verts_.push_back(*rti);
		handCap->normals_.push_back(n3);
	}

	limbs_.push_back(upperCap);
	limbs_.push_back(upper);
	limbs_.push_back(lower);
	limbs_.push_back(lowerCap);
	limbs_.push_back(hand);
	limbs_.push_back(handCap);
}

void HumanSkin::makeLeg(const std::string& hip, const std::string& knee)
{
	VertexList_t temp;

	// The quad strip vertices
	Limb* upper = new Limb(GL_QUAD_STRIP, Vector3(0.925, 0.925, 0.925));
	Limb* upperCap = new Limb(GL_TRIANGLE_FAN, Vector3(0.925, 0.925, 0.925));
	Limb* lower = new Limb(GL_QUAD_STRIP, Vector3(0.925, 0.925, 0.925));
	Limb* lowerCap = new Limb(GL_TRIANGLE_FAN, Vector3(0.925, 0.925, 0.925));

	// y offsets for the upper arm cylinder, and lower arm cylinder
	double uy1 = 0.0, uy2 = -humanData.upperleg;
	double ly1 = 0.0, ly2 = -humanData.lowerleg;

	// Create center points and normals for caps
	Vertex* c1 = new Vertex();
	c1->attachments_.push_back(new Vertex::Attachment(hip, Vector3(0.0, uy1, 0.0), 1.0));
	verts_.push_back(c1);

	Vertex* n1 = new Vertex();
	n1->attachments_.push_back(new Vertex::Attachment(hip, Vector3(0.0, 1.0, 0.0), 1.0));
	verts_.push_back(n1);

	Vertex* c2 = new Vertex();
	c2->attachments_.push_back(new Vertex::Attachment(knee, Vector3(0.0, ly2, 0.0), 1.0));
	verts_.push_back(c2);

	Vertex* n2 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment(knee, Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n2);

	upperCap->verts_.push_back(c1);
	upperCap->normals_.push_back(n1);
	lowerCap->verts_.push_back(c2);
	lowerCap->normals_.push_back(n2);

	// The leg has two cylinders connected at the knee
	// The knee must be attached to both the upper and lower bones
	int i;
	for (i = 0; i <= EDGES; i++)
	{
		double x = LEGRADIUS * cos(ANGLERAD*(EDGES - i));
		double z = LEGRADIUS * sin(ANGLERAD*(EDGES - i));

		// Create the normals
		Vertex* nu = new Vertex();
		nu->attachments_.push_back(new Vertex::Attachment(hip, Vector3(2.0 * x / (LEGRADIUS * LEGRADIUS), 0.0, 2.0 * z / (LEGRADIUS * LEGRADIUS)), 1.0));
		verts_.push_back(nu);

		Vertex* nl = new Vertex();
		nl->attachments_.push_back(new Vertex::Attachment(knee, Vector3(2.0 * x / (LEGRADIUS * LEGRADIUS), 0.0, 2.0 * z / (LEGRADIUS * LEGRADIUS)), 1.0));
		verts_.push_back(nl);

		// Create top of upper leg
		Vertex* v1 = new Vertex();
		v1->attachments_.push_back(new Vertex::Attachment(hip, Vector3(x, uy1, z), 1.0));
		verts_.push_back(v1);

		// Create bottom of upper leg/top of lower leg
		Vertex* v2 = new Vertex();
		v2->attachments_.push_back(new Vertex::Attachment(hip, Vector3(x, uy2, z), 0.5));
		v2->attachments_.push_back(new Vertex::Attachment(knee, Vector3(x, ly1, z), 0.5));
		verts_.push_back(v2);

		// Create bottom of lower leg
		Vertex* v3 = new Vertex();
		v3->attachments_.push_back(new Vertex::Attachment(knee, Vector3(x, ly2, z), 1.0));
		verts_.push_back(v3);

		// Add the vertices to the upper and lower leg quad strips
		upper->verts_.push_back(v1);
		upper->verts_.push_back(v2);
		upper->normals_.push_back(nu);
		upper->normals_.push_back(nu);

		lower->verts_.push_back(v2);
		lower->verts_.push_back(v3);
		lower->normals_.push_back(nl);
		lower->normals_.push_back(nl);

		upperCap->verts_.push_back(v1);
		upperCap->normals_.push_back(n1);

		temp.push_back(v3);
	}

	// Specify the lower cap vertices in reverse
	VertexList_t::reverse_iterator rti;
	for (rti = temp.rbegin(); rti != temp.rend(); ++rti)
	{
		lowerCap->verts_.push_back(*rti);
		lowerCap->normals_.push_back(n2);
	}

	limbs_.push_back(upperCap);
	limbs_.push_back(upper);
	limbs_.push_back(lower);
	limbs_.push_back(lowerCap);
}

void HumanSkin::makeFoot(const std::string& ankle)
{
	// The quad vertices
	Limb* foot = new Limb(GL_QUADS, Vector3(235.0 / 255.0, 235.0 / 255.0, 235.0 / 255.0));

	// Create the vertices for the foot
	Vertex* v1 = new Vertex();
	v1->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(-0.1, 0.05, -0.1), 1.0));
	verts_.push_back(v1);
	Vertex* v2 = new Vertex();
	v2->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(-0.1, 0.05, humanData.foot + 0.05), 1.0));
	verts_.push_back(v2);
	Vertex* v3 = new Vertex();
	v3->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.1, 0.05, humanData.foot + 0.05), 1.0));
	verts_.push_back(v3);
	Vertex* v4 = new Vertex();
	v4->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.1, 0.05, -0.1), 1.0));
	verts_.push_back(v4);

	Vertex* v5 = new Vertex();
	v5->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(-0.1, -0.05, -0.1), 1.0));
	verts_.push_back(v5);
	Vertex* v6 = new Vertex();
	v6->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(-0.1, -0.05, humanData.foot + 0.05), 1.0));
	verts_.push_back(v6);
	Vertex* v7 = new Vertex();
	v7->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.1, -0.05, humanData.foot + 0.05), 1.0));
	verts_.push_back(v7);
	Vertex* v8 = new Vertex();
	v8->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.1, -0.05, -0.1), 1.0));
	verts_.push_back(v8);

	Vertex* n1 = new Vertex();
	n1->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.0, 1.0, 0.0), 1.0));
	verts_.push_back(n1);

	Vertex* n2 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n2);

	Vertex* n3 = new Vertex();
	n3->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(-1.0, 0.0, 0.0), 1.0));
	verts_.push_back(n3);

	Vertex* n4 = new Vertex();
	n4->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(1.0, 0.0, 0.0), 1.0));
	verts_.push_back(n4);

	Vertex* n5 = new Vertex();
	n5->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.0, 0.0, 1.0), 1.0));
	verts_.push_back(n5);

	Vertex* n6 = new Vertex();
	n6->attachments_.push_back(new Vertex::Attachment(ankle, Vector3(0.0, 0.0, -1.0), 1.0));
	verts_.push_back(n6);

	// Back
	foot->verts_.push_back(v8);
	foot->verts_.push_back(v5);
	foot->verts_.push_back(v1);
	foot->verts_.push_back(v4);
	foot->normals_.push_back(n6);
	foot->normals_.push_back(n6);
	foot->normals_.push_back(n6);
	foot->normals_.push_back(n6);

	// Front
	foot->verts_.push_back(v6);
	foot->verts_.push_back(v7);
	foot->verts_.push_back(v3);
	foot->verts_.push_back(v2);
	foot->normals_.push_back(n5);
	foot->normals_.push_back(n5);
	foot->normals_.push_back(n5);
	foot->normals_.push_back(n5);

	// Left
	foot->verts_.push_back(v7);
	foot->verts_.push_back(v8);
	foot->verts_.push_back(v4);
	foot->verts_.push_back(v3);
	foot->normals_.push_back(n4);
	foot->normals_.push_back(n4);
	foot->normals_.push_back(n4);
	foot->normals_.push_back(n4);

	// Right
	foot->verts_.push_back(v5);
	foot->verts_.push_back(v6);
	foot->verts_.push_back(v2);
	foot->verts_.push_back(v1);
	foot->normals_.push_back(n3);
	foot->normals_.push_back(n3);
	foot->normals_.push_back(n3);
	foot->normals_.push_back(n3);

	// Bottom
	foot->verts_.push_back(v8);
	foot->verts_.push_back(v7);
	foot->verts_.push_back(v6);
	foot->verts_.push_back(v5);
	foot->normals_.push_back(n2);
	foot->normals_.push_back(n2);
	foot->normals_.push_back(n2);
	foot->normals_.push_back(n2);

	// Top
	foot->verts_.push_back(v1);
	foot->verts_.push_back(v2);
	foot->verts_.push_back(v3);
	foot->verts_.push_back(v4);
	foot->normals_.push_back(n1);
	foot->normals_.push_back(n1);
	foot->normals_.push_back(n1);
	foot->normals_.push_back(n1);

	limbs_.push_back(foot);
}

void HumanSkin::makeTorso()
{
	VertexList_t temp;

	// The quad strip vertices
	Limb* upper = new Limb(GL_QUAD_STRIP, Vector3(0.925, 0.925, 0.925));
	Limb* upperCap = new Limb(GL_TRIANGLE_FAN, Vector3(0.925, 0.925, 0.925));
	Limb* lower = new Limb(GL_QUAD_STRIP, Vector3(0.925, 0.925, 0.925));
	Limb* lowerCap = new Limb(GL_TRIANGLE_FAN, Vector3(0.925, 0.925, 0.925));

	// y offsets for the upper arm cylinder, and lower arm cylinder
	double uy1 = humanData.torse , uy2 = humanData.torse / 2+0.2;//uyl  ly2 是立方贴图的上方坐标 和最低端坐标
	double ly1 = 0, ly2 = 0;

	// Create center points and normals for caps
	Vertex* c1 = new Vertex();
	c1->attachments_.push_back(new Vertex::Attachment("Body", Vector3(0.0, uy1, 0.0), 1.0));
	verts_.push_back(c1);

	Vertex* n1 = new Vertex();
	n1->attachments_.push_back(new Vertex::Attachment("Body", Vector3(0.0, 1.0, 0.0), 1.0));
	verts_.push_back(n1);

	Vertex* c2 = new Vertex();
	c2->attachments_.push_back(new Vertex::Attachment("Body", Vector3(0.0, ly2, 0.0), 1.0));
	verts_.push_back(c2);

	Vertex* n2 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment("Body", Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n2);

	upperCap->verts_.push_back(c1);
	upperCap->normals_.push_back(n1);
	lowerCap->verts_.push_back(c2);
	lowerCap->normals_.push_back(n2);

	// The body has two cylinders connected at the waist
	int i;
	for (i = 0; i <= EDGES; i++)
	{
		double x1 = TORSORADIUSX1 * cos(ANGLERAD*(EDGES - i));
		double x2 = TORSORADIUSX2 * cos(ANGLERAD*(EDGES - i));
		double z = TORSORADIUSZ * sin(ANGLERAD*(EDGES - i));

		// Create the normals
		Vertex* nx1 = new Vertex();
		nx1->attachments_.push_back(new Vertex::Attachment("Body", Vector3(2.0 * x1 / (TORSORADIUSX1 * TORSORADIUSX1), 0.0, 2.0 * z / (TORSORADIUSZ * TORSORADIUSZ)), 1.0));
		verts_.push_back(nx1);

		Vertex* nx2 = new Vertex();
		nx2->attachments_.push_back(new Vertex::Attachment("Body", Vector3(2.0 * x2 / (TORSORADIUSX2 * TORSORADIUSX2), 0.0, 2.0 * z / (TORSORADIUSZ * TORSORADIUSZ)), 1.0));
		verts_.push_back(nx2);

		// Create top of torso
		Vertex* v1 = new Vertex();
		v1->attachments_.push_back(new Vertex::Attachment("Body", Vector3(x1, uy1, z), 1.0));
		verts_.push_back(v1);

		// Create bottom of upper leg/top of lower leg
		Vertex* v2 = new Vertex();
		v2->attachments_.push_back(new Vertex::Attachment("Body", Vector3(x2, uy2, z), 1.0));
		verts_.push_back(v2);

		// Create bottom of lower leg
		Vertex* v3 = new Vertex();
		v3->attachments_.push_back(new Vertex::Attachment("Body", Vector3(x2, ly2, z), 1.0));
		verts_.push_back(v3);

		// Add the vertices to the upper and lower leg quad strips
		upper->verts_.push_back(v1);
		upper->verts_.push_back(v2);
		upper->normals_.push_back(nx1);
		upper->normals_.push_back(nx2);

		lower->verts_.push_back(v2);
		lower->verts_.push_back(v3);
		lower->normals_.push_back(nx2);
		lower->normals_.push_back(nx2);

		upperCap->verts_.push_back(v1);
		upperCap->normals_.push_back(n1);

		temp.push_back(v3);
	}

	// Specify the lower cap vertices in reverse
	VertexList_t::reverse_iterator rti;
	for (rti = temp.rbegin(); rti != temp.rend(); ++rti)
	{
		lowerCap->verts_.push_back(*rti);
		lowerCap->normals_.push_back(n2);
	}

	limbs_.push_back(upperCap);
	limbs_.push_back(upper);
	limbs_.push_back(lower);
	limbs_.push_back(lowerCap);
}

void HumanSkin::makeNeck()
{
	// The quad strip vertices
	Limb* neck = new Limb(GL_QUAD_STRIP, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));

	// y offsets for the neck cylinder
	double y1 = 0.1, y2 = 0.0;

	// The neck has one cylinder
	int i;
	for (i = 0; i <= EDGES; i++)
	{
		double x = NECKRADIUS * cos(ANGLERAD*(EDGES - i));
		double z = NECKRADIUS * sin(ANGLERAD*(EDGES - i));

		// Create the normals
		Vertex* n = new Vertex();
		n->attachments_.push_back(new Vertex::Attachment("NeckBase", Vector3(2.0 * x / (NECKRADIUS * NECKRADIUS), 0.0, 2.0 * z / (NECKRADIUS * NECKRADIUS)), 1.0));
		verts_.push_back(n);

		// Create top of neck
		Vertex* v1 = new Vertex();
		v1->attachments_.push_back(new Vertex::Attachment("NeckBase", Vector3(x, y1, z), 1.0));
		verts_.push_back(v1);

		// Create bottom of neck
		Vertex* v2 = new Vertex();
		v2->attachments_.push_back(new Vertex::Attachment("NeckBase", Vector3(x, y2, z), 1.0));
		verts_.push_back(v2);

		// Add the vertices to the neck quad strip
		neck->verts_.push_back(v1);
		neck->verts_.push_back(v2);
		neck->normals_.push_back(n);
		neck->normals_.push_back(n);
	}

	limbs_.push_back(neck);
}

void HumanSkin::makeHead()
{
	VertexList_t temp;

	// The quad strip vertices
	Limb* upper = new Limb(GL_QUAD_STRIP, Vector3(139.0 / 255.0, 90.0 / 255.0, 43.0 / 255.0));
	Limb* upperCap = new Limb(GL_TRIANGLE_FAN, Vector3(139.0 / 255.0, 90.0 / 255.0, 43.0 / 255.0));
	Limb* lower = new Limb(GL_QUAD_STRIP, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));
	Limb* lowerCap = new Limb(GL_TRIANGLE_FAN, Vector3(235.0 / 255.0, 199.0 / 255.0, 158.0 / 255.0));

	// y offsets for the upper arm cylinder, and lower arm cylinder
	double uy1 = 0.401, uy2 = 0.32;
	double ly1 = 0.32, ly2 = 0.0;

	// Create center points and normals for caps
	Vertex* c1 = new Vertex();
	c1->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(0.0, uy1, 0.0), 1.0));
	verts_.push_back(c1);

	Vertex* n1 = new Vertex();
	n1->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(0.0, 1.0, 0.0), 1.0));
	verts_.push_back(n1);

	Vertex* c2 = new Vertex();
	c2->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(0.0, ly2, 0.0), 1.0));
	verts_.push_back(c2);

	Vertex* n2 = new Vertex();
	n2->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(0.0, -1.0, 0.0), 1.0));
	verts_.push_back(n2);

	upperCap->verts_.push_back(c1);
	upperCap->normals_.push_back(n1);
	lowerCap->verts_.push_back(c2);
	lowerCap->normals_.push_back(n2);

	// The body has two cylinders connected at the hairline
	int i;
	for (i = 0; i <= EDGES; i++)
	{
		double x = HEADRADIUSX * cos(ANGLERAD*(EDGES - i));
		double z = HEADRADIUSZ * sin(ANGLERAD*(EDGES - i));

		// Create the normals
		Vertex* n = new Vertex();
		n->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(2.0 * x / (HEADRADIUSX * HEADRADIUSX), 0.0, 2.0 * z / (HEADRADIUSZ * HEADRADIUSZ)), 1.0));
		verts_.push_back(n);

		// Create hair
		Vertex* v1 = new Vertex();
		v1->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(x, uy1, z), 1.0));
		verts_.push_back(v1);

		// Create bottom of upper leg/top of lower leg
		Vertex* v2 = new Vertex();
		v2->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(x, uy2, z), 1.0));
		verts_.push_back(v2);

		// Create bottom of lower leg
		Vertex* v3 = new Vertex();
		v3->attachments_.push_back(new Vertex::Attachment("HeadBase", Vector3(x, ly2, z), 1.0));
		verts_.push_back(v3);

		// Add the vertices to the upper and lower leg quad strips
		upper->verts_.push_back(v1);
		upper->verts_.push_back(v2);
		upper->normals_.push_back(n);
		upper->normals_.push_back(n);

		lower->verts_.push_back(v2);
		lower->verts_.push_back(v3);
		lower->normals_.push_back(n);
		lower->normals_.push_back(n);

		upperCap->verts_.push_back(v1);
		upperCap->normals_.push_back(n1);

		temp.push_back(v3);
	}

	// Specify the lower cap vertices in reverse
	VertexList_t::reverse_iterator rti;
	for (rti = temp.rbegin(); rti != temp.rend(); ++rti)
	{
		lowerCap->verts_.push_back(*rti);
		lowerCap->normals_.push_back(n2);
	}

	limbs_.push_back(upperCap);
	limbs_.push_back(upper);
	limbs_.push_back(lower);
	limbs_.push_back(lowerCap);
}

void HumanSkin::draw()
{
  LimbList_t::iterator lli;
  for(lli = limbs_.begin(); lli != limbs_.end(); ++lli)
  {
    glBegin((*lli)->type_);

    glColor3dv(&((*lli)->color_[0]));

    size_t i, n = (*lli)->verts_.size();
    for(i = 0; i < n; i++)
    {
      glNormal3dv(&((*lli)->normals_[i]->position_[0]));
      glVertex3dv(&((*lli)->verts_[i]->position_[0]));
    }

    glEnd();
  }
}
