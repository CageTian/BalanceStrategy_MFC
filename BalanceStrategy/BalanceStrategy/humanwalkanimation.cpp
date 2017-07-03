#include "stdafx.h"
#include "terrain.h"
#include "humanwalkanimation.h"

// This is the offset of the ankle from the ground, buffer for skin which is -0.1 below bone on y axis
#define ANKLEOFFSET 0.08

void HumanWalkAnimation::addActor(HumanSkeleton* skeleton, HumanWalkFramer* framer)
{
  HumanWalkActor* actor = new HumanWalkActor(skeleton, framer);

  // Attach inverse kinematic solver to left and right legs, for terrain based height correction
  actor->rightleg_.addChain("RLeg", skeleton, "RHipX", "RAnkle");
  actor->leftleg_.addChain("LLeg", skeleton, "LHipX", "LAnkle");

  Animation::addActor(actor);
}

void HumanWalkAnimation::setAngles(char * src, GLdouble angle)
{
	if (angle == 0)
		return;

	actors_t::iterator iter;
	  for (iter = actors_.begin(); iter != actors_.end(); ++iter)
	  {
		  HumanWalkActor* actor = dynamic_cast<HumanWalkActor*>(*iter);
		  if (actor)
		  {
			  Actor newAngles(actor->skeleton_, actor->framer_);
			  Animation::angles_t::iterator rli;
			  for (rli = actor->angles_.begin(); rli != actor->angles_.end(); ++rli)
				  newAngles.angles_.insert(std::make_pair(rli->first, rli->second));
			   newAngles.angles_[src].setValue(angle);
			 // newAngles.angles_.insert(std::make_pair(src, angle));
			  ActorUpdater au(&newAngles);
			  actor->skeleton_->visitArcs(au);  //更新actor的骨架信息
			  cout << "wxwxwx"<<src<<angle<<endl;
			  cout << "look look" << newAngles.angles_[src].getValue();

		  }
    	}

}

void HumanWalkAnimation::update()
{
  // Perform parent's update
  Animation::update();//根据时间跟新actor下每个angle的角度

  // Now get the positions of the joints
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  { //行走过程中  本程序只定义了一个actor
	  //将父类actor强制转换成继承类HumanWalkActor
    HumanWalkActor* actor = dynamic_cast<HumanWalkActor*>(*iter);
    if(actor)
    {
      Vector3 ranklepos, lanklepos;
      double rheight, lheight, rdiff, ldiff;
      Skeleton::ArcPositionVector_t positions;

      actor->skeleton_->getArcPositions(positions);

      // Check right ankle position vs terrain height
      ranklepos = positions["RAnkle"];//获取脚踝的坐标
      rheight = terrain_->getHeight(ranklepos.getX(), ranklepos.getZ());//脚踝停留在的那个面的高度 也就是那个面的y轴坐标
      rdiff = (ranklepos.getY() - ANKLEOFFSET) - rheight;

      // Check left ankle position vs terrain height
      lanklepos = positions["LAnkle"];
      lheight = terrain_->getHeight(lanklepos.getX(), lanklepos.getZ());
	  //cout << "getHeight  "<< terrain_->getHeight(lanklepos.getX(), lanklepos.getZ())<<"   "<<lanklepos.getY();
      ldiff = (lanklepos.getY() - ANKLEOFFSET) - lheight;
	  //cout << ldiff << "   " << rdiff << endl;
      // Adjust ankle height if necessary //调整脚踝高度   脚踝还不在那个面上
      if(rdiff < 0.0 || ldiff < 0.0)
      {
        Actor newAngles(actor->skeleton_, actor->framer_);
        Animation::angles_t::iterator rli;
        for(rli = actor->angles_.begin(); rli != actor->angles_.end(); ++rli)
          newAngles.angles_.insert(std::make_pair(rli->first, rli->second));

        if(rdiff < 0.0)
        {
          // 获得斜坡高度
          double ankleadj = terrain_->getAngle(ranklepos.getX(), ranklepos.getZ());

          // IK treats root as origin, so translate positions to hip local space
          Vector3 hippos = positions["RHipX"];//获得臀部的坐标
          ranklepos -= hippos;   //脚踝坐标-臀部坐标 

          // Attempt to correct with inverse kinematices
		  cout << "wxxxtest  " << hippos.getX() << " " << hippos.getY() << "  " << hippos.getZ()
			  << endl << positions["RAnkle"].getX() << "  " << positions["RAnkle"].getY() << "  " << positions["RAnkle"].getZ() << endl
			  << "rdiff" << rdiff << endl;

		  cout << "reslut" << positions["RAnkle"].getX() - hippos.getX() << "  " << positions["RAnkle"].getY() - hippos.getY() << "  " << positions["RAnkle"].getZ() - hippos.getZ() << endl;

          actor->rightleg_.setGoal("RLeg", ranklepos.getX(), ranklepos.getY() - rdiff, ranklepos.getZ());
          actor->rightleg_.update(newAngles.angles_);
		  cout << endl << endl;
		  
		 
		  // Change the angle of the ankle
          newAngles.angles_["RAnkle"].setValue(ankleadj);//脚踝旋转

          // Traverse the skeleton
          ActorUpdater au(&newAngles);   
          actor->skeleton_->visitArcs(au);  //更新actor的骨架信息

          printf("Lifting right leg by %lf\n", -rdiff);
        }

        if(ldiff < 0.0)
        {
          // Get terrain angle for ankle adjustment
          double ankleadj = terrain_->getAngle(lanklepos.getX(), lanklepos.getZ());

          // IK treats root as origin, so translate positions to hip local space
          Vector3 hippos = positions["LHipX"];
          lanklepos -= hippos;

          // Attempt to correct with inverse kinematices
          actor->leftleg_.setGoal("LLeg", lanklepos.getX(), lanklepos.getY() - ldiff, lanklepos.getZ());
          actor->leftleg_.update(newAngles.angles_);

          // Change the angle of the ankle
          newAngles.angles_["LAnkle"].setValue(ankleadj);

          // Traverse the skeleton
          ActorUpdater au(&newAngles);
          actor->skeleton_->visitArcs(au);

          printf("Lifting left leg by %lf\n", -ldiff);
        }
      }
    }
  }
}
