#include "stdafx.h"
#include "balanceControl.h"

bool isAnkleProfileEnd = false;
bool isCOMProfileEnd = false;
double currentDataTime = -1;
double StartTime = -1;
double AnkleStartTime = -1;
double NStepMarkFlag = 0;
Vector3 v1, v2;
bool animating = false;
bool skinning = true;
bool wireframe = false;
bool action = true;
bool yacobi = false;
bool walking = true;

char *currentChooseSrc = new char[11];
char *currentChooseIK = new char[11];
GLdouble currentChooseAngle1 = 0.0;
GLdouble currentChooseAngle2 = 0.0;
char currentChooseAct = 0;
Vector3 currentChooseTranslate;
bool ToRecoverBalance = false;

Vector3 markRootPos;
Vector3 tempRootPos;
std::map<std::string, double>angles_;
int Flag4 = 0;

double currentStrategy = -1;
double Recover = 0;   //��ÿ�ʼ�ָ���վ��״̬��ʱ�䣬�Դ�Ϊ��־
					  /*Vector3 tempCOM;     */     //�ָ�ƽ��ʱ���м�ص�����λ��
double recoverT = 0;;
double Recover2 = 0;
double recoverT2 = 0;

//���Ա�־
bool isAngle = 0;
bool isHip = 0;
bool isStep = 0;
bool isMultStep = 0;

//�׹ؽڲ���
double ANKLE_startTime = -1;
double ANKLE_time =0;
double mark_ANKLE_time = 0;
std::map<double, double>ANKLE_HipAngles;  //�������ݼ�
std::map<double, Offset> ANKLE_Offsets;    //����ƫ�����ݼ�
std::map<double, COMState> ANKLE_States;    //����״̬���ݼ�
std::map<double, double>::iterator ANKLE_HipAngles_iter;  //�������ݼ�
std::map<double, Offset>::iterator ANKLE_Offsets_iter;    //����ƫ�����ݼ�
std::map<double, COMState>::iterator ANKLE_States_iter;    //����״̬���ݼ�


//�Źؽڲ���
double HIP_startTime = -1;
double HIP_time = 0;
double mark_HIP_time = 0;
double tempXPos;   //�����Źؽ����ý���ʱ�� ���ĵ�λ�ú��ٶ� �������׹ؽ�
double tempXRate;


//�粽����
double STEP_startTime = -1;
double mark_STEP_time = 0;
double STEP_time = 0;
double STEP_dis = 0;			//�ڶ���hip��ankle�ľ���
double STEP_hipAngle = 0;		//�ڶ��ȵ��Źؽ�ת��
double STEP_kneeAngle = 0;	//�ڶ��ȵ�ϥ�ؽ�ת��
Point  STEP_tempHip;	    //�ڶ��ȸ�����hip��λ������
Point  STEP_tempAnkle;		//�ڶ��ȸ�����ankle��λ������

std::map<double, COMState>  STEP_COMStates;			 //����״̬���ݼ�
std::map<double, AnkleState>  STEP_AnkleStates;      //�׹ؽ�״̬���ݼ�
std::map<double, double> STEP_HipAngles;			 //�Źؽ�ƫת�Ƕȼ�
std::map<double, double> STEP_BodyPitchs;			 //����ת�Ǽ�
std::map<double, Offset> STEP_Offsets;				 //����ƫ�����ݼ�

std::map<double, COMState>::iterator STEP_COMStates_iter;    //����״̬���ݼ�
std::map<double, AnkleState>::iterator STEP_AnkleStates_iter;    //����״̬���ݼ�
std::map<double, double>::iterator STEP_HipAngles_iter;  //�������ݼ�
std::map<double, double>::iterator STEP_BodyPitchs_iter;
std::map<double, Offset>::iterator STEP_Offsets_iter;    //����ƫ�����ݼ�

//��粽����
double MSTEP_startTime = -1;
double MSTEP_time = 0;
double MSTEP_dis = 0;			//�ڶ���hip��ankle�ľ���
double MSTEP_hipAngle = 0;		//�ڶ��ȵ��Źؽ�ת��
double MSTEP_kneeAngle = 0;	//�ڶ��ȵ�ϥ�ؽ�ת��
double MSTEP_kneeAngle_flag = 0;
Point  MSTEP_tempHip;	    //�ڶ��ȸ�����hip��λ������
Point  MSTEP_tempAnkle;		//�ڶ��ȸ�����ankle��λ������
double MSTEP_NStepMarkFlag = 0;

void BalanceControl::updateAnkleModel()
{
	double bodypitch = 0;
	std::map<std::string, double> ::iterator it = angles_.find("Body");
	if (it != angles_.end())
		bodypitch = it->second;
	if (ankleStrategy.BodyPitch == 90)
		bodypitch = ankleStrategy.BodyPitch;

	///////////////////////////////////////////////��������ת��
	hskel.setArcAngle("Body", bodypitch);
	hskel.setArcAngle("LHipX", -ankleStrategy.COMAngle);
	hskel.setArcAngle("RHipX", -ankleStrategy.COMAngle);
	hskel.setArcAngle("RAnkle", ankleStrategy.COMAngle);
	hskel.setArcAngle("LAnkle", ankleStrategy.COMAngle);
	///////////////////////////////////////////////��������ƫ��
	hskel.getRootArc()->setTranslation(0, ankleStrategy.COMOffset.pz, ankleStrategy.COMOffset.px);
	hskin.update(&hskel);

}
void BalanceControl::updateHipModel()
{

	///////////////////////////////////////////////��������ת��
	hskel.setArcAngle("Body", hipStrategy.BodyPitch);
	hskel.setArcAngle("LHipX", -hipStrategy.COMAngle);
	hskel.setArcAngle("RHipX", -hipStrategy.COMAngle);
	hskel.setArcAngle("RAnkle", hipStrategy.COMAngle);
	hskel.setArcAngle("LAnkle", hipStrategy.COMAngle);
	///////////////////////////////////////////////��������ƫ��
	hskel.getRootArc()->setTranslation(0, hipStrategy.COMOffset.pz, hipStrategy.COMOffset.px);
	hskin.update(&hskel);
}
void BalanceControl::updateStepModel()
{
	//���ݴ��ȳ��� С�ȳ��� �Źؽ����� �׹ؽ����� �� �������Ҷ��� ����ڶ���״̬
	STEP_tempHip.setPoint(0, stepStrategy.zPos - cos(stepStrategy.BodyPitch*pi / 180)*(humanData.l - humanData.legLen), stepStrategy.xPos - sin(stepStrategy.BodyPitch*pi / 180)*(humanData.l - humanData.legLen));
	STEP_tempAnkle.setPoint(0, stepStrategy.anklezPos, stepStrategy.anklexPos);
	STEP_dis = sqrt(pow((STEP_tempHip.py - STEP_tempAnkle.py), 2) + pow((STEP_tempHip.pz - STEP_tempAnkle.pz), 2));
	if (humanData.upperleg + humanData.lowerleg == STEP_dis)
		STEP_hipAngle = STEP_kneeAngle = 0;
	else
	{
		if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(STEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) < 1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(STEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) > -1)
		{
			STEP_hipAngle = acos((pow(humanData.upperleg, 2) + pow(STEP_dis, 2) - pow(humanData.lowerleg, 2)) / (2 * humanData.upperleg*STEP_dis)) * 180 / pi;
			STEP_hipAngle -= asin((STEP_tempHip.pz - STEP_tempAnkle.pz) / STEP_dis) * 180 / pi;
		}
		if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(STEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) <= 1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(STEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) >= -1)
			STEP_kneeAngle = 180 - acos((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(STEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg)) * 180 / pi;

	}
	if (stepStrategy.BodyPitch == 90)
	{
		STEP_hipAngle = -stepStrategy.COMAngle;
		STEP_kneeAngle = 0;
	}
	///////////////////////////////////////////////��������ת��
	hskel.setArcAngle("Body", stepStrategy.BodyPitch);
	hskel.setArcAngle("LHipX", -stepStrategy.COMAngle);
	hskel.setArcAngle("LAnkle", stepStrategy.COMAngle);
	hskel.setArcAngle("RHipX", STEP_hipAngle);
	hskel.setArcAngle("RKnee", -STEP_kneeAngle);
	hskel.setArcAngle("RAnkle", STEP_kneeAngle - STEP_hipAngle);
	///////////////////////////////////////////////��������ƫ��
	hskel.getRootArc()->setTranslation(0, stepStrategy.COMOffset.pz, stepStrategy.COMOffset.px);
	hskin.update(&hskel);

}
void BalanceControl::updateMultStepModel()
{
	std::string moving = MstepStrategy.NStep.leg[MstepStrategy.NStep.legToTake];
	std::string notmoving = MstepStrategy.NStep.leg[(MstepStrategy.NStep.legToTake + 1) % 2];
	//���ݴ��ȳ��� С�ȳ��� �Źؽ����� �׹ؽ����� �� �������Ҷ��� ����ڶ���״̬
	MSTEP_tempHip.setPoint(0, MstepStrategy.zPos - cos(MstepStrategy.BodyPitch*pi / 180)*(humanData.l - humanData.legLen), MstepStrategy.xPos - sin(MstepStrategy.BodyPitch*pi / 180)*(humanData.l - humanData.legLen));
	MSTEP_tempAnkle.setPoint(0, MstepStrategy.anklezPos, MstepStrategy.anklexPos);
	MSTEP_dis = sqrt(pow((MSTEP_tempHip.py - MSTEP_tempAnkle.py), 2) + pow((MSTEP_tempHip.pz - MSTEP_tempAnkle.pz), 2));

	MSTEP_hipAngle = -asin((MSTEP_tempHip.pz - MstepStrategy.anklexPos) / humanData.legLen) * 180 / pi;
	MSTEP_kneeAngle = 5 * (MSTEP_kneeAngle_flag++);

	if (humanData.upperleg + humanData.lowerleg == MSTEP_dis)
		MSTEP_hipAngle = MSTEP_kneeAngle = 0;
	else
	{
		if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(MSTEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) < 1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(MSTEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) > -1)
		{
			MSTEP_hipAngle = acos((pow(humanData.upperleg, 2) + pow(MSTEP_dis, 2) - pow(humanData.lowerleg, 2)) / (2 * humanData.upperleg*MSTEP_dis)) * 180 / pi;
			MSTEP_hipAngle -= asin((MSTEP_tempHip.pz - MSTEP_tempAnkle.pz) / MSTEP_dis) * 180 / pi;
			MSTEP_kneeAngle_flag = 0;
		}
		if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(MSTEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) <= 1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(MSTEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) >= -1)
			MSTEP_kneeAngle = 180 - acos((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(MSTEP_dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg)) * 180 / pi;
	}

	///////////////////////////////////////////////��������ת��
	hskel.setArcAngle("Body", MstepStrategy.BodyPitch);
	hskel.setArcAngle(notmoving + "HipX", MstepStrategy.HipAngle);
	if (MstepStrategy.NStep.isStanceStraight == 0)
		hskel.setArcAngle(notmoving + "Knee", MstepStrategy.KneeAngle);
	hskel.setArcAngle(notmoving + "Ankle", -MstepStrategy.HipAngle - MstepStrategy.KneeAngle);
	hskel.setArcAngle(moving + "HipX", MSTEP_hipAngle);
	hskel.setArcAngle(moving + "Knee", -MSTEP_kneeAngle);
	if ((MstepStrategy.zPos - (humanData.l - humanData.legLen)*cos(MstepStrategy.BodyPitch*pi / 180) - humanData.upperleg*cos(MSTEP_hipAngle*pi / 180) - humanData.lowerleg*cos((MSTEP_hipAngle - MSTEP_kneeAngle)*pi / 180))<0.01)
		hskel.setArcAngle(moving + "Ankle", -MSTEP_hipAngle + MSTEP_kneeAngle);

	/////////////////////////////////////////////////////////////////////�޸�ƫ����
	hskel.getRootArc()->setTranslation(0, MstepStrategy.COMOffset.pz, MstepStrategy.COMOffset.px);
	hskin.update(&hskel);

	std::map<std::string, Vector3> positions;
	hskel.getArcPositions(positions);
	cout << "  ƫ�ư� ���" << MstepStrategy.NStep.ankleNeedMove << "  " << positions[moving + "Ankle"].getZ() << "   " << MstepStrategy.anklexPos << endl;
	cout << " ת�ǰ� ���" << MSTEP_hipAngle << "   " << MSTEP_kneeAngle << endl;


}
void BalanceControl::takeAnkle(std::map<std::string, double> angles_, Vector3 COM, double xpos, double xrate)
{
	if (ANKLE_startTime == -1)
	{
		hskel.setAngles(angles_);
		ankleStrategy.startStrategy(xpos, xrate);//��ʼ���ٶ� ʱ�� ��׽��
		ANKLE_startTime = 0;
		ANKLE_time =- humanData.sampleTime;
	}
	else if (ANKLE_startTime == 1)
	{
		if (ankleStrategy.BodyPitch != 90)
		{
			std::cout << "�׹ؽڻָ�����......" << endl;
			
		    ANKLE_time = 0;
			recoverT += humanData.sampleTime;
			Recover = 0;
			
			if (mark_ANKLE_time - (recoverT - Recover) >= 0)
			{
				ankleStrategy.updateState(mark_ANKLE_time - (recoverT - Recover));
				ankleStrategy.ModelDataFormat(hskel.getArcAngle("Body"));  //��õ�ǰʱ�̵����ĵ�ƫ��
				Point p(0, ankleStrategy.anklezPos, ankleStrategy.anklexPos);
				markAnkleProfile.push_back(p);
				Point p1(0, ankleStrategy.zPos, ankleStrategy.xPos);
				markCOMProfile.push_back(p1);
				updateAnkleModel();
			}
			else
			{
				ankleStrategy.updateState(0);
				ankleStrategy.ModelDataFormat(hskel.getArcAngle("Body"));  //��õ�ǰʱ�̵����ĵ�ƫ��
				Point p(0, ankleStrategy.anklezPos, ankleStrategy.anklexPos);
				markAnkleProfile.push_back(p);
				Point p1(0, ankleStrategy.zPos, ankleStrategy.xPos);
				markCOMProfile.push_back(p1);
				updateAnkleModel();
				HIP_startTime = 1;

								
				if (curStrategy == ANKLE)
				{
					initData();
					ankleStrategy.ReStart(humanData);
					isFinished = true;
				}
				else if (curStrategy == AUTO)
				{
					if (isHip == 0)
					{
						initData();
						ankleStrategy.ReStart(humanData);
						isFinished = true;
					}
				}
			}
		}
		else
		{
			initControl();
			initData();
			ankleStrategy.ReStart(humanData);
			hipStrategy.Restart(humanData);
			stepStrategy.Restart(humanData);
			MstepStrategy.Restart(humanData);
			isFinished = true;
		}
	}
	else
	{
		double t;
		if (!ankleStrategy.isEnd())
		{
	
			ANKLE_startTime = 0;
			ANKLE_time += humanData.sampleTime;
			ankleStrategy.updateState(ANKLE_time);
			ankleStrategy.ModelDataFormat(hskel.getArcAngle("Body"));  //��õ�ǰʱ�̵����ĵ�ƫ��
			updateAnkleModel();

			Point p(0, ankleStrategy.anklezPos, ankleStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, ankleStrategy.zPos, ankleStrategy.xPos);
			markCOMProfile.push_back(p1);

			std::cout << "�׵���  " << "  " << ankleStrategy.xPos << "   " << ankleStrategy.xRate << endl;

		}
		else
		{
			/*ANKLE_startTime = 1;*/
			 mark_ANKLE_time= ANKLE_time;
			 ANKLE_time = 0;
		}
	}
}
void BalanceControl::takeHip(double xpos, double xrate)
{
	if (HIP_startTime == -1)  //��һ�ν��� ��ʼ���粽���ԵĲ���
	{
		hipStrategy.startStrategy(xpos, xrate);//��ʼ���ٶ� ʱ�� ��׽��
		HIP_startTime = 0;
		HIP_time =- humanData.sampleTime;
	}
	else if (HIP_startTime == 1)
	{
		HIP_time = 0;
		if (tempRootPos.getY() > 0)
		{
			std::cout << "�Źؽڻָ�����......" << endl;

			recoverT += humanData.sampleTime;           //��ʼ����ƽ��ظ���ʱ��
			Recover = 0;

			if (mark_HIP_time - (recoverT - Recover) >= 0)
			{
				hipStrategy.updateState(mark_HIP_time - (recoverT - Recover));
				hipStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����ĵ���Ϣ
				updateHipModel();
			}
			else
			{
				hipStrategy.updateState(0);
				hipStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����ĵ���Ϣ
				updateHipModel();
				
				if (curStrategy == HIP|| curStrategy == AUTO)
				{
					initData();
					ankleStrategy.ReStart(humanData);
					hipStrategy.Restart(humanData);
					isFinished = true;
				}					
			}
			/*�Źؽڻع�վ��*/
		}
	}
	else
	{
		
		HIP_time += humanData.sampleTime;
		if (HIP_time - humanData.TR2 <= 0.1 &&HIP_time - humanData.TR2 >= -0.1)  //ʵʱ�����ݲ����ܳ��ָպõ���TR2����������ֻ����һ�������
			HIP_time = humanData.TR2;

		if (!hipStrategy.isEnd(HIP_time))
		{
			hipStrategy.updateState(HIP_time);
			hipStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����ĵ���Ϣ
			updateHipModel();
			std::cout << "�Źؽڵ���  " << HIP_time << "  " << hipStrategy.xPos << "   " << hipStrategy.xRate << endl;
			if (hipStrategy.markEndFlag == 1)
			{
				tempXPos = hipStrategy.xPos;
				tempXRate = hipStrategy.xRate;
				mark_HIP_time = HIP_time;

			}
		}
		else
		{
			
			angles_ = hskel.getAngles();
			Vector3 v = *hskel.getRootArc()->getTranslation();
			Vector3 p = markRootPos;
			cout << "�ٲ����׹ؽڲ��ԣ�";
			ankleStrategy.IsUseful(tempXPos, tempXRate);
			takeAnkle(angles_, p, tempXPos, tempXRate);
			HIP_time = ANKLE_time + mark_HIP_time;
		}
	}
}
void BalanceControl::takeStep(double xpos, double xrate)
{
	if (STEP_startTime == -1)  //��һ�ν��룬��ʼ���粽���ԵĲ���
	{
		STEP_startTime = 0;
		stepStrategy.startStrategy(xpos, xrate);//��ʼ���ٶȣ�ʱ��

	}
	else if (STEP_startTime == 1)
	{
		if (stepStrategy.BodyPitch != 90)
		{
			cout << "�粽���Իָ�����......" << endl;
			STEP_time = 0;
			recoverT2 += humanData.sampleTime;         //��ʼ����ƽ��ظ���ʱ��
			Recover2 = 0;
			
			double time = (mark_STEP_time - (recoverT2 - Recover2)) / 1;
			if (time >= 0)
			{

				cout << "�ָ�����ʱ��  " << mark_STEP_time - (recoverT2 - Recover2) << endl;
				stepStrategy.startStrategy(xpos, xrate);//��ʼ���ٶȣ�ʱ��
				stepStrategy.updateState(time);
				stepStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����� �׹ؽڵ���Ϣ
				updateStepModel();

				Point p(-humanData.hipwidth / 2, stepStrategy.anklezPos, stepStrategy.anklexPos);
				markAnkleProfile.push_back(p);
				Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
				markCOMProfile.push_back(p1);

			}
			else
			{
				if (Flag4 == 0)
				{
					stepStrategy.updateState(0);
					stepStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����� �׹ؽڵ���Ϣ
					Point p(0, stepStrategy.anklezPos, stepStrategy.anklexPos);
					markAnkleProfile.push_back(p);
					Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
					markCOMProfile.push_back(p1);
					updateStepModel();
					Flag4 = 1;

					if (curStrategy == STEP)
					{
						initData();
						stepStrategy.Restart(humanData);
						isFinished = true;
					}
					else if (curStrategy == AUTO)
					{
						if (isStep == 1)
						{
							initData();
							stepStrategy.Restart(humanData);
							isFinished = true;
						}
					}
				}
			}
		}
	}
	else
	{

		if (!stepStrategy.isEnd())
		{
			double t = timer.getTime();
			if (STEP_startTime == 0)
				STEP_startTime = t;
			STEP_time += humanData.sampleTime;
			if (stepStrategy.isTR2(STEP_time))
				STEP_time = humanData.stepTime;
			stepStrategy.updateState(STEP_time);
			stepStrategy.ModelDataFormat();
			updateStepModel();

			
			Point p(-humanData.hipwidth / 2, stepStrategy.anklezPos, stepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
			markCOMProfile.push_back(p1);

		}
		else
		{
			mark_STEP_time = STEP_time;
			STEP_time=0;
			//STEP_startTime = 1;

		}
	}

}
void BalanceControl::takeMultSteps(double xpos, double xrate)
{

	if (!MstepStrategy.NStep.isCurStep) 
	{  //�Ƿ����µ�һ�����µ�һ���Ŀ�ʼ ��ֵ
		MstepStrategy.startStrategy(xpos, xrate);
		MstepStrategy.NStep.isCurStep = 1; //���µ�һ��
		std::cout << "current move leg  " << MstepStrategy.NStep.leg[(MstepStrategy.NStep.legToTake) % 2] << endl;
	}
	else if (MstepStrategy.NStep.isCurStep == -1)
	{
			MstepStrategy.updateState(0);
			MstepStrategy.ModelDataFormat(0);
			updateMultStepModel();
			MSTEP_time = 0;
	}
	else
	{
		MSTEP_time += humanData.sampleTime;
		if (MstepStrategy.isTR2(MSTEP_time))
			MSTEP_time = humanData.TR2forStep;
		else if (MstepStrategy.isStepTime(MSTEP_time))
			MSTEP_time = humanData.stepTime*MstepStrategy.NStep.curStep;


		//////////////////////////////////////////////////////////////////////////////////////////
		if (!MstepStrategy.isEnd()) 
		{
			MstepStrategy.updateState(MSTEP_time); //curStep�Ѿ�������һ��  //curStep++
			if (MstepStrategy.enterStanceOneTime == 1) 
			{//������ʱ�����µ�֧���ȵİڶ���
				MstepStrategy.initialCOMAngle = MSTEP_hipAngle - MSTEP_kneeAngle;   //������δ���µ�MSTEP_hipAngle��MSTEP_kneeAngle�����µ�֧����(�ϴο粽�İڶ���)�ĽǶ�
				MstepStrategy.NStep.isStanceStraight = 0;
				MstepStrategy.enterStanceOneTime = 0;
			}
			MstepStrategy.ModelDataFormat(MSTEP_time);
			updateMultStepModel();
			//��β
			double pos = (MstepStrategy.NStep.legToTake == 1) ? -humanData.hipwidth / 2 : humanData.hipwidth / 2;
			Point p(pos, MstepStrategy.anklezPos, MstepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, MstepStrategy.zPos, MstepStrategy.xPos);
			markCOMProfile.push_back(p1);

			if (MstepStrategy.NStep.isStepTime && MstepStrategy.NStep.isCurStep)
			{
				double pos = (MstepStrategy.NStep.legToTake == 1) ? -humanData.hipwidth / 2 : humanData.hipwidth / 2;
				Vector3 v(pos, 0, MstepStrategy.NStep.ankleNeedMove);
				MstepStrategy.endOfEachStep(v);
			}
		}
		else
		{
			MstepStrategy.NStep.isCurStep = -1;
			MSTEP_time = 0;
		
		}

	
	}

}



void BalanceControl::takeAnkleAuto(double xpos, double xrate, bool iship)
{
	if (ANKLE_startTime == -1)
	{
		ankleStrategy.startStrategyAuto(xpos, xrate, iship);//��ʼ���ٶ� ʱ�� ��׽��
		ANKLE_startTime = -humanData.sampleTime;
		ANKLE_HipAngles = ankleStrategy.getHipAngles();
		ANKLE_States = ankleStrategy.getStates();
		ANKLE_Offsets = ankleStrategy.getOffset();
		ANKLE_HipAngles_iter = ANKLE_HipAngles.begin();
		ANKLE_States_iter = ANKLE_States.begin();
		ANKLE_Offsets_iter = ANKLE_Offsets.begin();

	}
	else
	{

		if (ANKLE_States_iter != ANKLE_States.end())
		{
			ankleStrategy.xPos = ANKLE_States_iter->second.xpos;
			ankleStrategy.xRate = ANKLE_States_iter->second.xrate;
			ankleStrategy.COMOffset = ANKLE_Offsets_iter->second;
			ankleStrategy.COMAngle = ANKLE_HipAngles_iter->second;
			updateAnkleModel();
			ANKLE_States_iter++;
			ANKLE_Offsets_iter++;
			ANKLE_HipAngles_iter++;

			Point p(0, ankleStrategy.anklezPos, ankleStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, ankleStrategy.zPos, ankleStrategy.xPos);
			markCOMProfile.push_back(p1);

			std::cout << "�׵���  " << "  " << ankleStrategy.xPos << "   " << ankleStrategy.xRate << endl;

		}
		else
		{
			ANKLE_startTime = 1;
		}
	}
}
void BalanceControl::takeStepAuto(double xpos, double xrate)
{
	if (STEP_startTime == -1)  //��һ�ν��룬��ʼ���粽���ԵĲ���
	{

		stepStrategy.startStrategyAuto(xpos, xrate);//��ʼ���ٶȣ�ʱ��
		STEP_time = -humanData.sampleTime;
		STEP_COMStates = stepStrategy.getCOMStates();
		STEP_AnkleStates = stepStrategy.getAnkleStates();
		STEP_HipAngles = stepStrategy.getHipAngles();
		STEP_BodyPitchs = stepStrategy.getBodyPitchs();

		STEP_Offsets = stepStrategy.getOffsets();

		STEP_COMStates_iter = STEP_COMStates.begin();
		STEP_AnkleStates_iter = STEP_AnkleStates.begin();
		STEP_HipAngles_iter = STEP_HipAngles.begin();
		STEP_BodyPitchs_iter = STEP_BodyPitchs.begin();
		STEP_Offsets_iter = STEP_Offsets.begin();
		STEP_startTime = 0;

	}
	else
	{
		if (STEP_COMStates_iter != STEP_COMStates.end())
		{
			stepStrategy.xPos = STEP_COMStates_iter->second.xpos;
			stepStrategy.zPos = STEP_COMStates_iter->second.zpos;
			stepStrategy.xRate = STEP_COMStates_iter->second.xrate;
			stepStrategy.anklexPos = STEP_AnkleStates_iter->second.anklexpos;
			stepStrategy.anklezPos = STEP_AnkleStates_iter->second.anklezpos;
			stepStrategy.COMAngle = STEP_HipAngles_iter->second;
			stepStrategy.BodyPitch = STEP_BodyPitchs_iter->second;
			stepStrategy.COMOffset = STEP_Offsets_iter->second;

			updateStepModel();

			STEP_COMStates_iter++;
			STEP_AnkleStates_iter++;
			STEP_HipAngles_iter++;
			STEP_BodyPitchs_iter++;
			STEP_Offsets_iter++;


			Point p(-humanData.hipwidth / 2, stepStrategy.anklezPos, stepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
			markCOMProfile.push_back(p1);

		}
		else
		{
			STEP_startTime = 1;
		}
	}
}
void BalanceControl::takeMultStepsAuto(double xpos, double xrate)
{

	if (!MstepStrategy.NStep.isCurStep) {  //�Ƿ����µ�һ�����µ�һ���Ŀ�ʼ ��ֵ
		MstepStrategy.startStrategy(xpos, xrate);
		MstepStrategy.NStep.isCurStep = 1; //���µ�һ��
		std::cout << "current move leg  " << MstepStrategy.NStep.leg[(MstepStrategy.NStep.legToTake) % 2] << endl;
	}
	else
	{
		/////////////////////////////////////////////////////////////////////////////////////// time
		double t = timer.getTime();
		if (MSTEP_startTime == -1) // ֻ����һ��
			MSTEP_startTime = t;
		MSTEP_time += humanData.sampleTime;
		// MSTEP_time = (t - MSTEP_startTime)/2;
		if (MstepStrategy.isTR2(MSTEP_time))
		{
			MSTEP_time = humanData.TR2forStep;
			std::cout << "!!!!!!!!!!!!!!!!!!!!!�Ѿ�����һTR2�˹�  " << MstepStrategy.inixPos << "  " << MstepStrategy.inixRate << "  " << MSTEP_time << MstepStrategy.xRate << "  " << MstepStrategy.xPos << endl;
		}

		else if (MstepStrategy.isStepTime(MSTEP_time))
		{
			MSTEP_time = humanData.stepTime*MstepStrategy.NStep.curStep;
			std::cout << "�Ѿ�����һstepTime�˹�  " << MSTEP_time << endl;
		}

		std::cout << "�粽ʱ��  " << MSTEP_time << endl;

		//////////////////////////////////////////////////////////////////////////////////////////
		if (!MstepStrategy.isEnd()) {
			MstepStrategy.updateState(MSTEP_time); //curStep�Ѿ�������һ��  //curStep++
			{
				std::string notmoving = MstepStrategy.NStep.leg[(MstepStrategy.NStep.legToTake + 1) % 2];
				if (MstepStrategy.enterStanceOneTime == 1 && hskel.getArcAngle(notmoving + "Knee") != 0)//������ʱ�����ȵİڶ���
				{
					std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
					std::map<std::string, Vector3> positions;
					hskel.getArcPositions(positions);
					MstepStrategy.initialCOMAngle = -asin((positions[notmoving + "Knee"].getZ() - MstepStrategy.NStep.lastAnkleNeedMove) / humanData.lowerleg) * 180 / pi;
					MstepStrategy.initialkneePos = positions[notmoving + "Knee"].getZ();
					MstepStrategy.NStep.isStanceStraight = 0;
					MstepStrategy.enterStanceOneTime = 0;
					//std::cout << asin((positions[notmoving + "Knee"].getZ() - MstepStrategy.xPos) / humanData.upperleg) * 180 / pi << endl;
					//std::cout << "swingPos��ֵ  " << "  " << positions[notmoving + "Ankle"].getY() << "  " << positions["COM"].getZ() << "  " << positions["COM"].getY() << "  " << positions["COM"].getX() << "  " << positions[notmoving + "Knee"].getZ() << "  " << positions[notmoving + "Knee"].getY() << "  " << positions[notmoving + "HipX"].getZ() << "  " << positions[notmoving + "Knee"].getY() << "  " << MstepStrategy.xPos << "  " << MstepStrategy.NStep.swingLeginitialPos << endl;
					//std::cout << hskel.getArcAngle(notmoving + "Knee") + hskel.getArcAngle(notmoving + "HipX")
					//<< "  " << hskel.getArcAngle(notmoving + "Knee") << "  " << MstepStrategy.initialCOMAngle
					//<< "  " << hskel.getArcAngle(notmoving + "HipX") << "  " <<
					//hskel.getArcAngle(notmoving + "Knee") << "  " << endl;
				}

			}
			MstepStrategy.ModelDataFormat(MSTEP_time);
			updateMultStepModel();


			//��β
			if (MstepStrategy.NStep.isStepTime && MstepStrategy.NStep.isCurStep)
			{
				//��ÿһ��������ʱ���¼һ��ƫ��ֵ
				std::string moving = MstepStrategy.NStep.leg[(MstepStrategy.NStep.legToTake) % 2];
				std::map<std::string, Vector3> positions;
				hskel.getArcPositions(positions);
				MstepStrategy.endOfEachStep(positions[moving + "Ankle"]);
			}




			double pos = (MstepStrategy.NStep.legToTake == 1) ? -humanData.hipwidth / 2 : humanData.hipwidth / 2;
			Point p(pos, MstepStrategy.anklezPos, MstepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, MstepStrategy.zPos, MstepStrategy.xPos);
			markCOMProfile.push_back(p1);



		}
	
	
	}



}

void BalanceControl::RecoverBalance(double xpos, double xrate, Strategy strategyToTake)
{
	tempRootPos = *hskel.getRootArc()->getTranslation();
	switch (strategyToTake) {
	case ANKLE:
		curStrategy = ANKLE;
		ankleStrategy.IsUseful(xpos, xrate);
		takeAnkle(angles_, v1, xpos, xrate);
		break;
	case HIP:
		curStrategy = HIP;
		hipStrategy.IsUseful(xpos, xrate);
		takeHip(xpos, xrate);
		break;
	case STEP:
		curStrategy = STEP;
		if (isStep == 1 || stepStrategy.IsUseful(xpos, xrate))
		{
			isStep = 1;
			takeStep(xpos, xrate);
		}
		else
		{
			if (MSTEP_NStepMarkFlag == 0)//�� ��һ����ֵ
			{
				//�޸ı����
				curStrategy = MSTEP;
				isMultStep = 1;
				MSTEP_NStepMarkFlag = 1;
				//����һ��Nstep��ֵ
				MstepStrategy.NStep.totalStep = MstepStrategy.IsUseful(xpos, xrate);
				MstepStrategy.NStep.curStep = 1;
				MstepStrategy.NStep.xPos = xpos;
				MstepStrategy.NStep.xRate = xrate;
				MstepStrategy.NStep.curCOP = humanData.rf;
				MstepStrategy.NStep.isCurStep = 0;   //��û����һ���أ����� ֻ���ڵ�һ��������ʼ֮ǰ ����Nstep״ֵ̬
				double pos = (MstepStrategy.NStep.legToTake == 1) ? pos = humanData.hipwidth / 2 : -humanData.hipwidth / 2;
				MstepStrategy.NStep.offset = Vector3(pos, 0, 0);
				MSTEP_time = -humanData.sampleTime;
				cout << "��ȡ�˶�粽������Ҫ�ߵĲ���Ϊ:  " << MstepStrategy.NStep.totalStep << endl;
			}

			if (MstepStrategy.NStep.curStep <= MstepStrategy.NStep.totalStep)   //������ȡ�ಽ
				takeMultSteps(MstepStrategy.NStep.xPos, MstepStrategy.NStep.xRate);
			else
				takeStep(MstepStrategy.NStep.xPos, MstepStrategy.NStep.xRate);

		}
		break;
	case AUTO:
		curStrategy = AUTO;
		if (isAngle == 1 || ankleStrategy.IsUseful(xpos, xrate))
		{
			Auto_Strategy = ANKLE;
			isAngle = 1;   //��ȥÿ�ζ��õ���IsUseful()���˷ѷ���ʱ��
			takeAnkle(angles_,v1,xpos, xrate);
			
		}
		else if (isHip == 1 || hipStrategy.IsUseful(xpos, xrate))
		{
			Auto_Strategy = HIP;
			isHip = 1;
			takeHip(xpos, xrate);
			
		}
		else if (isStep == 1 || stepStrategy.IsUseful(xpos, xrate))
		{
			Auto_Strategy = STEP;
			isStep = 1;
			takeStep(xpos, xrate);
		}
		else if (isMultStep == 1 || MSTEP_NStepMarkFlag == 0 || MstepStrategy.NStep.totalStep != -1)
		{
			Auto_Strategy = MSTEP;
			if (MSTEP_NStepMarkFlag == 0)//�� ��һ����ֵ
			{

				//�޸ı����
				isMultStep = 1;
				MSTEP_NStepMarkFlag = 1;
				//����һ��Nstep��ֵ
				MstepStrategy.NStep.totalStep = MstepStrategy.IsUseful(xpos, xrate);
				MstepStrategy.NStep.curStep = 1;
				MstepStrategy.NStep.xPos = xpos;
				MstepStrategy.NStep.xRate = xrate;
				MstepStrategy.NStep.curCOP = humanData.rf;
				MstepStrategy.NStep.isCurStep = 0;   //��û����һ���أ����� ֻ���ڵ�һ��������ʼ֮ǰ ����Nstep״ֵ̬
				double pos = (MstepStrategy.NStep.legToTake == 1) ? pos = humanData.hipwidth / 2 : -humanData.hipwidth / 2;
				MstepStrategy.NStep.offset = Vector3(pos, 0, 0);
				MSTEP_time = -humanData.sampleTime;
				//cout << "��ȡ�˶�粽������Ҫ�ߵĲ���Ϊ:  " << MstepStrategy.NStep.totalStep << endl;
			}

			if (MstepStrategy.NStep.curStep <= MstepStrategy.NStep.totalStep)   //������ȡ�ಽ
				takeMultSteps(MstepStrategy.NStep.xPos, MstepStrategy.NStep.xRate);

		}
		break;
	default:
		break;
	}


}

void BalanceControl::getxPosAndxRate(double &xPos, double &xRate, double &time)
{
	if (curStrategy == ANKLE)
	{
		if (ANKLE_time != 0)
		{
			xPos = ankleStrategy.xPos;
			xRate = ankleStrategy.xRate;
			time = ANKLE_time;
		}
	}
	else if (curStrategy == HIP)
	{
		if (mark_HIP_time != 0 && HIP_time != mark_HIP_time)
		{
			xPos = ankleStrategy.xPos;
			xRate = ankleStrategy.xRate;
			time = HIP_time;
		}
		else if (HIP_time != 0)
		{
			xPos = hipStrategy.xPos;
			xRate = hipStrategy.xRate;
			time = HIP_time;
		}

	}

	else if (curStrategy == STEP)
	{
		if (STEP_time != 0)
		{
			xPos = stepStrategy.xPos;
			xRate = stepStrategy.xRate;
			time = STEP_time;
		}
	}

	else if (curStrategy == AUTO)
	{

		if (Auto_Strategy == ANKLE)
		{
			if (ANKLE_time != 0)
			{
				xPos = ankleStrategy.xPos;
				xRate = ankleStrategy.xRate;
				time = ANKLE_time;
			}
		}
		else if (Auto_Strategy == HIP)
		{
			if (mark_HIP_time != 0 && HIP_time != mark_HIP_time)
			{
				xPos = ankleStrategy.xPos;
				xRate = ankleStrategy.xRate;
				time = HIP_time;
			}
			else if (HIP_time != 0)
			{
				xPos = hipStrategy.xPos;
				xRate = hipStrategy.xRate;
				time = HIP_time;
			}

		}

		else if (Auto_Strategy == STEP)
		{
			if (STEP_time != 0)
			{
				xPos = stepStrategy.xPos;
				xRate = stepStrategy.xRate;
				time = STEP_time;
			}
		}

		else if (Auto_Strategy == MSTEP)
		{
			if (MSTEP_time != 0)
			{
				xPos = MstepStrategy.xPos;
				xRate = MstepStrategy.xRate;
				time = MSTEP_time;
			}

		}
	}
}
void BalanceControl::initControl()
{
	hskel.create();//����ÿ���Ǽܵ�ƽ�� ���� ��ת�Ƕ� �� ����  �Լ��ӹؽ�//����ÿ���ؽڵ����ɶ� �Լ������Ǽ�		   
    hskin.create();
	hskin.update(&hskel);
	bodypos.set(0, humanData.legLen,0.0);
	humanFramer.setRate(1);
	humanAnimator.addActor(&hskel, &humanFramer);
	(hskel.getRootArc())->setTranslation(bodypos);//�Ǽܵ�root����xyz��ƽ��
	
}

void BalanceControl::initData()
{
	isAnkleProfileEnd = false;
	isCOMProfileEnd = false;
	currentDataTime = -1;
	StartTime = -1;
	AnkleStartTime = -1;
	NStepMarkFlag = 0;
	animating = false;
	skinning = true;
	wireframe = false;
	action = true;
	yacobi = false;
	walking = true;

	currentChooseAngle1 = 0.0;
	currentChooseAngle2 = 0.0;
	currentChooseAct = 0;
	ToRecoverBalance = false;

	Flag4 = 0;


	Recover = 0;  
	Recover2 = 0;
	recoverT = 0;
	recoverT2 = 0;
	//���Ա�־
	isAngle = 0;
	isHip = 0;
	isStep = 0;
	isMultStep = 0;

	ANKLE_startTime = -1;
	ANKLE_time = -humanData.sampleTime;
	mark_ANKLE_time = 0;

	HIP_startTime = -1;
	HIP_time = -humanData.sampleTime;
	mark_HIP_time = 0;

	STEP_startTime = -1;
	STEP_time = -humanData.sampleTime;
	STEP_dis = 0;			//�ڶ���hip��ankle�ľ���
	STEP_hipAngle = 0;		//�ڶ��ȵ��Źؽ�ת��
	STEP_kneeAngle = 0;	//�ڶ��ȵ�ϥ�ؽ�ת��
	mark_STEP_time = 0;

	MSTEP_startTime = -1;
	MSTEP_time = 0;
	MSTEP_dis = 0;			//�ڶ���hip��ankle�ľ���
	MSTEP_hipAngle = 0;		//�ڶ��ȵ��Źؽ�ת��
	MSTEP_kneeAngle = 0;	//�ڶ��ȵ�ϥ�ؽ�ת��
	MSTEP_kneeAngle_flag = 0;
	MSTEP_NStepMarkFlag = 0;

	angles_ = hskel.getAngles();
}
void BalanceControl::Reset()
{	
	if(curStrategy==ANKLE||curStrategy==HIP)
		ANKLE_startTime = 1;
	else if (curStrategy == STEP)
	{
		if (isStep == 1)
		{
			STEP_startTime = 1;
		}
		else
		{
			initControl();
			initData();
			ankleStrategy.ReStart(humanData);
			hipStrategy.Restart(humanData);
			stepStrategy.Restart(humanData);
			MstepStrategy.Restart(humanData);
			isFinished = true;
		}
	}
	else
	{
		if (isAngle == 1)
		{
			ANKLE_startTime = 1;
		}	
		else if (isHip == 1)
		{
			ANKLE_startTime = 1;
		}
		else if (isStep == 1)
		{
			STEP_startTime = 1;
		}
		else
		{
			initControl();
			initData();
			ankleStrategy.ReStart(humanData);
			hipStrategy.Restart(humanData);
			stepStrategy.Restart(humanData);
			MstepStrategy.Restart(humanData);
			isFinished = true;
		}
	}
}
