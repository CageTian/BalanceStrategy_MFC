#include "stdafx.h"
#include "actionAnimation.h"

// This is the offset of the ankle from the ground, buffer for skin which is -0.1 below bone on y axis
#define ANKLEOFFSET 0.08


void  actionAnimation::actionActor::setFramer(actionFramer* f)
{
	framer_ = f;
}
void actionAnimation::addActor(HumanSkeleton* skeleton, actionFramer* framer)
{
	actionActor* actor;
	if (framer != NULL)
		 actor = new actionActor(skeleton, framer);
	else
		actor = new actionActor(skeleton);
	// Attach inverse kinematic solver to left and right legs, for terrain based height correction
	actor->rightleg_.addChain("RLeg", skeleton, "RHipX", "RAnkle");
	actor->leftleg_.addChain("LLeg", skeleton, "LHipX", "LAnkle");

	Animation::addActor(actor);
}

void actionAnimation::updateJacobi()
{

}
void actionAnimation::clearActor()
{

	actors_t::iterator itr = actors_.begin();
	actors_t::iterator tem = actors_.begin();
	while (itr != actors_.end())
	{
		tem = itr;
		itr++;
		actors_.erase(tem);
	}


}
void actionAnimation::setJacobi(char * name ,Vector3 v)
{

	//Animation::update();//根据时间跟新actor下每个angle的角度

	actors_t::iterator iter;
	Skeleton::ArcPositionVector_t positions;
	Vector3 hippos;
	for (iter = actors_.begin(); iter != actors_.end(); ++iter)
	{
		actionActor* actor = dynamic_cast<actionActor*>(*iter);
		if (actor)
		{
			actor->skeleton_->getArcPositions(positions);

			
			hippos = positions["RHipX"];//获取脚踝的坐标
			v -= hippos;
			Actor newAngles(actor->skeleton_,NULL);
			Animation::angles_t::iterator rli;
			for (rli = actor->angles_.begin(); rli != actor->angles_.end(); ++rli)
			{
				newAngles.angles_.insert(std::make_pair(rli->first, rli->second));
				//cout << "look  look lo~~~~~~~~~~~~"<<endl;
				//cout << rli->first << "  " << rli->second.getValue() << endl;
			}
			//cout << name << v.getX()<<"  "<<v.getY()<<"  "<<v.getZ()<<"!!";
			if (name == "RLeg")
			{
					
			
				//actor->framer_->addFrame(newAngles.angles_);
				actor->rightleg_.setGoal(name, v.getX(), v.getY(), v.getZ());
				actor->rightleg_.update(newAngles.angles_);
				//actor->framer_->addFrame(newAngles.angles_);
				

	
			

			}
			ActorUpdater au(&newAngles);
			actor->skeleton_->visitArcs(au);  //更新actor的骨架信息
		}
	}

	
}


void actionAnimation::setAngles(char * src, GLdouble angle)
{
	if (angle == 0)
		return;

	actors_t::iterator iter;
	for (iter = actors_.begin(); iter != actors_.end(); ++iter)
	{
		actionActor* actor = dynamic_cast<actionActor*>(*iter);
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
			//cout << "wxwxwx" << src << angle << endl;
			//cout << "look look" << newAngles.angles_[src].getValue();

		}
	}

}

void actionAnimation::update()
{
	// Perform parent's update
	//cout << "~~~~~~~~~~~~~" << endl;
	Animation::update();//根据时间跟新actor下每个angle的角度
	/*actors_t::iterator iter;
	for (iter = actors_.begin(); iter != actors_.end(); ++iter)
	{ //行走过程中  本程序只定义了一个actor
	  //将父类actor强制转换成继承类HumanWalkActor
		actionActor* actor = dynamic_cast<actionActor*>(*iter);
		if (actor)
		{
			Actor newAngles(actor->skeleton_, actor->framer_);
			Animation::angles_t::iterator rli;
			for (rli = actor->angles_.begin(); rli != actor->angles_.end(); ++rli)
				newAngles.angles_.insert(std::make_pair(rli->first, rli->second));

			actionFramer* framer = dynamic_cast<actionFramer*>(actor->framer_);
			std::cout << "在这里修改 0.05  单位时间" << endl;
			
			Point p(0, 0, 0);
			std::map<double, Point>::iterator iter = Ankle.find(framer->currentFrame_*0.05);
			if (iter != Ankle.end())
            	p = iter->second;
			Vector3 v(p.px, p.py, p.pz);
			setJacobi("RLeg", v);
			
		}
	}*/
}
	
