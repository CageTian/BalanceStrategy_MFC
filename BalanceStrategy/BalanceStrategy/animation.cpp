#include "stdafx.h"
#include "framer.h"
#include "animation.h"


void Animation::addActor(Actor* actor)
{
  actors_.push_back(actor);

  ActorInitializer ao(actors_.back());
  //  actor->angles_.insert(std::make_pair(arc->getName(), *arc->getRotationAngle()));
  actor->skeleton_->visitArcs(ao);
}

// update the frame
void Animation::update()//����actor�ĽǶ�ֵ
{
	actors_t::iterator iter;
	for (iter = actors_.begin(); iter != actors_.end(); ++iter)
	{
		cout << "����update������" << endl;
		if ((*iter)->framer_)
		{
			cout << "����update" << endl;
			(*iter)->framer_->update((*iter)->angles_);
			
		}

		else
			cout << "û��update������" << endl;
		// Traverse the skeleton
		ActorUpdater au(*iter);
		(*iter)->skeleton_->visitArcs(au);
		}
	}

// draw the current frame
void Animation::draw()
{
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    (*iter)->skeleton_->evaluate();//��actor�ĹǼ�
  }
}

Animation::~Animation()
{
  actors_t::iterator iter;
  for(iter = actors_.begin(); iter != actors_.end(); ++iter)
  {
    delete (*iter);
  }
}
