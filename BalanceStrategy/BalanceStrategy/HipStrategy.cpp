#include "stdafx.h"
#include "HipStrategy.h"
#define E 2.7182818284



bool HipStrategy::IsUseful(double xpos, double xrate)
{
	xCapture = xrate / humanData.w + xpos;
	double H = humanData.Tmax / (humanData.m*humanData.g)*pow(exp(-humanData.w*humanData.TR1) - 1, 2);
	//cout <<"框架 "<<xCapture << "  " << H<<"  "<<H + humanData.rf<<endl;
	if (xCapture <= H + humanData.rf &&xCapture >= -H - humanData.rf)
		return true;
	else
		return false;

}
void HipStrategy::startStrategy(double xpos, double xrate)
{
	setPosAndRate(xpos, xrate);
}
void HipStrategy::setPosAndRate(double xpos, double xrate)
{
	inixPos = xpos;
	inixRate = xrate;
	xRate = xrate;
	xPos = xpos;
}
void HipStrategy::updateState(double t)
{
	double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
	if (t <= humanData.TR1)
	{

		xPos = humanData.rf + (inixPos - humanData.rf)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 1);
		xRate = humanData.w*(inixPos - humanData.rf)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t) - number1*humanData.w*sinh(humanData.w*t);
		zPos = sqrt(pow(humanData.l, 2) - pow(xPos, 2));
		BodyPitch = (t*t*humanData.Tmax / humanData.J / 2) * 180 / pi;
	}
	else if (t <= humanData.TR2)
	{
		xPos = humanData.rf + (inixPos - humanData.rf)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 2 * cosh(humanData.w*(t - humanData.TR2 / 2)) + 1);
		xRate = humanData.w*(inixPos - humanData.rf)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t) - number1*humanData.w*(sinh(humanData.w*t) - 2 * sinh(humanData.w*(t - humanData.TR2 / 2)));
		zPos = sqrt(pow(humanData.l, 2) - pow(xPos, 2));
		BodyPitch = (humanData.Qmax - (pow((humanData.TR2 - t), 2)*humanData.Tmax / humanData.J / 2)) * 180 / pi;
	}
}
void HipStrategy::ModelDataFormat()
{


	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);
	COMAngle = asin(hipPos / humanData.legLen) * 180 / pi;
	COMOffset.px = sin(COMAngle*pi / 180)*humanData.legLen;
	COMOffset.pz = cos(COMAngle*pi / 180)*humanData.legLen;

}
bool HipStrategy::isEnd(double time) {
	//cout << "!!!" << time<<"  "<<time-humanData.TR2 <<"  "<< humanData.TR2 << endl;
	if (markEndFlag == 0 && time - humanData.TR2 <= 0.000001 && time - humanData.TR2 >= -0.000001)
	{  //有偏差 不可能完全等于
		markEndFlag = 1;
		return false;//等TR2时刻运行完成再结束髋关节调节策略
	}
	if (markEndFlag == 1)
		return true;

	return false;
}

//以 下尚未用到
void HipStrategy::output()
{
	std::map<double, Point> ::iterator iter;
	for (iter = COM.begin(); iter != COM.end(); iter++)
	{
		double t = iter->first;
		Point pos = iter->second;
		cout << t << "  " << pos.px << "  " << pos.py << "  " << pos.pz << "  " << endl;
	}
}
void HipStrategy::outToModel()
{
	double angle = 0;
	double offx = 0, offz = 0;
	Offset off;
	std::map<double, Point> ::iterator iter;
	for (iter = COM.begin(); iter != COM.end(); iter++)
	{
		angle = asin(iter->second.pz / humanData.l);
		COMData.insert(std::make_pair(iter->first, angle * 180 / pi));
		offx = iter->second.pz;
		offz = humanData.l - sqrt(pow(humanData.l, 2) - pow(offx, 2));
		off.px = offx;
		off.pz = -offz;
		offset.insert(pair<double, Offset>(iter->first, off));
	}
}