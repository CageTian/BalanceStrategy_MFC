#include "stdafx.h"
#include "MultStepStrategy.h"

int  MultStepStrategy::IsUseful(double xpos, double xrate)
{
	double H = humanData.Tmax / (humanData.m*humanData.g)*pow(exp(-humanData.w*humanData.TR1forStep) - 1, 2);
	double xcp = xrate / humanData.w + xpos;
	double min = -humanData.lf - H;
	double max = humanData.rf + H;
	double limits[20][2];//跨20步限制?
	for (int i = 0; i<humanData.maxSteps; i++)
	{

		limits[i][0] = min = (min + H + humanData.rstep - (-humanData.lf))*exp(-humanData.w*humanData.stepTime) + (-humanData.lf) - H;
		limits[i][1] = max = (max - H + humanData.rstep - humanData.rf)*exp(-humanData.w*humanData.stepTime) + humanData.rf + H;

		cout << "look!!!!" << i << "  " << limits[i][0] * humanData.w << "  " << limits[i][1] * humanData.w << endl;
		if (xcp >= limits[i][0] && xcp <= limits[i][1])
			return i + 1;

	}
	return -1;
}
void MultStepStrategy::startStrategy(double xpos, double xrate)
{
	setPosAndRate(xpos, xrate);
	computexCapture();
	setPosAndRate(xpos, xrate);

}
void MultStepStrategy::setPosAndRate(double xpos, double xrate) //设置质心数据
{
	inixPos = xpos;
	inixRate = xrate;
	xPos = xpos;
	xRate = xrate;

}
void MultStepStrategy::computexCapture()         //根据初始位置计算xcapture
{
	NStep.swingLeginitialPos = NStep.lastAnkleNeedMove;
	NStep.lastAnkleNeedMove = NStep.ankleNeedMove;

	if (NStep.totalStep == 1)
	{
		double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
		double t = humanData.TR2forStep;
		xPos = humanData.rf + (inixPos - humanData.rf)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 2 * cosh(humanData.w*(t - humanData.TR2forStep / 2)) + 1);
		xRate = humanData.w*(inixPos - humanData.rf)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t) - number1*humanData.w*(sinh(humanData.w*t) - 2 * sinh(humanData.w*(t - humanData.TR2forStep / 2)));
		inixPos = xPos;
		inixRate = xRate;
		updateFirstCOMState(humanData.stepTime);  //摆腿结束时
		NStep.ankleNeedMove = xPos + xRate / humanData.w - humanData.rf;
		inixPos = NStep.xPos;
		inixRate = NStep.xRate;

	}
	else if (NStep.curStep != NStep.totalStep)
		NStep.ankleNeedMove = humanData.rstep*NStep.curStep;

	else if (NStep.curStep == NStep.totalStep) //到了最后一步
	{
		updateLastCOMState(humanData.stepTime*NStep.curStep);
		NStep.ankleNeedMove = xPos + xRate / humanData.w - humanData.rf;
	}
	NStep.xCapture = NStep.ankleNeedMove + humanData.rf;
	// std::cout << "在改变ankleneedmove  " << NStep.ankleNeedMove << "  "<< humanData.rstep+humanData.rf<<"   "<< xPos << "  " << xRate << "  " << humanData.w<< endl;

}
void MultStepStrategy::updateStateAnkle(double t)
{
	cout << "NStep.swingLeginitialPos" << NStep.swingLeginitialPos << endl;
	double kx = (NStep.ankleNeedMove - NStep.swingLeginitialPos) / pow(humanData.stepTime, 2);
	double kz = -((humanData.hstep) / pow(humanData.stepTime / 2, 2));
	if (t <= humanData.stepTime*NStep.curStep)//TR2时刻前，摆动腿在空中，压力中心位置保持不变。
	{
		anklexPos = NStep.swingLeginitialPos + kx*pow(t - humanData.stepTime*(NStep.curStep - 1), 2);
		anklezPos = kz*(t - (NStep.curStep - 1)*humanData.stepTime)*(t - NStep.curStep*humanData.stepTime);
	}
	else
	{
		anklexPos = NStep.ankleNeedMove;
		anklezPos = 0;
	}

}
void MultStepStrategy::updateState(double t) {
	updateStateAnkle(t);
	cout << "NStep.curStep  " << NStep.curStep << endl;
	if (NStep.curStep == 1)
		updateFirstCOMState(t);
	else if (NStep.curStep != NStep.totalStep)
		updateMidCOMState(t);
	else
		updateLastCOMState(t);

}
void MultStepStrategy::updateFirstCOMState(double t)
{
	/*if (sqrt(pow(humanData.l, 2) - pow(xPos, 2)) >= 0.0001)
	{*/
	if (t <= humanData.TR1forStep)
	{
		double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
		xPos = humanData.rf + (inixPos - humanData.rf)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 1);
		xRate = humanData.w*(inixPos - humanData.rf)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t) - number1*humanData.w*sinh(humanData.w*t);
		BodyPitch = (t*t*humanData.Tmax / humanData.J / 2) * 180 / pi;
	}
	else if (t <= humanData.TR2forStep)
	{
		double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
		xPos = humanData.rf + (inixPos - humanData.rf)*cosh(humanData.w*t) + inixRate / humanData.w*(sinh(humanData.w*t)) - number1*(cosh(humanData.w*t) - 2 * cosh(humanData.w*(t - humanData.TR2forStep / 2)) + 1);
		xRate = humanData.w*(inixPos - humanData.rf)*sinh(humanData.w*t) + inixRate*cosh(humanData.w*t) - number1*humanData.w*(sinh(humanData.w*t) - 2 * sinh(humanData.w*(t - humanData.TR2forStep / 2)));
		BodyPitch = (humanData.QmaxforStep - (pow((humanData.TR2forStep - t), 2)*humanData.Tmax / humanData.J / 2)) * 180 / pi;
		if (NStep.isTR2)
		{
			cout << "yijingdaole " << t << "  " << inixPos << "  " << inixRate << endl;
			inixPos = xPos; //注意！！！  TR2时刻后第一步仍然没有跨完
			inixRate = xRate;
			NStep.isTR2 = 0;
			cout << "yijingdaole " << t << "  " << inixPos << "  " << inixRate << humanData.w*(0 - humanData.rf)*sinh(humanData.w*t) + 0.7*cosh(humanData.w*t) - number1*humanData.w*(sinh(humanData.w*t) - 2 * sinh(humanData.w*(t - humanData.TR2forStep / 2)))
				<< endl;
		}
	}
	else
	{

		double timeoffset = humanData.TR2forStep;
		if (t > humanData.stepTime)//说明策略只有一步
		{
			timeoffset = humanData.stepTime;
		}


		xPos = NStep.curCOP + (inixPos - NStep.curCOP)*cosh(humanData.w*(t - humanData.TR2forStep)) + inixRate / humanData.w*(sinh(humanData.w*(t - humanData.TR2forStep)));
		xRate = humanData.w*(inixPos - NStep.curCOP)*sinh(humanData.w*(t - humanData.TR2forStep)) + inixRate*cosh(humanData.w*(t - humanData.TR2forStep));
		BodyPitch = 180 / pi*humanData.QmaxforStep;

	}
	/*}
	else
	{
	xPos = humanData.l;
	xRate = 0;
	zPos = 0;
	BodyPitch = 90;
	}*/
}
void MultStepStrategy::updateMidCOMState(double t)
{
	/*if (sqrt(pow(humanData.l, 2) - pow(xPos, 2)) >= 0.0001)
	{*/
	double timeOffset = humanData.stepTime*(NStep.curStep - 1);
	xPos = NStep.curCOP + (inixPos - NStep.curCOP)*cosh(humanData.w*(t - timeOffset)) + inixRate / humanData.w*(sinh(humanData.w*(t - timeOffset)));
	xRate = humanData.w*(inixPos - NStep.curCOP)*sinh(humanData.w*(t - timeOffset)) + inixRate*cosh(humanData.w*(t - timeOffset));
	BodyPitch = 180 / pi*humanData.QmaxforStep;
	/*}
	else
	{
	xPos = humanData.l;
	xRate = 0;
	zPos = 0;
	BodyPitch = 90;
	}*/
}
void MultStepStrategy::updateLastCOMState(double t)
{
	/*if (sqrt(pow(humanData.l, 2) - pow(xPos, 2)) >= 0.0001)
	{*/
	double timeOffset = humanData.stepTime*(NStep.curStep - 1);
	if (t > humanData.stepTime*NStep.curStep)
		timeOffset = humanData.stepTime*(NStep.curStep);
	xPos = NStep.curCOP + (inixPos - NStep.curCOP)*cosh(humanData.w*(t - timeOffset)) + inixRate / humanData.w*(sinh(humanData.w*(t - timeOffset)));
	xRate = humanData.w*(inixPos - NStep.curCOP)*sinh(humanData.w*(t - timeOffset)) + inixRate*cosh(humanData.w*(t - timeOffset));
	BodyPitch = 180 / pi*humanData.QmaxforStep;
	/*}
	else
	{
	xPos = humanData.l;
	xRate = 0;
	zPos = 0;
	BodyPitch = 90;
	}*/
}
void MultStepStrategy::ModelDataFormat(double t)
{
	double hipPos = xPos - sin(BodyPitch*pi / 180)*(humanData.l - humanData.legLen);

	if (NStep.isStanceStraight == 0) {
		initialkneePos = NStep.lastAnkleNeedMove - sin(pi / 180 * initialCOMAngle)*humanData.lowerleg;
		HipAngle = -asin((hipPos - initialkneePos) / humanData.upperleg) * 180 / pi;
		KneeAngle = initialCOMAngle - HipAngle;

		if (KneeAngle >= 0) {
			HipAngle = -asin((hipPos - NStep.lastAnkleNeedMove) / humanData.legLen) * 180 / pi;
			KneeAngle = 0;
			NStep.isStanceStraight = 1;
		}
	}
	else {
		initialCOMAngle = 0;
		HipAngle = -asin((hipPos - NStep.lastAnkleNeedMove) / humanData.legLen) * 180 / pi;
		KneeAngle = 0;
	}

	zPos = humanData.lowerleg*cos((HipAngle + KneeAngle)*pi / 180) + humanData.upperleg*cos(HipAngle*pi / 180) + (humanData.l - humanData.legLen)*cos(BodyPitch*pi / 180);
	COMOffset.px = hipPos;
	COMOffset.pz = zPos - (humanData.l - humanData.legLen)*cos(BodyPitch*pi / 180);

}
void MultStepStrategy::endOfEachStep(Vector3 offset)
{
	if (NStep.curStep != NStep.totalStep)
	{
		std::cout << "进入到下一步 " << xPos << "  " << "  " << xRate << endl;
		NStep.curStep++; //进入到下一步
		NStep.xPos = xPos;
		NStep.xRate = xRate;
		NStep.curCOP = humanData.rf + NStep.ankleNeedMove;
		NStep.isCurStep = 0;//开始进行下一步
		NStep.legToTake = (NStep.legToTake + 1) % 2;//先出左腿
		enterStanceOneTime = 1;
		EnterStepTimeOneTime = 1;
		NStep.offset = offset;//记录偏移坐标
	}
	else
	{
		inixPos = xPos; //TR2时刻后仍然需要继续运动，直到速度为0
		inixRate = xRate;
		NStep.curCOP = humanData.rf + NStep.ankleNeedMove;
		std::cout << "最后一步，则在TR2时刻后还要继续  " << endl;

	}
	NStep.isStepTime = 0;
}
bool MultStepStrategy::isEnd()
{
	return xRate <= humanData.rateLimit && xRate >= -humanData.rateLimit;
}

bool MultStepStrategy::isTR2(double t)
{
	//每一步只可以进入一次  ; 实时的数据不可能出现刚好等于TR2的情况，因此只能在一定误差内
	if (EnterTR2OneTime == 1 && t - humanData.TR2forStep <= humanData.sampleTime && t - humanData.TR2forStep >= -humanData.sampleTime)
	{
		NStep.isTR2 = 1;
		EnterTR2OneTime = 0;
		return true;
	}
	return false;

}
bool MultStepStrategy::isStepTime(double t)
{
	//每一步只可以进入一次  ; 实时的数据不可能出现刚好等于TR2的情况，因此只能在一定误差内
	if (EnterStepTimeOneTime == 1 && t - humanData.stepTime*NStep.curStep <= humanData.sampleTime && t - humanData.stepTime*NStep.curStep >= -humanData.sampleTime)
	{
		NStep.isStepTime = 1;
		EnterStepTimeOneTime = 0;
		return true;
	}
	return false;

}
void  MultStepStrategy::ModelData()
{

}

void  MultStepStrategy::takeMultSteps()
{
	{	                                                    //给第一步Nstep赋值
		NStep.totalStep = IsUseful(inixPos, inixRate);
		NStep.curStep = 1;
		NStep.xPos = inixPos;
		NStep.xRate = inixRate;
		NStep.curCOP = humanData.rf;
		NStep.isCurStep = 0;   //还没到第一步呢！！！ 只是在第一步即将开始之前 赋给Nstep状态值
		double pos = (NStep.legToTake == 1) ? pos = humanData.hipwidth / 2 : -humanData.hipwidth / 2;
		NStep.offset = Vector3(pos, 0, 0);
		MSTEP_time = -humanData.sampleTime;
		cout << "采取了多跨步策略需要走的步数为:  " << NStep.totalStep << endl;
	}


	while (NStep.curStep <= NStep.totalStep)   //连续采取多步
	{

		if (!NStep.isCurStep)
		{  // 修改isCurStep 进入当前步  赋值
			startStrategy(NStep.xPos, NStep.xRate);
			NStep.isCurStep = 1; //是新的一步
								 //std::cout << "current move leg  " << NStep.leg[(NStep.legToTake) % 2] << endl;
		}

		else  //已经进入到当前步中
		{
			/////////////////////////////////////////////////////////////////////////////////////// time
			MSTEP_time += humanData.sampleTime;
			if (isTR2(MSTEP_time))
			{
				MSTEP_time = humanData.TR2forStep;
				std::cout << "!!!!!!!!!!!!!!!!!!!!!已经到下一TR2了哈  " << inixPos << "  " << inixRate << "  " << MSTEP_time << xRate << "  " << xPos << endl;
			}

			else if (isStepTime(MSTEP_time))
			{
				MSTEP_time = humanData.stepTime*NStep.curStep;
				std::cout << "已经到下一stepTime了哈  " << MSTEP_time << endl;
			}

			std::cout << "跨步时间  " << MSTEP_time << endl;
		}

	}
}

