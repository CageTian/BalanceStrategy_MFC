#ifndef MULTSTEPSTRATEGY_H
#define  MULTSTEPSTRATEGY_H


#include "HumanSimulation.h"
#include "vector3.h"
#include <stdlib.h>
#include <stdio.h>
#include <map> 
#include<iostream>

using namespace std;
struct NStepData
{
	string leg[2] = { "L","R" };         //�����жϵ�ǰ�ڶ�������ֻ��(���� or ����)
	int  legToTake = 0;					 //�˴���Ϊ0 Ĭ���ȳ�����
	double xPos = 0;					 //��ǰ�粽��λ��
	double xRate = 0;					 //��ǰ�粽���ٶ�
	double curStep = 0;					 //��ǰ�粽��
	double totalStep = 0;				 //�����Ե��ܲ���
	double xCapture = 0;		    	 //��ǰ�粽�Ĳ�׽��
	double curCOP = 0;					 //��ǰ�粽��ѹ������
	Vector3 offset;					     //��ǰ�粽����ƫ�ƣ�ȷ��֧����ʼ��λ�ڵ��治�� 

										 //�滮�׹ؽڹ켣
	double ankleNeedMove = 0;			 //��ǰ�粽���׹ؽ���Ҫ�ƶ��ľ���
	double lastAnkleNeedMove = 0;		 //�ϴο粽���׹ؽ���Ҫ�ƶ��ľ���
	double swingLeginitialPos = 0;		 //���ڹ滮�׹ؽڹ켣���ڶ����׹ؽڵĳ�ʼλ�ã����������ü�����ʼʱ�̰ڶ����׹ؽڵĳ�ʼλ��

	bool isCurStep = 0;					 //�ж��Ƿ��ǵ�ǰһ��  ��������֮�������ȷ�ν�
	bool isStanceStraight = 1;			 //֧�����Ƿ���ֱ��״̬ ��������֮�������ȷ�ν�
	bool isTR2 = 0;					     //�Ƿ��Ѿ���TR2ʱ�̣����������ý���ʱ��
	bool isStepTime = 0;			     //�Ƿ��Ѿ����粽����ʱ��

};
class MultStepStrategy
{
public:
	HumanData humanData;    //Ψһһ�ݾͺ�
	NStepData NStep; //��ǰ�粽���������

					 //��������
	double inixPos, inixRate, xPos, zPos, xRate, zRate;
	//�׹ؽ�����
	double anklexPos, anklexRate, anklezPos, anklezRate;
	//�����������ݵ���ģ��ת�Ǻ�ƫ��
	double BodyPitch = 0;
	double HipAngle = 0;
	double KneeAngle = 0;
	double MSTEP_time = -humanData.sampleTime;
	// ��������֮�������ȷ�ν�
	double initialCOMAngle = 0;
	double initialkneePos = 0;
	Offset COMOffset;
	//��Ҫ�ؼ���
	double EnterTR2OneTime = 1;          //��һ�������У�ֻ�ж�����ǰ�Ƿ�ΪTR2ʱ�̡�һ��
	double enterStanceOneTime = 0;       //ÿ���粽�����У�ֻ�жϡ�֧�����Ƿ�Ϊֱ��״̬��һ��
	double EnterStepTimeOneTime = 1;      //ÿ���粽�����У�ֻ�жϡ���ǰ�Ƿ�ΪstepTimeʱ�̡�һ��



	MultStepStrategy(HumanData h)
	{
		humanData = h;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
	}
	//��Ҫ���ܺ���
	/////////////////////////////////////////////////////////////////////////////////////////
	int IsUseful(double xpos, double xrate);			//�ж��Ƿ�ɵ��øò��Իָ�ƽ�ⷵ����Ҫ�缸��
	void startStrategy(double xpos, double xrate);		//��ֵ ���ڿ�ʼ
	void setPosAndRate(double xpos, double xrate);		//������������
	void computexCapture();								//���Կ�ʼ����  ���㲶׽��
	void updateStateAnkle(double t);					//�����׹ؽ�״̬
	void updateState(double t);							//��������״̬��Ϊ������㣬��Ϊ������𣺵�һ�����м䲽�����һ��
	void updateFirstCOMState(double t);					//���µ�һ��
	void updateMidCOMState(double t);					//�����м䲽
	void updateLastCOMState(double t);					//�������һ��
	void ModelDataFormat(double t);						//������״̬ת��Ϊģ�͵�ת���ǶȺ�ƫ��
	void endOfEachStep(Vector3 offset);					//��stepTimeʱ���л��������˺�������Ϊ�µ�һ����NStep״ֵ̬
	bool isEnd();										//�ж�ģ���Ƿ�ƽ�⣬�����Ƿ����
	bool isTR2(double t);								//�ж��Ƿ񵽴�TR2ʱ�̣����������ý���ʱ��
	bool isStepTime(double t);							//�ж��Ƿ��Ƿ񵽴�stepTimeʱ�̣�ָʾ��ǰ�粽�Ƿ��ѽ���
														/////////////////////////////////////////////////////////////////////////////////////////

	void Restart(HumanData h)
	{
		BodyPitch = 0;
		HipAngle = 0;
		KneeAngle = 0;
		MSTEP_time = -humanData.sampleTime;
		// ��������֮�������ȷ�ν�
		initialCOMAngle = 0;
		initialkneePos = 0;
		COMOffset;
		//��Ҫ�ؼ���
		EnterTR2OneTime = 1;          //��һ�������У�ֻ�ж�����ǰ�Ƿ�ΪTR2ʱ�̡�һ��
		enterStanceOneTime = 0;       //ÿ���粽�����У�ֻ�жϡ�֧�����Ƿ�Ϊֱ��״̬��һ��
		EnterStepTimeOneTime = 1;      //ÿ���粽�����У�ֻ�жϡ���ǰ�Ƿ�ΪstepTimeʱ�̡�һ��

		NStep.legToTake = 0;					 //�˴���Ϊ0 Ĭ���ȳ�����
		NStep.xPos = 0;					 //��ǰ�粽��λ��
		NStep.xRate = 0;					 //��ǰ�粽���ٶ�
		NStep.curStep = 0;					 //��ǰ�粽��
		NStep.totalStep = 0;				 //�����Ե��ܲ���
		NStep.xCapture = 0;		    	 //��ǰ�粽�Ĳ�׽��
		NStep.curCOP = 0;					 //��ǰ�粽��ѹ������

											 //�滮�׹ؽڹ켣
		NStep.ankleNeedMove = 0;			 //��ǰ�粽���׹ؽ���Ҫ�ƶ��ľ���
		NStep.lastAnkleNeedMove = 0;		 //�ϴο粽���׹ؽ���Ҫ�ƶ��ľ���
		NStep.swingLeginitialPos = 0;		 //���ڹ滮�׹ؽڹ켣���ڶ����׹ؽڵĳ�ʼλ�ã����������ü�����ʼʱ�̰ڶ����׹ؽڵĳ�ʼλ��

		NStep.isCurStep = 0;					 //�ж��Ƿ��ǵ�ǰһ��  ��������֮�������ȷ�ν�
		NStep.isStanceStraight = 1;			 //֧�����Ƿ���ֱ��״̬ ��������֮�������ȷ�ν�
		NStep.isTR2 = 0;					     //�Ƿ��Ѿ���TR2ʱ�̣����������ý���ʱ��
		NStep.isStepTime = 0;			     //�Ƿ��Ѿ����粽����ʱ��


		humanData = h;
		xPos = 0;           //�Ǽ����ĵ�λ��
		zPos = h.l;         //���ĸ߶�
		xRate = 0;          //���ĵ��ٶ�
		zRate = 0;
		anklexPos = 0;
		anklezPos = 0;
		anklexRate = 0;
		anklezRate = 0;
	}
	void takeMultSteps();
	void ModelData();				//������

									//get �� set����
	double getxPos() { return xPos; }
	double getxRate() { return xRate; }
	double getzPos() { return zPos; }
	double getzRate() { return zRate; }
	double getanklexPos() { return  anklexPos; }
	double getanklexRate() { return anklexRate; }
	double getanklezPos() { return  anklezPos; }
	double getanklezRate() { return anklezRate; }
	double getHipAngle() { return HipAngle; }
	double getBodyPitch() { return BodyPitch; }

};
#endif