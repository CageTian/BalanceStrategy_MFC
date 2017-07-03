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
		if (frameStart_ < 0.0) //Ĭ��Ϊ-1
			frameStart_ = timer_.getTime();

		// Get time elapsed since start of frame
		double t = timer_.getTime() - frameStart_;//t��ʾ��ǰ֡�ĳ���ʱ��

		t *= rate_;   //��������rate�ӿ첥���ٶ� 
		//std::cout << currentFrame_ << "   " << frames_[currentFrame_].getDuration() << "   " << t << endl;
		//if (t > frames_[currentFrame_].getDuration())//��ǰ֡����ʱ��> Ӧ�ó�����ʱ�� ��֡�������
		{
			cout << "��ǰ֡" << currentFrame_ << endl;
			// Proceed to next frame
			if (++currentFrame_ == totalFrames_)//��һ��֡������� �����ֻ�
			{
				cycles_++;
				currentFrame_ = 0;
			}

			frameStart_ = timer_.getTime();
			t = 0.0;
		}

		double a = t / frames_[currentFrame_].getDuration();//�޸�angleֵ

		FKFrame::frameAngles_t::const_iterator iter;
		for (iter = frames_[currentFrame_].begin(); iter != frames_[currentFrame_].end(); ++iter)
		{//��ǰ֡
			//std::cout << iter->second + ((frames_[currentFrame_ + 1][iter->first] - iter->second) * a) << endl;
			angles[iter->first].setValue(iter->second + ((frames_[currentFrame_ + 1][iter->first] - iter->second) * a));
			//������ǰ֡FKFrame������ÿ���ؽ� ������ת�Ƕ�
			//��ת�Ƕ�=��ǰ֡��¼�ĸùؽڵ���ת�Ƕ�+������һ��֡��¼�ĸýڵ����ת�ǶȵĲ�ֵ��*a
		}
	}
}
