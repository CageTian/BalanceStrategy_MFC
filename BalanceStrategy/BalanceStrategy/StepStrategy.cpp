#include "stdafx.h"
#include "StepStrategy.h"


bool StepStrategy::IsUseful(double xpos, double xrate)
{
	xCapture = xrate / humanData.w + xpos;
	double H = humanData.Tmax / (humanData.m*humanData.g)*pow(exp(-humanData.w*humanData.TR1) - 1, 2);
	double min = (-humanData.lf - H + H + humanData.rstep - (-humanData.lf))*exp(-humanData.w*humanData.stepTime) + (-humanData.lf) - H;
	double max = (humanData.rf + H - H + humanData.rstep - humanData.rf)*exp(-humanData.w*humanData.stepTime) + humanData.rf + H;
	if (xCapture <= max &&xCapture >= min)
	{
		return true;
	}
	else
		return false;
}
void StepStrategy::startStrategy(double xpos, double xrate)
{
	setPosAndRate(xpos, xrate);
	computexCapture();
	
}
void StepStrategy::setPosAndRate(double xpos, double xrate)
{
	inixPos = xpos;
	inixRate = xrate;
	markinixPos = xpos;
	markinixRate = xrate;
}
void StepStrategy::computexCapture()   //根据初始位置计算xcapture
{
	//1.说明：将压力中心落在脚尖处；计算脚在空中摆动直到落地时刻的质心位置和速度计算捕捉点
	curCOP = humanData.rf;				   //压力中心在脚尖
	updateState(humanData.stepTime);	  //计算指定时刻的质心位置和速度
	xCapture = xPos + xRate / humanData.w;    //计算捕捉点
											  //2.给变量赋值
	ankleNeedMove = xCapture;             //指示踝关节移动距离  用于规划踝关节轨迹
										  //！！！重要  BUG出处！！！ 
	inixPos = markinixPos;                 //恢复质心的初始状态
	inixRate = markinixRate;              //恢复质心的初始状态
}

bool StepStrategy::isTR2(double t)
{
	if (EnterTR2OneTime == 1 && t - humanData.TR2forStep <= humanData.sampleTime && t - humanData.TR2forStep >= -humanData.sampleTime)
	{
		
		EnterTR2OneTime = 0;
		return true;
	}
	return false;

}
void StepStrategy::updateState(double t)
{
	//状态更新包括两个部分：1.质心状态更新   2.踝关节状态更新
	double kx = ankleNeedMove / pow(humanData.stepTime, 2);
	double kz = -((humanData.hstep) / pow(humanData.stepTime / 2, 2));
	double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
	//质心状态更新
	//TR2时刻前 带飞轮的倒立摆运动学公式 压力中心 为 rf （脚尖处）  初始速度和位置 为策略最初传入的受扰初速度和初始位置
	if (sqrt(pow(humanData.l, 2) - pow(xPos, 2)) >= 0.0001)
	{
		if (t <= humanData.TR1)
		{
			xPos = humanData.rf + (markinixPos - humanData.rf)*cosh(humanData.w*t) + markinixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 1);
			xRate = humanData.w*(markinixPos - humanData.rf)*sinh(humanData.w*t) + markinixRate*cosh(humanData.w*t) - number1*humanData.w*sinh(humanData.w*t);
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
		else             //TR2时刻后分为两个时间段：1摆腿TR2~stepTime之间   2落地后压力中心切换 质心继续运动直到静止			
		{
			if (t <= humanData.stepTime) //时间段1：仍处于摆腿状态
			{

				inixPos = markinixPos;
				inixRate = markinixRate;
				updateState(humanData.TR2);   //获得TR2时刻，即飞轮作用结束时刻的质心速度和位置
				inixPos = xPos;				  //设为TR2~stepTime时间段的初始质心状态
				inixRate = xRate;
				timeoffset = humanData.TR2;   //TR2时刻后从零重新计时，所以设置时间偏移为TR2
				curCOP = humanData.rf;        //TR2~stepTime时间段 压力中心仍为rf 即脚尖	
			}
			else
			{
				if (curCOP != xCapture) // 时间段2：摆动腿落地，压力中心切换  （只进入了一次！！）  
				{
					updateState(humanData.stepTime);   //获得摆腿落地时刻，即stepTime时刻的质心速度和位置
					inixPos = xPos;					   //设为stepTime~N 时间段的初始质心状态
					inixRate = xRate;
					timeoffset = humanData.stepTime;   //stepTime时刻后从零重新计时，所以设置时间偏移为stepTime
					curCOP = xCapture;                 //stepTime~N时间段  压力重心位置改变 到 捕捉点位置
				}
			}
			xPos = curCOP + (inixPos - curCOP)*cosh(humanData.w*(t - timeoffset)) + inixRate / humanData.w*(sinh(humanData.w*(t - timeoffset)));
			xRate = humanData.w*(inixPos - curCOP)*sinh(humanData.w*(t - timeoffset)) + inixRate*cosh(humanData.w*(t - timeoffset));
			zPos = sqrt(pow(humanData.l, 2) - pow(xPos, 2));
		}

		//踝关节状态更新
		if (t <= humanData.stepTime)//stepTime时刻前，摆动腿在空中
		{
			anklexPos = kx*t*t;
			anklezPos = kz*t*(t - humanData.stepTime);
		}
		else                       //stepTime时刻后，摆动腿的踝关节落地到ankleNeedMove处。
		{
			anklexPos = ankleNeedMove;
			anklezPos = 0;
		}
	}
	else
	{
		xPos = humanData.l;
		xRate = 0;
		zPos = 0;
		BodyPitch = 90;
	}
}
bool StepStrategy::isEnd()
{
	return xRate <= humanData.rateLimit && xRate >= -humanData.rateLimit;
}
void StepStrategy::ModelDataFormat()
{
	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);
	COMAngle = asin(hipPos / humanData.legLen) * 180 / pi;
	COMOffset.px = sin(COMAngle*pi / 180)*humanData.legLen;
	COMOffset.pz = cos(COMAngle*pi / 180)*humanData.legLen;

}

//以 下尚未用到

void StepStrategy::startStrategyAuto(double xpos, double xrate)
{
	setPosAndRate(xpos, xrate);
	computexCapture();
	takeStep();
	output();

}

void StepStrategy::takeStep()
{
	for (STEP_time = 0; !isEnd(); STEP_time += humanData.sampleTime)
	{
		updateState(STEP_time);
		ModelData();  //获得当前时刻的质心的偏移

	}
}
void StepStrategy::ModelData()
{
	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);
	COMAngle = asin(hipPos / humanData.legLen) * 180 / pi;
	COMOffset.px = sin(COMAngle*pi / 180)*humanData.legLen;
	COMOffset.pz = cos(COMAngle*pi / 180)*humanData.legLen;

	COMStates.insert(std::make_pair(STEP_time, COMState(xPos, xRate, zPos)));
	AnkleStates.insert(std::make_pair(STEP_time, AnkleState(anklexPos, anklezPos)));
	BodyPitchs.insert(std::make_pair(STEP_time, BodyPitch));
	HipAngles.insert(std::make_pair(STEP_time, COMAngle));
	Offsets.insert(std::make_pair(STEP_time, COMOffset));

}
void StepStrategy::output()
{
	std::map<double, Offset> ::iterator iter;
	std::map<double, double> ::iterator iter2;
	std::map<double, COMState> ::iterator iter3;
	std::map<double, double> ::iterator iter4;
	for (iter = Offsets.begin(), iter2 = HipAngles.begin(), iter3 = COMStates.begin(), iter4 = BodyPitchs.begin(); iter != Offsets.end(), iter2 != HipAngles.end(), iter3 != COMStates.end(), iter4 != BodyPitchs.end(); iter++, iter2++, iter3++, iter4++)
	{
		double t = iter->first;
		Offset pos = iter->second;
		double angle = iter2->second;
		COMState  state = iter3->second;
		//cout << t << " 质心偏移数据：  " << pos.px << "  " << pos.pz << endl;
		//cout<<" 质心状态 " << state.xpos << "   " << state.xrate << "  "<<state.zpos<< endl;
		//cout << "BodyPitch" << iter4->second << endl;
	}
}
void StepStrategy::outToModel()
{

}
