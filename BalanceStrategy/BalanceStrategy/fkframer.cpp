#include "stdafx.h"
#include "fkframer.h"

FKFramer::FKFramer()
	: currentFrame_(0),
	totalFrames_(0),
	frameStart_(-1.0)
{
}

void FKFramer::update(Animation::angles_t& angles)
{
	if (!frames_.empty())
	{
		// Has the timer been initialized?
		if (frameStart_ < 0.0) //默认为-1
			frameStart_ = timer_.getTime();

		// Get time elapsed since start of frame
		double t = timer_.getTime() - frameStart_;//t表示当前帧的持续时间

		t *= rate_;   //可以利用rate加快播放速度 
		//std::cout << currentFrame_ << "   " << frames_[currentFrame_].getDuration() << "   " << t << endl;
		//if (t > frames_[currentFrame_].getDuration())//当前帧播放时间> 应该持续的时间 该帧播放完毕
		{
			cout << "当前帧" << currentFrame_ << endl;
			// Proceed to next frame
			if (++currentFrame_ == totalFrames_)//这一轮帧播放完毕 重新轮回
			{
				cycles_++;
				currentFrame_ = 0;
			}

			frameStart_ = timer_.getTime();
			t = 0.0;
		}

		double a = t / frames_[currentFrame_].getDuration();//修改angle值

		FKFrame::frameAngles_t::const_iterator iter;
		for (iter = frames_[currentFrame_].begin(); iter != frames_[currentFrame_].end(); ++iter)
		{//当前帧
			//std::cout << iter->second + ((frames_[currentFrame_ + 1][iter->first] - iter->second) * a) << endl;
			angles[iter->first].setValue(iter->second + ((frames_[currentFrame_ + 1][iter->first] - iter->second) * a));
			//遍历当前帧FKFrame包含的每个关节 设置旋转角度
			//旋转角度=当前帧记录的该关节的旋转角度+（与下一个帧记录的该节点的旋转角度的差值）*a
		}
	}
}
