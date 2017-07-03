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
void StepStrategy::computexCapture()   //���ݳ�ʼλ�ü���xcapture
{
	//1.˵������ѹ���������ڽż⴦��������ڿ��аڶ�ֱ�����ʱ�̵�����λ�ú��ٶȼ��㲶׽��
	curCOP = humanData.rf;				   //ѹ�������ڽż�
	updateState(humanData.stepTime);	  //����ָ��ʱ�̵�����λ�ú��ٶ�
	xCapture = xPos + xRate / humanData.w;    //���㲶׽��
											  //2.��������ֵ
	ankleNeedMove = xCapture;             //ָʾ�׹ؽ��ƶ�����  ���ڹ滮�׹ؽڹ켣
										  //��������Ҫ  BUG���������� 
	inixPos = markinixPos;                 //�ָ����ĵĳ�ʼ״̬
	inixRate = markinixRate;              //�ָ����ĵĳ�ʼ״̬
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
	//״̬���°����������֣�1.����״̬����   2.�׹ؽ�״̬����
	double kx = ankleNeedMove / pow(humanData.stepTime, 2);
	double kz = -((humanData.hstep) / pow(humanData.stepTime / 2, 2));
	double number1 = humanData.Tmax / (humanData.m*humanData.l*pow(humanData.w, 2));
	//����״̬����
	//TR2ʱ��ǰ �����ֵĵ������˶�ѧ��ʽ ѹ������ Ϊ rf ���ż⴦��  ��ʼ�ٶȺ�λ�� Ϊ���������������ų��ٶȺͳ�ʼλ��
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
		else             //TR2ʱ�̺��Ϊ����ʱ��Σ�1����TR2~stepTime֮��   2��غ�ѹ�������л� ���ļ����˶�ֱ����ֹ			
		{
			if (t <= humanData.stepTime) //ʱ���1���Դ��ڰ���״̬
			{

				inixPos = markinixPos;
				inixRate = markinixRate;
				updateState(humanData.TR2);   //���TR2ʱ�̣����������ý���ʱ�̵������ٶȺ�λ��
				inixPos = xPos;				  //��ΪTR2~stepTimeʱ��εĳ�ʼ����״̬
				inixRate = xRate;
				timeoffset = humanData.TR2;   //TR2ʱ�̺�������¼�ʱ����������ʱ��ƫ��ΪTR2
				curCOP = humanData.rf;        //TR2~stepTimeʱ��� ѹ��������Ϊrf ���ż�	
			}
			else
			{
				if (curCOP != xCapture) // ʱ���2���ڶ�����أ�ѹ�������л�  ��ֻ������һ�Σ�����  
				{
					updateState(humanData.stepTime);   //��ð������ʱ�̣���stepTimeʱ�̵������ٶȺ�λ��
					inixPos = xPos;					   //��ΪstepTime~N ʱ��εĳ�ʼ����״̬
					inixRate = xRate;
					timeoffset = humanData.stepTime;   //stepTimeʱ�̺�������¼�ʱ����������ʱ��ƫ��ΪstepTime
					curCOP = xCapture;                 //stepTime~Nʱ���  ѹ������λ�øı� �� ��׽��λ��
				}
			}
			xPos = curCOP + (inixPos - curCOP)*cosh(humanData.w*(t - timeoffset)) + inixRate / humanData.w*(sinh(humanData.w*(t - timeoffset)));
			xRate = humanData.w*(inixPos - curCOP)*sinh(humanData.w*(t - timeoffset)) + inixRate*cosh(humanData.w*(t - timeoffset));
			zPos = sqrt(pow(humanData.l, 2) - pow(xPos, 2));
		}

		//�׹ؽ�״̬����
		if (t <= humanData.stepTime)//stepTimeʱ��ǰ���ڶ����ڿ���
		{
			anklexPos = kx*t*t;
			anklezPos = kz*t*(t - humanData.stepTime);
		}
		else                       //stepTimeʱ�̺󣬰ڶ��ȵ��׹ؽ���ص�ankleNeedMove����
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

//�� ����δ�õ�

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
		ModelData();  //��õ�ǰʱ�̵����ĵ�ƫ��

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
		//cout << t << " ����ƫ�����ݣ�  " << pos.px << "  " << pos.pz << endl;
		//cout<<" ����״̬ " << state.xpos << "   " << state.xrate << "  "<<state.zpos<< endl;
		//cout << "BodyPitch" << iter4->second << endl;
	}
}
void StepStrategy::outToModel()
{

}
