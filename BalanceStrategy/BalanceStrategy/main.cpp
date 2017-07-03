#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gl/glut.h>
#include<vector>
#include "timer.h"
#include "basicterrain.h"
#include "skeleton.h"
#include "fkframer.h"
#include "ikframer.h"
#include "humanskeleton.h"
#include "humanwalkanimation.h"
#include "humanwalkframer.h"
#include "animation.h"
#include "humanskin.h"
#include"names.h"
#include"actionFramer.h"
#include"actionAnimation.h"
#include "StepStrategy.h"
#include "HipStrategy.h"
#include "AnkleStrategy.h"
#include<iostream>
using namespace std;


#define DEG_TO_RAD 0.0174532925

							 // Distance of camera from actor
#define CAMERA_DISTANCE 10.0

							 // Number of stairs to walk up
#define NUM_STAIRS 4

							 // Initial y translation for the human skeleton, to place bottom of feet at y = 0.0
#define INITIAL_ELEVATION 1.98

							 // Initial z translation for the human skeleton
#define INITIAL_POSITION -2.6

							 // Actor velocity - rate of motion along z axis as units per second; can be adjusted with rate variable from keyboard (r/R keys)
#define ACTOR_VELOCITY 0.40

							 // Translation increment when translating x, y, z from keyboard input
#define TRANSLATION_INCREMENT 0.25

							 // Translation increment when translating x, y, z from keyboard input
#define ROTATION_INCREMENT 0.5

							 // Initial window size, this is used when window is created and opened
GLsizei wh = 600, ww = 800;

// Translations and rotations for the scene
GLdouble x = 0;
GLdouble y = 0.0;
GLdouble z = 12.25;
GLdouble xrot = 23.0;// 2.5;
GLdouble yrot = -45.0;// 251.5;
GLdouble zrot = 0.0;


bool  porduceData = TRUE;
bool teststep = false;
bool ToRecoverBalance = false;


HumanData humanData;                      //�洢��������
HumanSkeleton hskel(humanData);
StepStrategy  stepStrategy(humanData);   //�粽����
HipStrategy hipStrategy(humanData);    //�Źؽڲ���
AnkleStrategy ankleStrategy(humanData); //�׹ؽڲ���
actionFramer  stepFramer;
actionAnimation *stepAnimation = NULL;
std::vector<Point>markAnkleProfile;
std::vector<Point>markCOMProfile;
bool isAnkleProfileEnd = false;
bool isCOMProfileEnd = false;
bool markRootPosFlag = true;
Vector3 markRootPos;
std::map<double, Point> COM;
std::map<double, Point> Ankle;
std::map<double, double> COMData;
std::map<double, Offset> offset;
std::map<string, double>firstFramercpy;
bool firstFramercpyFlag=true;
double currentDataTime = -1;
double StartTime = -1;
double AnkleStartTime = -1;
double tempXPos; /*= hipStrategy.getxPos();*/
double tempXRate; /*= hipStrategy.getxRate();*/
//double sampleTime = 0.005;
double flag = -humanData.sampleTime, flag1 = -humanData.sampleTime;
double NStepMarkFlag = 0;
Vector3 v1, v2;

// Animation rate adjustment
double rate = 1.0;

// Timer for calculating velocities
Timer timer;

// Time of last update
double lastTime = 0.0;

// Scene controls
bool animating = false;
bool skinning = true;
bool wireframe = false;
bool action = true;
bool yacobi = false;

// Position of light in the scene
GLfloat LightPosition[] = { 15.0f, 25.0f, 15.0f, 1.0f };

// The ground on which to walk
BasicTerrain ground;

// Forward kinematic animation with Inverse kinematic foot correction for walking over the ground
HumanWalkAnimation walkanimator(&ground);

// Forward kinematic animation for walking up stairs; switches between skeletons with root at body and feet

actionAnimation *tempanimation = NULL;
actionAnimation * yacobianimation = NULL;
actionFramer *jacobiframer;
actionFramer *forTest;
// The skeleton


// The skin attached to the skeleton
HumanSkin hskin(humanData);

// The forward kinematic animation for walking
HumanWalkFramer walkframer;

// The forward kinematic for walking up stairs


// Select between walking up stairs and general walking
bool walking = true;
char *currentChooseSrc = new char[11];
GLdouble currentChooseAngle1 = 0.0;
GLdouble currentChooseAngle2 = 0.0;
char currentChooseAct = 0;
Vector3  currentChooseTranslate(0.0,0.0,0.0);
char * currentChooseIK = new char[11];
char markSrc[21][11] = { ROOT,NECKBASE,HEADBASE,RSHOULDERX,RSHOULDERY,
						RSHOULDERZ,RELBOW,LSHOULDERX,LSHOULDERY,LSHOULDERZ,
						LELBOW,RHIPX,RHIPY,RHIPZ,RKNEE,
						RANKLE,LHIPX,LHIPY,LHIPZ,LKNEE,
						LANKLE
};
// Position of actor for tracking with camera
Vector3 bodypos;
int wxflag= 1;

//��ǳ�ÿ���ؽڵ�λ�� ע�⣺body�ؽڲ�����
void drawSrcs()
{
	if (wxflag == 1)
	{
		std::map<std::string, Vector3> positions;
		hskel.getArcPositions(positions);
		std::map<std::string, Vector3>::iterator it;
		for (it = positions.begin(); it != positions.end(); ++it)
		{
		//	std::cout << "wx   " << it->first << it->second.getX() << "   " << it->second.getY() << "   " << it->second.getZ() << "  " << endl;
			glColor3f(1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef((it->second).getX(), (it->second).getY(), (it->second).getZ());
			glutWireSphere(0.025, 100, 100);
			glPopMatrix();
		}
		//wxflag++;
	}

}
void drawXYZ() 
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3d(0 + 1, 0,0);
	glVertex3d(0 - 1, 0, 0);
	glVertex3d(0 + 1, 0, 0);
	glVertex3d(0 + 0.95,0, 0 + 0.05);
	glVertex3d(0 + 1, 0,0);
	glVertex3d(0 + 0.95,0,0 - 0.05);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0, 0 - 1, 0);
	glVertex3d(0, 0 + 1.15, 0);
	glVertex3d(0, 0+ 1.15, 0);
	glVertex3d(0 - 0.05,0+ 1.1,0);
	glVertex3d(0, 0 + 1.15,0);
	glVertex3d(0 + 0.05,0 + 1.1,0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0, 0, 0 - 1);
	glVertex3d(0, 0,0 + 1);
	glVertex3d(0, 0,0 + 1);
	glVertex3d(0 + 0.05, 0, 0+ 0.95);
	glVertex3d(0, 0, 0+ 1);
	glVertex3d(0 - 0.05, 0, 0 + 0.95);
	glEnd();

}

// Save matrix state and do the rotation
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xrot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xrot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yrot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yrot += 5.0f;

	if (key > 356.0f)
		xrot = 0.0f;

	if (key < -1.0f)
		xrot = 355.0f;

	if (key > 356.0f)
		yrot = 0.0f;

	if (key < -1.0f)
		yrot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}
/*
void updateStepModel()
{
	
	
	Point tempCOM(0, stepStrategy.zPos, stepStrategy.xPos);
	Point tempAnkle(0, stepStrategy.anklezPos, stepStrategy.anklexPos);
	double dis = sqrt(pow((tempCOM.px - tempAnkle.px), 2) + pow((tempCOM.py - tempAnkle.py), 2) + pow((tempCOM.pz - tempAnkle.pz), 2));
	double hipAngle = acos((pow(humanData.upperleg, 2) + pow(dis, 2) - pow(humanData.lowerleg, 2)) / (2 * humanData.upperleg*dis)) * 180 / pi;
	double kneeAngle = 180 - acos((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg)) * 180 / pi;
	double temp = asin((tempCOM.pz - tempAnkle.pz) / dis) * 180 / pi;
	hipAngle -= temp;
	hskel.setArcAngle("Body", stepStrategy.BodyPitch);
	//hskel.setArcAngle("Body", stepStrategy.COMAngle);
	hskel.setArcAngle("LHipX", -stepStrategy.COMAngle);
	hskel.setArcAngle("LAnkle", stepStrategy.COMAngle);//Զ���ҷ��������
	hskel.setArcAngle("RAnkle", stepStrategy.COMAngle);
	hskel.setArcAngle("RHipX", hipAngle);
	hskel.setArcAngle("RKnee", -kneeAngle);
	hskel.getRootArc()->setTranslation(markRootPos.getX(), markRootPos.getY() + stepStrategy.COMOffset.pz, markRootPos.getZ() + stepStrategy.COMOffset.px);
    hskin.update(&hskel);
	cout << "  " << kneeAngle << "  " << hipAngle << endl;
}

void takeStep(std::map<std::string,double> angles_,double xpos,double xrate)
{
	
	if (markRootPosFlag)  //��ȡ���ڵ��λ��
	{
		markRootPos = *hskel.getRootArc()->getTranslation();

		markRootPosFlag = false;
	}
	if (StartTime == -1)  //��һ�ν��룬��ʼ���粽���ԵĲ���
	{
		hskel.setAngles(angles_,xpos);
		stepStrategy.startStrategy(xpos,xrate);//��ʼ���ٶȣ�ʱ��
		StartTime = 0;
	}
	else
	{		
		double t;		
		if (!stepStrategy.isEnd())
		{
			t=timer.getTime();
			if (StartTime == 0)
				StartTime = t;
			
			stepStrategy.updateState((t-StartTime) );
			stepStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����� �׹ؽڵ���Ϣ
		
			Point p(0, stepStrategy.anklezPos, stepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
			markCOMProfile.push_back(p1);
			updateStepModel();
		}
		else
		{
			//StartTime = -1;
		}
	}
}
*/

void updateStepModel()
{
	std::string moving = stepStrategy.NStep.leg[stepStrategy.NStep.legToTake];
	std::string notmoving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake + 1) % 2];
	std::map<std::string, Vector3> positions;
	hskel.getArcPositions(positions);
	double kneeX = positions[notmoving + "Knee"].getZ();
	Vector3 ha = positions[moving + "Ankle"];//��ȡ���׵�����
	Vector3 ha1 = positions[notmoving + "Ankle"];//��ȡ���׵�����
	//cout << "��ʼǰmoving " << "  " << moving << "  " << stepStrategy.anklexPos << "  " << ha.getX() << "  " << ha.getY() << "  " << ha.getZ() << endl;
	//cout << "��ʼǰnotmoving " << "  " << notmoving << "  " << stepStrategy.anklexPos << "  " << ha1.getX() << "  " << ha1.getY() << "  " << ha1.getZ() << endl;


	double l1 = humanData.lowerleg;
	double x1 = humanData.upperleg, x2 = humanData.l - humanData.legLen, x3 = 0;
	x3 = sqrt(pow(x1, 2) + pow(x2, 2) - 2 * x1*x2*cos((180 - stepStrategy.initialBodyPitch)*pi / 180));
	double a3 = acos((pow(x1, 2) + pow(x3, 2) - pow(x2, 2)) / (2 * x1*x3));
	double a4 = asin((kneeX - stepStrategy.xPos) / x3);
	double lala = hskel.getArcAngle(notmoving + "Knee") + (a3 + a4) * 180 / pi;
	
	hskel.setArcAngle("Body", stepStrategy.BodyPitch);
	hskel.setArcAngle(notmoving+"HipX" , -stepStrategy.COMAngle);
	if(stepStrategy.NStep.isStanceStraight==0)
	hskel.setArcAngle(notmoving + "Knee", stepStrategy.kneeAngle);
	hskel.setArcAngle(notmoving+"Ankle", stepStrategy.COMAngle-stepStrategy.kneeAngle);
	hskin.update(&hskel);
	//cout << "kalaa����ɶ ������  "<<stepStrategy.BodyPitch << "  " << stepStrategy.COMAngle << stepStrategy.kneeAngle<< endl;
	
	
	/////////////////////////////////////////////////////////////////////�޸�ƫ����
	hskel.getArcPositions(positions); 
	Vector3 v = positions[notmoving + "Ankle"];//��ȡ���׵�����
	Vector3 root = *(hskel.getRootArc()->getTranslation());
	//cout << "����Ϊɶƫ�Ʋ�������" << notmoving << "  " << root.getX() << "  " << root.getY() << "  " << root.getZ() << endl;
	v -= stepStrategy.NStep.offset;
	//cout << "����Ϊɶƫ�Ʋ�������" << notmoving << "  " << v.getX() << "  " << v.getY() << "  " << v.getZ() << endl;
	//cout << "����Ϊɶƫ�Ʋ�������" << notmoving << "  " << stepStrategy.NStep.offset.getX() << "  " << stepStrategy.NStep.offset.getY() << "  " << stepStrategy.NStep.offset.getZ() << endl;
	hskel.getRootArc()->setTranslation(root.getX() - v.getX(), root.getY() - v.getY(), root.getZ() - v.getZ());
	hskin.update(&hskel);

	//cout << notmoving << "  " << hskel.getArcAngle(notmoving + "HipX") << "  " << hskel.getArcAngle("Body") << "  " << stepStrategy.NStep.offset.getZ() << "  " << hskel.getArcAngle(notmoving + "Knee") << endl;
	
	hskel.getArcPositions(positions);
	Vector3 vvaa = positions[moving + "Ankle"];//��ȡ���׵�����
	Vector3 va3 = positions[notmoving + "Ankle"];//��ȡ���׵�����
	Vector3 v4 = positions["Root"];//��ȡ���׵�����
//	cout << "���ں��վ��Ankle  " << va3.getX() << "  " << va3.getY() << "  " << va3.getZ() << endl;;
	
	cout <<"���ں��root  "<<stepStrategy.xRate<<"  "<< stepStrategy.xPos << "  "<< v4.getZ() << "  " << v4.getY() << endl;;
	//cout << "moving " << "  " <<moving << "  " << vvaa.getX() << "  " << vvaa.getY() << "  " << vvaa.getZ() << endl;
	//cout << "notmoving " << "  " << notmoving << "  " << stepStrategy.anklexPos << "  " << va3.getX() << "  " << va3.getY() << "  " << va3.getZ() << endl;


	//////////////////////////////////////////////////////////////���°ڶ���״̬
	Point tempCOM(0, stepStrategy.zPos, stepStrategy.xPos);
	Point tempAnkle(0, stepStrategy.anklezPos, stepStrategy.anklexPos);
	cout << tempCOM.py << "  " << tempCOM.pz << endl;
	cout << tempAnkle.py << "  " << tempAnkle.pz << endl;
	double dis = sqrt(pow((tempCOM.px - tempAnkle.px), 2) + pow((tempCOM.py - tempAnkle.py), 2) + pow((tempCOM.pz - tempAnkle.pz), 2));
	double hipAngle = 0;
	if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) <1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) > -1)
	hipAngle = acos((pow(humanData.upperleg, 2) + pow(dis, 2) - pow(humanData.lowerleg, 2)) / (2 * humanData.upperleg*dis)) * 180 / pi;
	double  kneeAngle = 15;
	if ((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) <= 1 && (pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg) >= -1)
	kneeAngle = 180 - acos((pow(humanData.upperleg, 2) + pow(humanData.lowerleg, 2) - pow(dis, 2)) / (2 * humanData.upperleg*humanData.lowerleg)) * 180 / pi;
	double temp = asin((tempCOM.pz - tempAnkle.pz) / dis) * 180 / pi;
	hipAngle -= temp;
	hipAngle > -25 ? hipAngle :-5 ;//����һ�·���ת��̫��
	cout << " �Ƕ�  " << temp << "  " << hipAngle << "  " << kneeAngle << endl;

	
	hskel.setArcAngle(moving + "HipX", hipAngle);
	hskel.setArcAngle(moving + "Knee", -kneeAngle);
	hskin.update(&hskel);
	hskel.getArcPositions(positions);
	cout<<positions[moving+"Ankle"].getY()<<"  "<<  positions[moving + "Ankle"].getZ()- stepStrategy.NStep.ankleNeedMove << "  "<<positions[moving + "Ankle"].getZ() <<"  "<< stepStrategy.NStep.ankleNeedMove << endl;
	if (positions[moving + "Ankle"].getY()<0.01)
	{
		hskel.setArcAngle(moving + "Ankle", -hipAngle + kneeAngle);
		//cout << "�޸�Ankle����������������";
	}
	
	
	hskin.update(&hskel);
	//cout << " ע���� " << hipAngle << "  " << kneeAngle << endl;
	hskel.getArcPositions(positions);
	Vector3 vaa = positions[moving + "Ankle"];//��ȡ���׵�����
	Vector3 va = positions[notmoving + "Ankle"];//��ȡ���׵�����
	cout << "����Ϊɶƫ�Ʋ�������" << "  " << stepStrategy.NStep.ankleNeedMove << "  " << stepStrategy.anklexPos << "  " << vaa.getX() << "  " << vaa.getY() << "  " << vaa.getZ() << endl;
	cout << hskel.getArcAngle(moving + "HipX") << "  " << hskel.getArcAngle(moving + "Knee") << endl;
	

}


void takeStep( double xpos, double xrate)
{

	if (!stepStrategy.NStep.isCurStep) {  //�Ƿ����µ�һ�����µ�һ���Ŀ�ʼ ��ֵ
		stepStrategy.startStrategy(xpos, xrate);
		stepStrategy.NStep.legToTake = (stepStrategy.NStep.legToTake + 1) % 2;//�ȳ�����
		stepStrategy.NStep.isTR2 = 0;//Ŀǰ��û�е�TR2ʱ��
		stepStrategy.NStep.isCurStep = 1; //���µ�һ��
		stepStrategy.EnterTR2OneTime = 1;
		stepStrategy.enterStanceOneTime =1;
		std::string notmoving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake + 1) % 2];
		std::string moving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake) % 2];
		std::map<std::string, Vector3> positions;
		hskel.getArcPositions(positions);
		stepStrategy.NStep.offset = positions[notmoving+"Ankle"];
		//cout << stepStrategy.NStep.offset.getX() << "  " << stepStrategy.NStep.offset.getY() << "  " << stepStrategy.NStep.offset.getZ() << endl;
		//cout << positions[moving + "Ankle"].getX() << "  " << positions[moving + "Ankle"].getY() << "  " << positions[moving + "Ankle"].getZ() << endl;
		//cout << positions["Root"].getX() << "  " << positions["Root"].getY() << "  " << positions["Root"].getZ() << endl;

		std::cout << "current move leg  " << stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake) % 2] << endl;

	}
	else 
	{
		/////////////////////////////////////////////////////////////////////////////////////// time
		double t = timer.getTime();
		if (StartTime == -1) // ֻ����һ��
			StartTime = t;
		flag += humanData.sampleTime;
		//flag = t - StartTime;
		if (stepStrategy.isTR2(flag))
		flag = humanData.TR2forStep*stepStrategy.NStep.curStep;
		
		std::cout << "�粽ʱ��  " << flag << "  " << flag1 << endl;

		//////////////////////////////////////////////////////////////////////////////////////////
		if (!stepStrategy.isEnd()) {
			stepStrategy.updateStateAnkle(flag);
			stepStrategy.updateState(flag); //curStep�Ѿ�������һ��  //curStep++
			{
			std::string moving = stepStrategy.NStep.leg[stepStrategy.NStep.legToTake];
			std::string notmoving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake + 1) % 2];
			//cout << "�ж�վ����״̬��  " << stepStrategy.enterStanceOneTime <<"  "<<hskel.getArcAngle(notmoving + "Knee") << endl;
			if (stepStrategy.enterStanceOneTime == 1 && hskel.getArcAngle(notmoving + "Knee") != 0)//������ʱ�����ȵİڶ���
				{
					std::map<std::string, Vector3> positions;
					hskel.getArcPositions(positions);
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
					stepStrategy.NStep.isStanceStraight = 0;
					stepStrategy.enterStanceOneTime = 0;
					stepStrategy.initialCOMAngle = -asin((positions[notmoving + "Knee"].getZ() - stepStrategy.NStep.lastAnkleNeedMove) / humanData.lowerleg) * 180 / pi;
					stepStrategy.NStep.swingLeginitialPos=positions[moving+"Ankle"].getZ();
					stepStrategy.kneeAngle = hskel.getArcAngle(notmoving + "Knee");
				    stepStrategy.HipzPos = positions[notmoving + "HipX"];
					stepStrategy.initialBodyPitch = hskel.getArcAngle(notmoving + "HipX");
				//	cout << asin((positions[notmoving + "Knee"].getZ() - stepStrategy.xPos) / humanData.upperleg)*180/pi << endl;
				//	cout << "swingPos��ֵ  "<< positions["Root"].getZ() <<"  "<<positions[notmoving + "Knee"].getZ()<<"  "<<stepStrategy.xPos<<"  "<<  stepStrategy.NStep.swingLeginitialPos << endl;
				//	cout<< hskel.getArcAngle(notmoving + "Knee") + hskel.getArcAngle(notmoving + "HipX") << "  " << hskel.getArcAngle(notmoving + "Knee") << "  " << stepStrategy.initialCOMAngle << "  " << hskel.getArcAngle(notmoving + "HipX") << "  " << hskel.getArcAngle(notmoving + "Knee") << "  " << endl;
				}
				
			}
			stepStrategy.ModelDataFormat(flag);
			updateStepModel();
			
			//��β
			if (stepStrategy.NStep.isTR2 && !stepStrategy.NStep.isCurStep)
			{
				//��ÿһ��������ʱ���¼һ��ƫ��ֵ
				std::string notmoving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake + 1) % 2];
				std::string moving = stepStrategy.NStep.leg[(stepStrategy.NStep.legToTake) % 2];
				std::map<std::string, Vector3> positions;
				hskel.getArcPositions(positions);
				//stepStrategy.NStep.lastAnklePos = positions[notmoving + "Ankle"].getZ();//�ڹ滮�ڶ���ankle�켣��ʱ��ʹ��
				stepStrategy.NStep.offset = positions[moving + "Ankle"];//��¼ƫ������
				stepStrategy.NStep.isTR2 = 0;
				// cout << "�������޸�anklelastpos  " << stepStrategy.NStep.lastAnklePos << "  "<<positions[notmoving + "Ankle"].getZ()<<"  "<< positions[notmoving + "Ankle"].getY()<<endl;
				//	 cout << "�����޸�.offset  " << stepStrategy.NStep.offset.getX() << "  " << stepStrategy.NStep.offset.getY()<<"  " << stepStrategy.NStep.offset.getZ() << endl;
			}
			double pos = 0.15;
			if (stepStrategy.NStep.legToTake == 1) pos = -0.15;
			Point p(pos, stepStrategy.anklezPos, stepStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, stepStrategy.zPos, stepStrategy.xPos);
			markCOMProfile.push_back(p1);



		}
	}

}
void updateHipModel()
{
	hskel.setArcAngle("Body",   hipStrategy.BodyPitch);
	hskel.setArcAngle("LHipX",  hipStrategy.COMAngle);
	hskel.setArcAngle("RHipX",  hipStrategy.COMAngle);
	hskel.setArcAngle("RAnkle", -hipStrategy.COMAngle);
	hskel.setArcAngle("LAnkle", -hipStrategy.COMAngle);


	///////////////////////////////////////////////�޸�ƫ������˫��ʼ�վ�ֹ��ԭ�أ��������׹ؽ�ƫ������������λ��
	std::map<std::string, Vector3> positions;
	hskel.getArcPositions(positions);
	Vector3 root = *(hskel.getRootArc()->getTranslation());
	Vector3 v = positions["LAnkle"];//��ȡ���׵�����
	Vector3 voffset(humanData.hipwidth / 2, 0, 0);//���׹ؽھ�ֹλ������
	v -= voffset;
	hskel.getRootArc()->setTranslation(root.getX() - v.getX(), root.getY() - v.getY(), root.getZ() - v.getZ());
	hskin.update(&hskel);
}
void updateAnkleModel()
{
	///////////////////////////////////////////////��������ת��
	hskel.setArcAngle("LHipX", -ankleStrategy.hipAngle);
	hskel.setArcAngle("RHipX", -ankleStrategy.hipAngle);
	hskel.setArcAngle("RAnkle", ankleStrategy.hipAngle);
	hskel.setArcAngle("LAnkle", ankleStrategy.hipAngle);
	hskin.update(&hskel);

	///////////////////////////////////////////////�޸�ƫ������˫��ʼ�վ�ֹ��ԭ�أ��������׹ؽ�ƫ������������λ��
	std::map<std::string, Vector3> positions;
	hskel.getArcPositions(positions);
	Vector3 root = *(hskel.getRootArc()->getTranslation());
	Vector3 v = positions["LAnkle"];//��ȡ���׵�����
	Vector3 voffset(humanData.hipwidth/2,0,0);//���׹ؽھ�ֹλ������
	v -= voffset;
	hskel.getRootArc()->setTranslation(root.getX() - v.getX(), root.getY() - v.getY(), root.getZ() - v.getZ());
	hskin.update(&hskel);

	}
void takeAnkle(double xpos, double xrate)
{
	if (AnkleStartTime == -1) //�״ν��룬�����Գ�ʼ��
	{
		ankleStrategy.startStrategy(xpos, xrate);//��ʼ�����ź����ĵ�λ�ã��ٶ�
		AnkleStartTime = 0;
	}
	else  //���״ν���
	{	 
		if (!ankleStrategy.isEnd())//�����Դ����ٶ�ʱ����
		{
			double t= timer.getTime();
			if (AnkleStartTime == 0)
				AnkleStartTime = t;
			flag = t - AnkleStartTime;

			ankleStrategy.updateState(flag);
			ankleStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����� �׹ؽڵ���Ϣ
			cout << "�׹ؽڵ��ڵ���������" << ankleStrategy.xPos << "  " << ankleStrategy.xRate <<"  "<< t - AnkleStartTime << endl;
			Point p(0, ankleStrategy.anklezPos, ankleStrategy.anklexPos);
			markAnkleProfile.push_back(p);
			Point p1(0, ankleStrategy.zPos, ankleStrategy.xPos);
			markCOMProfile.push_back(p1);
			updateAnkleModel();
		}
	
	}

}
void takeHip(double xpos, double xrate)
{   
	if (StartTime == -1)  //��һ�ν��� ��ʼ���Źؽڲ��ԵĲ���
	{
		hipStrategy.startStrategy(xpos, xrate);//��ʼ���ٶ� ʱ�� ��׽��
		StartTime = 0;
	}
	else                  //���״ν���
	{		 
	    double t = timer.getTime();
	     if (StartTime == 0)
			 StartTime = t;
		 //flag = (t - StartTime);
		 flag += humanData.sampleTime;
		 if (hipStrategy.isTR2(flag))  //TR2ʱ��ʱ���Źؽ����ý���
			 flag = humanData.TR2;
			
		if (!hipStrategy.isEnd(flag))
		{
			hipStrategy.updateState((flag));			
			hipStrategy.ModelDataFormat();  //��õ�ǰʱ�̵����ĵ���Ϣ
			updateHipModel();
			if (hipStrategy.IsTR2==1)
			{
				tempXPos = hipStrategy.xPos;
				tempXRate = hipStrategy.xRate;
				hipStrategy.IsTR2 = 0;  //TR2ʱ�̽���
			}
	}
		else
		{
		
			if (ankleStrategy.IsUseful(tempXPos, tempXRate))
			{
				
				takeAnkle(tempXPos, tempXRate);
			}
		}
	}
}




void RecoverBalance(double xpos, double xrate)
{
	if (markRootPosFlag)  //�ڲ��Կ�ʼʱ�̣���¼��ʼ���ڵ��λ�ã�����updateAngleModel()��updateHipModel()
	{
		//markRootPos = *hskel.getRootArc()->getTranslation();
		//markRootPosFlag = false;
	}

	if (ankleStrategy.IsUseful(xpos, xrate))
	{
	
		takeAnkle(xpos, xrate);
	}
	else if (hipStrategy.IsUseful(xpos, xrate))
	{
		
		takeHip( xpos, xrate);
	
	}
	else if (stepStrategy.IsUseful(xpos, xrate) != -1)
	{
		if (NStepMarkFlag == 0)//�� ��һ����ֵ
		{
			stepStrategy.NStep.totalStep = stepStrategy.IsUseful(xpos, xrate);
			stepStrategy.NStep.curStep = 1;
			stepStrategy.NStep.xPos = xpos;
			stepStrategy.NStep.xRate = xrate;
			stepStrategy.NStep.curCOP = humanData.rf;
			NStepMarkFlag = 1;
		}

		if (stepStrategy.NStep.curStep <= stepStrategy.NStep.totalStep)   //������ȡ�ಽ
		
			takeStep( stepStrategy.NStep.xPos, stepStrategy.NStep.xRate);
		
	}
}

// Draw the scene
void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(bodypos.getX(), bodypos.getY(), bodypos.getZ() + CAMERA_DISTANCE,
		bodypos.getX(), bodypos.getY(), bodypos.getZ(),
		0.0, 1.0, 0.0);

	// Orient the scene view based on user input
	glRotated(xrot, 1.0, 0.0, 0.0);
	glRotated(yrot, 0.0, 1.0, 0.0);
	glRotated(zrot, 0.0, 0.0, 1.0);

	drawXYZ();
	drawSrcs();
	glColor3f(1.0f, 0.0f, 0.0f);

	std::vector<Point>::iterator iter;
	for (iter = markAnkleProfile.begin(); iter != markAnkleProfile.end(); iter++)
	{
		glPushMatrix();
		glTranslatef(iter->px, iter->py, iter->pz);
		glutWireSphere(0.025, 100, 100);
		glPopMatrix();
	}

	for (iter = markCOMProfile.begin(); iter != markCOMProfile.end(); iter++)
	{
		glPushMatrix();
		glTranslatef(iter->px, iter->py, iter->pz);
		glutWireSphere(0.025, 100, 100);
		glPopMatrix();
	}

	std::map<double, Point>::iterator iter2;
	for (iter2 = COM.begin(); iter2 != COM.end(); iter2++)
	{
		glPushMatrix();
		glTranslatef(iter2->second.px, iter2->second.py, iter2->second.pz);
		glutWireSphere(0.025, 100, 100);
		glPopMatrix();
	}

	for (iter2 = Ankle.begin(); iter2 != Ankle.end(); iter2++)
	{
		glPushMatrix();
		glTranslatef(iter2->second.px, iter2->second.py, iter2->second.pz);
		glutWireSphere(0.025, 100, 100);
		glPopMatrix();
	}
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);



	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	// Draw the skeleton
	glColor3f(0.5, 1.0, 1.0);
	if (walking)
		walkanimator.draw();//��������

	if (skinning)
		hskin.draw();

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}



// Update the animation
void animate()
{
  double t = timer.getTime();
  if(animating)//������a��
  {
    if(walking)//��·
    {
		std::cout<<currentChooseSrc;
      // Move the actor
      Vector3* pos = hskel.getRootArc()->getTranslation();

      if(pos->getZ() < 10.0)
      {
		  //С������z��ǰ�� newz��ʾһ��ʱ�����ߵ�·�� newzԽ�� �ߵ�Խ��
        double newz = pos->getZ() + (ACTOR_VELOCITY* rate * (t - lastTime));//�ٶ�*ʱ��

	     //��ʼ��Ҫ���ڵ����ϣ��˴�ͨ������ground���� ��ô�ʱ��newy
        double newy = INITIAL_ELEVATION + ground.getHeight(pos->getX(), newz);

		/*double newy = INITIAL_ELEVATION;
		xһ��  С��������x�ᴹֱ ָ��z����ķ��� ��ʼ���� ���߹����г��˵���z(����ˮƽλ��)
		����Ҫ���ϵ���y��λ��(���ĸߵ�)*/
        hskel.getRootArc()->setTranslation(pos->getX(), newy, newz);

        // Update animation/correct foot elevetaion if ankle penetrates floor
        walkanimator.update();

        // Get current position for camera update
        bodypos = *hskel.getRootArc()->getTranslation();

        // Update the skin
        hskin.update(&hskel);
      }
    }
  }

  // Store current time for time since last frame calculation
  else if (yacobi)
  {
	  if (currentChooseIK != "NULL")
	  {
		  yacobianimation = new actionAnimation();
		  jacobiframer = new actionFramer();
		  yacobianimation->addActor(&hskel, jacobiframer);
		  yacobianimation->setJacobi(currentChooseIK, currentChooseTranslate);
		  hskin.update(&hskel);

		  currentChooseTranslate.set(0,0,0);
		  currentChooseIK = "NULL";
	  }
	  if (yacobianimation != NULL)
	  {
		  hskin.update(&hskel);
	  }
  }
  else if (ToRecoverBalance)      //����X������ʾ��ʼ����ģ������ƽ��ָ�
  {
	  std::map<std::string, double>angles_;
	  RecoverBalance(0,1.5);  //�Ƕȣ�xpos�����ٶ�
  }
  
  else if(action)
  { 
	  if (currentChooseSrc != "NULL")
	  {
		  forTest = new actionFramer();
		  forTest->addFrame(currentChooseSrc, currentChooseAngle1, currentChooseAngle2);
		  forTest->setRate(rate);
		  tempanimation = new actionAnimation();
		  tempanimation->addActor(&hskel, forTest);
		  

		  currentChooseAct = 0;
		  currentChooseAngle1 = 0;
		  currentChooseAngle2= 0;
		  currentChooseSrc = "NULL";
	  }

	  if (tempanimation != NULL)
	  {
		  tempanimation->update();
		  bodypos = *hskel.getRootArc()->getTranslation();
		  hskin.update(&hskel);
		 
	  }
  }
  lastTime = t;
  glutPostRedisplay();
}

// Process keyboard input
void key(unsigned char k, int xx, int yy)
{
	switch (k)
	{
	case 'w':
		animating = !animating;
		break;

	case 'a': case'b':case'c':case'd':case'e':case'f':case'g':case'h':case'i':case'j':
	case'k':case 'l': case'm':case'n':case'o':case'p':case'q':case'r':case's':case't':case'u':
		if (!animating &&currentChooseSrc == "NULL")
		{
			currentChooseSrc = markSrc[k - 'a'];
			cout << "you have chose  " << currentChooseSrc << endl;

			cout << "you have chose  rotate " << endl;
			cout << "input angle start and  end:   ";
			cin >> currentChooseAngle1 >> currentChooseAngle2;
			cout << "you have set angle  " << currentChooseAngle1 << "   " << currentChooseAngle2 << endl;
			cout << endl << endl;


		}
		break;
	case 'z':
		yacobi = !yacobi;
		if (yacobi)
		{
			cout << "you have chose  translate " << endl;
			cout << "input translate target num ( 1: RLeg  2:RArm  3:LLeg  4:LArm ) " << endl;

			GLdouble a, b, c;
			int num;
			cin >> num;
			switch (num)
			{
			case 1: currentChooseIK = "RLeg"; break;
			case 2: currentChooseIK = "RArm"; break;
			case 3: currentChooseIK = "LLeg"; break;
			case 4: currentChooseIK = "LArm"; break;
			default: break;
			}
			cout << "input translate position (x y z)" << endl;
			cin >> a >> b >> c;
			currentChooseTranslate.set(a, b, c);
			cout << "you have choose " << currentChooseIK << "and set vector  " << "(  " << currentChooseTranslate.getX() << "   " << currentChooseTranslate.getY() << "   " << currentChooseTranslate.getZ() << ")   " << endl;
		}
		break;

	case 'y':
		skinning = !skinning;
		break;
	case 'x':
		ToRecoverBalance = !ToRecoverBalance;
		porduceData = true;
		break;
	default:
		break;
	}

}


// Reshaping routine called whenever window is resized or moved
void myreshape(GLsizei w, GLsizei h)
{
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0f,(GLfloat)w/(GLfloat)h,0.1f,100000.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set global size
  ww = w;
  wh = h;
}

// Set initial OpenGL configuration
void myinit(void)
{
  GLfloat LightAmbient[]= { 0.4f, 0.4f, 0.4f, 1.0f };
  GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_FILL);

  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  // Initialize time
  lastTime = timer.getTime();
}

// Print usage instructions to console
void print_usage()
{
 

	cout << "a: ROOT                 " << "b: NECKBASE             " << "c: HEADBASE             " << "d: RSHOULDERX          " << "e: RSHOULDERY          " << endl;
	cout << "f: RSHOULDERZ           " << "g:RELBOW                " << "h: LSHOULDERX           " << "i: LSHOULDERY          " << "j: LSHOULDERZ           " << endl;
	cout << "k: LELBOW               " << "l: RHIPX                " << "m:RHIPY                 " << "n:RHIPZ                " << "o: RKNEE           " << endl;
	cout<< "p: RANKLE               " << "q: LHIPX                " << "r: LHIPY                " << "s: LHIPZ               " << "t:LKNEE" << endl;
	cout << "u: LANKLE     " << endl; 
	cout <<endl<< "or print [w] to start walking..." << endl;

}


int main(int argc, char** argv)
{
	// Print instructions for use to console
	print_usage();
	currentChooseSrc = "NULL";
	currentChooseIK = "NULL";

	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL | GLUT_DEPTH);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("Assignment 2");

	//currentStrategy = ONESTEP;
	// Initialize OpenGL settings
	myinit();//������Դ ʹ�������������

	humanData.look();

	// Initialize the skeleton
	hskel.create();//����ÿ���Ǽܵ�ƽ�� ���� ��ת�Ƕ� �� ����  �Լ��ӹؽ�
				   //����ÿ���ؽڵ����ɶ� �Լ������Ǽ�


	// Set the initial position for the skeleton
	//bodypos.set(0.0, INITIAL_ELEVATION - (0.39 * NUM_STAIRS), INITIAL_POSITION - (0.54 * NUM_STAIRS));
	bodypos.set(0, humanData.legLen, 0.0);
	(hskel.getRootArc())->setTranslation(bodypos);//�Ǽܵ�root����xyz��ƽ��
							         			  //hskel.findArc("Body")->setRotationAngle(15);
												  //Vector3 v(1,1.2805,0);
												  //hskel.setArcPosition("RHipZ",v);
												  // Initialize the skin
	hskin.create();
	hskin.update(&hskel);

	// Set the rate for the forward kinematic solver
	walkframer.setRate(rate);
	//stairframer.setRate(rate);

	// Add the skeleton and forward kinematic solver to the animators
	walkanimator.addActor(&hskel, &walkframer);
	//stairanimator.addActor(&hskel, &stairframer);



	glutReshapeFunc(myreshape);
	glutDisplayFunc(mydisplay);//��������ע��һ����ͼ����
	glutSpecialFunc(SpecialKeys);
	glutIdleFunc(animate);
	glutKeyboardFunc(key);
	glutMainLoop();

	return 0;
}
//act
//actor ���ɹǼ� ֡Framer  �Լ��ùǼܵĹؽ���ת�Ƕ�map�� ���<std::string, SkelDOF>
//��¼�ùǼܵ�ÿ���ؽڵ���ת��
//initializer ��actorvisitor ����������ָ��actor ʵ��������<string skeldof>�ġ�


//animation����һϵ�е�acotr���   vector<Actor*> 
//ÿ�����actorʱ ��Ҫʵ����  �ùǼܴ�root_��ʼ  actor angles_  
