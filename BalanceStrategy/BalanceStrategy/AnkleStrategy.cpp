#include "stdafx.h"
#include "AnkleStrategy.h"
#include <math.h>


bool AnkleStrategy::IsUseful(double xpos, double xrate)
{
	double xCapture = xrate / humanData.w + xpos;
	//cout << "look "<<xCapture << endl;
	if (xCapture <= humanData.rf)
	{
		curCOP = xCapture;   //设置当前压力中心位置
		return true;
	}
	else
		return false;
}

void AnkleStrategy::startStrategy(double xpos, double xrate)
{
	setPosAndRate(xpos, xrate);


}
void  AnkleStrategy::setPosAndRate(double xpos, double xrate)
{
	inixPos = xpos;
	inixRate = xrate;
	xRate = xrate;
	xPos = xpos;
}

void AnkleStrategy::updateState(double t)
{
	if (sqrt(pow(humanData.l, 2) - pow(xPos, 2)) >= 0.0001)
	{
		xPos = curCOP + (inixPos - curCOP)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t));
		xRate = humanData.w*(inixPos - curCOP)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t);
		zPos = sqrt(pow(humanData.l, 2) - pow(xPos, 2));
		BodyPitch = 0;
	}
	else
	{
		xPos = humanData.l;
		xRate = 0;
		zPos = 0;
		BodyPitch = 90;
	}
}

void AnkleStrategy::ModelDataFormat(double angle)
{
	/*BodyPitch = angle;*/
	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);
	COMAngle = asin(hipPos / humanData.legLen) * 180 / pi;
	COMOffset.px = sin(COMAngle*pi / 180)*humanData.legLen;
	COMOffset.pz = cos(COMAngle*pi / 180)*humanData.legLen;
	//保存到相应集合中
	Offsets.insert(std::make_pair(ANKLE_time, COMOffset));
	HipAngles.insert(std::make_pair(ANKLE_time, COMAngle));

}
bool AnkleStrategy::isEnd()
{
	return xRate <= humanData.rateLimit && xRate >= -humanData.rateLimit;
}


//以 下尚未用到

void AnkleStrategy::startStrategyAuto(double xpos, double xrate, bool iship)
{
	setPosAndRate(xpos, xrate);
	if (iship == 1)
	{
		isHip = 1;
		BodyPitch = humanData.Qmax;
	}
	takeAnkle();
	output();
}
void AnkleStrategy::takeAnkle()
{
	for (ANKLE_time = 0; !isEnd(); ANKLE_time += humanData.sampleTime)
	{
		updateState(ANKLE_time);
		ModelData();  //获得当前时刻的质心的偏移

	}

}
void AnkleStrategy::ModelData()
{
	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);
	COMAngle = asin(hipPos / humanData.legLen) * 180 / pi;
	COMOffset.px = sin(COMAngle*pi / 180)*humanData.legLen;
	COMOffset.pz = cos(COMAngle*pi / 180)*humanData.legLen;
	//保存到相应集合中
	Offsets.insert(std::make_pair(ANKLE_time, COMOffset));
	HipAngles.insert(std::make_pair(ANKLE_time, COMAngle));
	COMStates.insert(std::make_pair(ANKLE_time, COMState(xPos, xRate, zPos)));
}
void AnkleStrategy::output()
{

	std::map<double, Offset> ::iterator iter;
	std::map<double, double> ::iterator iter2;
	std::map<double, COMState> ::iterator iter3;
	for (iter = Offsets.begin(), iter2 = HipAngles.begin(), iter3 = COMStates.begin(); iter != Offsets.end(), iter2 != HipAngles.end(), iter3 != COMStates.end(); iter++, iter2++, iter3++)
	{
		double t = iter->first;
		Offset pos = iter->second;
		double angle = iter2->second;
		COMState  state = iter3->second;
		cout << t << " 质心偏移数据：  " << pos.px << "  " << pos.pz << " 质心状态 " << state.xpos << "   " << state.xrate << endl;
	}

}
