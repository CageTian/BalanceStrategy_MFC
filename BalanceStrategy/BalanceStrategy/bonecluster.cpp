#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include "bonecluster.h"

BoneCluster::~BoneCluster()
{
  BoneList_t::iterator bli;
  for(bli = bones_.begin(); bli != bones_.end(); ++bli)
    delete *bli;
}

void BoneCluster::addBone(ShapeBone::EBoneType type, double length, const Vector3& trans)
{
  ShapeBone* bone = new ShapeBone(type, length);
  bone->setTranslation(trans);
  bones_.push_back(bone);
}

void BoneCluster::draw()
{
  glPushMatrix();

  glTranslated(trans_.getX(), trans_.getY(), trans_.getZ());
  glRotated(rotAngle_, rotAxis_.getX(), rotAxis_.getY(), rotAxis_.getZ());
  glScaled(scale_.getX(), scale_.getY(), scale_.getZ());

  BoneList_t::iterator bli;
  for(bli = bones_.begin(); bli != bones_.end(); ++bli)
    (*bli)->draw();

  glPopMatrix();
}
