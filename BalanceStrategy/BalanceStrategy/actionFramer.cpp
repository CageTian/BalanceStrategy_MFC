#include "stdafx.h"
#include "actionFramer.h"
#include<gl/glut.h>

#define FRAMEDURATION 1
void  actionFramer::addFrame(char* arc, GLdouble angles, GLdouble newangles)
{
	//for test!!!!!!!!!!!!!!!!!!!!!!!!!
	FKFrame frame1;

	frame1.setDuration(FRAMEDURATION);
	frame1.addAngle(arc, angles);
	frames_.push_back(frame1);
	FKFrame frame2;
	frame2.setDuration(FRAMEDURATION);
	frame2.addAngle(arc, newangles);
	frames_.push_back(frame2);
	frames_.push_back(frame1);
	totalFrames_ = 2;
}
void  actionFramer::addFrame( std::map<string,double> angles_)
{
	FKFrame frame1;
	std::map<string, double> ::iterator rli;
	frame1.setDuration(0.5);
	for (rli = angles_.begin(); rli != angles_.end(); ++rli)
	{
		frame1.addAngle(rli->first, (rli->second));
	}
	frames_.push_back(frame1);
	totalFrames_++;
}
void  actionFramer::addFrame(Animation::angles_t angles_)
{
	FKFrame frame1;
	Animation::angles_t::iterator rli;
	frame1.setDuration(0.5);
	for (rli = angles_.begin(); rli != angles_.end(); ++rli)
	{
		frame1.addAngle(rli->first, (rli->second).getValue());
	}
	frames_.push_back(frame1);
	totalFrames_++;
}
void actionFramer::addStepFrame(std::map<double, double> COMData, std::map<double, Offset> offset, double rate)
{
	std::map<double, double>::iterator iter1;
	std::map<double, Offset>::iterator iter2;
	FKFrame temp;
	temp.setDuration(0.05);
	temp.addAngle("Body", COMData.begin()->second);
	//temp.addAngle("RAnkle", -COMData.begin()->second);
	//temp.addAngle("LAnkle", -COMData.begin()->second);
	//temp.setRootOffset(offset.begin()->second.px, offset.begin()->second.pz);
	for (iter1 = COMData.begin(), iter2 = offset.begin(); iter1 != COMData.end(), iter2 != offset.end(); iter1++, iter2++)
	{
		FKFrame frame;
		frame.setDuration(0.05);
		frame.addAngle("Body", iter1->second);
		frame.addAngle("RAnkle", iter1->second);
		frame.addAngle("LAnkle",iter1->second);
 		//frame.setRootOffset(iter2->second.px, iter2->second.pz);
		frames_.push_back(frame);

		totalFrames_++;
		//std::cout << "angle   " << totalFrames_<<"  "<< iter1->second << "   " << iter2->second.px << "   " << iter2->second.pz << endl;
	}
	frames_.push_back(temp);
	//totalFrames_++;

}
void actionFramer::addEndFramer(std::map<string, double> angles_)
{
	FKFrame frame1;
	std::map<string, double>::iterator rli;
	frame1.setDuration(0.5);
	for (rli = angles_.begin(); rli != angles_.end(); ++rli)
	{
		frame1.addAngle(rli->first, rli->second);
	}
	frames_.push_back(frame1);
}
void actionFramer::clearFrame()
{

	frames_t::iterator itr = frames_.begin();
	frames_t::iterator tem = frames_.begin();
	while (itr != frames_.end())
	{
		tem = itr;
		itr++;
		frames_.erase(tem);
	}
	totalFrames_ = 0;

}