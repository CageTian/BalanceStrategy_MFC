#include "stdafx.h"
#include <map>
#include <string>
#include "names.h"
#include "shapecube.h"
#include "bonecluster.h"
#include "humanskeleton.h"

HumanSkeleton::HumanSkeleton(HumanData h)
	: Skeleton(NULL)
{
	humanData = h;
}

HumanSkeleton::~HumanSkeleton()
{
	destroySkeleton();
}

void HumanSkeleton::create()
{
	buildSkeleton();
	initializeAngles();
}

//以下函数特别为 跨步策略初始化模型状态准备
void  HumanSkeleton::setAngles(std::map<std::string, double>angles_)
{
	std::map<std::string, double>::iterator iter;
	for (iter = angles_.begin(); iter != angles_.end(); iter++)
	{
		setArcAngle(iter->first, iter->second);
		//cout << "蓝看设置"<<iter->first << "  " << iter->second<<endl;
	}
	//double offx = pos;
	//double  offz = humanData.l - sqrt(pow(humanData.l, 2) - pow(offx, 2));
	//Vector3 tempCOM = *getRootArc()->getTranslation();
	//getRootArc()->setTranslation(tempCOM.getX(),tempCOM.getY()-offz,tempCOM.getZ()+offx);
}


void HumanSkeleton::setArcPosition(std::string name, Vector3 v)
{
	SkelArc*  tempArc = NULL;
	tempArc = findArc(name);
	if (tempArc != NULL)
		tempArc->setTranslation(v);

}

void HumanSkeleton::setArcAngle(std::string name, double angle)
{
	SkelArc*  tempArc = NULL;
	tempArc = findArc(name);
	if (tempArc != NULL)
		tempArc->setRotationAngle(angle);

}

std::map<std::string, double> HumanSkeleton::getAngles()
{
	std::map<std::string, double>  angles_;
	
	angles_.insert(std::make_pair("Body", (*findArc("Body")->getRotationAngle()).getValue()));
	angles_.insert(std::make_pair(ROOT,(*findArc("Root")->getRotationAngle()).getValue()));//??????
	angles_.insert(std::make_pair(RHIPZ, (*findArc(RHIPZ)->getRotationAngle()).getValue()));//-z
	angles_.insert(std::make_pair(RHIPY, (*findArc(RHIPY)->getRotationAngle()).getValue()));//-y
	angles_.insert(std::make_pair(RHIPX, (*findArc(RHIPX)->getRotationAngle()).getValue()));//-x
	angles_.insert(std::make_pair(RKNEE, (*findArc(RKNEE)->getRotationAngle()).getValue()));//-x
	angles_.insert(std::make_pair(RANKLE, (*findArc(RANKLE)->getRotationAngle()).getValue()));//-x
	angles_.insert(std::make_pair(LHIPZ, (*findArc(LHIPZ)->getRotationAngle()).getValue()));//-z
	angles_.insert(std::make_pair(LHIPY, (*findArc(LHIPY)->getRotationAngle()).getValue()));//-y
	angles_.insert(std::make_pair(LHIPX, (*findArc(LHIPX)->getRotationAngle()).getValue()));//-x
	angles_.insert(std::make_pair(LKNEE, (*findArc(LKNEE)->getRotationAngle()).getValue()));//-x
	angles_.insert(std::make_pair(LANKLE, (*findArc(LANKLE)->getRotationAngle()).getValue()));//-x
	return angles_;
}
void HumanSkeleton::initializeAngles()
{

	AngleInitializer ai;
	ai.angles.insert(std::make_pair(ROOT, SkelDOF(0.0, -45.0, 45.0)));//??????
	ai.angles.insert(std::make_pair(NECKBASE, SkelDOF(0.0, -90.0, 90.0)));// -y
	ai.angles.insert(std::make_pair(HEADBASE, SkelDOF(0.0, -45.0, 45.0))); //-x
	ai.angles.insert(std::make_pair(RSHOULDERZ, SkelDOF(0.0, 0.0, 90.0))); //-z
	ai.angles.insert(std::make_pair(RSHOULDERY, SkelDOF(0.0, -90.0, 90.0))); //-y
	ai.angles.insert(std::make_pair(RSHOULDERX, SkelDOF(0.0, -90.0, 90.0))); //-x
	ai.angles.insert(std::make_pair(RELBOW, SkelDOF(0.0, 0.0, 90.0))); //-x
	ai.angles.insert(std::make_pair(LSHOULDERZ, SkelDOF(0.0, 0.0, 90.0)));//-z
	ai.angles.insert(std::make_pair(LSHOULDERY, SkelDOF(0.0, -90.0, 90.0)));//-y
	ai.angles.insert(std::make_pair(LSHOULDERX, SkelDOF(0.0, -90.0, 90.0)));//-x
	ai.angles.insert(std::make_pair(LELBOW, SkelDOF(0.0, 0.0, 90.0)));//-x
	ai.angles.insert(std::make_pair(RHIPZ, SkelDOF(0.0, -90.0, 90.0)));//-z
	ai.angles.insert(std::make_pair(RHIPY, SkelDOF(0.0, -90.0, 90.0)));//-y
	ai.angles.insert(std::make_pair(RHIPX, SkelDOF(0.0, -90.0, 90.0)));//-x
	ai.angles.insert(std::make_pair(RKNEE, SkelDOF(0.0, -120.0, -10.0)));//-x
	ai.angles.insert(std::make_pair(RANKLE, SkelDOF(0.0, -45.0, 45.0)));//-x
	ai.angles.insert(std::make_pair(LHIPZ, SkelDOF(0.0, -90.0, 90.0)));//-z
	ai.angles.insert(std::make_pair(LHIPY, SkelDOF(0.0, -90.0, 90.0)));//-y
	ai.angles.insert(std::make_pair(LHIPX, SkelDOF(0.0, -90.0, 90.0)));//-x
	ai.angles.insert(std::make_pair(LKNEE, SkelDOF(0.0, -120.0, -10.0)));//-x
	ai.angles.insert(std::make_pair(LANKLE, SkelDOF(0.0, -45.0, 45.0)));//-x

	visitArcs(ai);
}

/*void HumanSkeleton::buildSkeleton()
{
	ShapeBone* bone;
	ShapeCube* empty;
	ShapeCube* COMM;
	//先设置质心骨架为一根棍
	 empty = new ShapeCube();
	//empty->setScale(0.015,1.015, 0.515);
	empty->setScale(0.0, 0.0, 0.0);
	bone = new ShapeBone(ShapeBone::HORIZONTALX, 0.3);
	SkelNode* COMNode = new SkelNode("Root", empty);//创建一个根骨架，根骨加的缩放比例是0:0:0)
	SkelArc* COM = new SkelArc("Root", COMNode);//创建一个根关节，根关节关联根骨架
	COM->setRotationAxis(1.0, 0.0, 0.0); //关节有旋转轴？？
	setRootArc(COM); //定义骨架的根部关节   为刚才创建的根关节



					 //设置上半身为一根竖直的棍
	bone = new ShapeBone(ShapeBone::VERTICAL, 1.0);//缩放比例是：(0.01,0.01,1.0)
	bone->setTranslation(0.0, 0.5, 0.0);
	SkelNode* torso = new SkelNode("Torso", bone);//躯干骨架 躯干是由三个骨架组成，缩放平移情况如下
	SkelArc* body = new SkelArc("Body", torso);//躯干关节 并关联躯干骨架
	body->setRotationAxis(1.0, 0.0, 0.0); //关节有旋转轴？？
	COMNode->addArc(body); //root骨架的子关节是躯干关节


						  
						   // Create upper right leg - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperlegx = new SkelNode("RUpperLegX", empty);
	SkelArc* rhipx = new SkelArc("RHipX", rupperlegx);
	rhipx->setTranslation(-0.15, 0, 0.0);
	rhipx->setRotationAxis(-1.0, 0.0, 0.0);
	COMNode->addArc(rhipx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperlegy = new SkelNode("RUpperLegY", empty);
	SkelArc* rhipy = new SkelArc("RHipY", rupperlegy);
	rhipy->setRotationAxis(0.0, -1.0, 0.0);
	rupperlegx->addArc(rhipy);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.ul);
	bone->setTranslation(0.0, -humanData.ul / 2, 0.0);
	SkelNode* rupperlegz = new SkelNode("RUpperLegZ", bone);
	SkelArc* rhipz = new SkelArc("RHipZ", rupperlegz);
	rhipz->setRotationAxis(0.0, 0.0, -1.0);
	rupperlegy->addArc(rhipz);


	// Create lower right leg
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.dl);
	bone->setTranslation(0.0, -humanData.dl / 2, 0.0);
	SkelNode* rlowerleg = new SkelNode("RLowerLeg", bone);
	SkelArc* rknee = new SkelArc("RKnee", rlowerleg);
	rknee->setTranslation(0.0, -humanData.ul, 0.0);
	rknee->setRotationAxis(-1.0, 0.0, 0.0);
	rupperlegz->addArc(rknee);


	// Create right foot
	bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
	bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
	SkelNode* rfoot = new SkelNode("RFoot", bone);
	SkelArc* rankle = new SkelArc("RAnkle", rfoot);
	rankle->setTranslation(0.0, -humanData.dl, 0.0);
	rankle->setRotationAxis(-1.0, 0.0, 0.0);
	rlowerleg->addArc(rankle);


	// Create upper left leg - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperlegx = new SkelNode("LUpperLegX", empty);
	SkelArc* lhipx = new SkelArc("LHipX", lupperlegx);
	lhipx->setTranslation(0.15, 0, 0.0);
	lhipx->setRotationAxis(-1.0, 0.0, 0.0);
	COMNode->addArc(lhipx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperlegy = new SkelNode("LUpperLegY", empty);
	SkelArc* lhipy = new SkelArc("LHipY", lupperlegy);
	lhipy->setRotationAxis(0.0, -1.0, 0.0);
	lupperlegx->addArc(lhipy);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.ul);
	bone->setTranslation(0.0, -humanData.ul / 2, 0.0);                             // Put top of bone at origin
	SkelNode* lupperlegz = new SkelNode("LUpperLegZ", bone);
	SkelArc* lhipz = new SkelArc("LHipZ", lupperlegz);
	lhipz->setRotationAxis(0.0, 0.0, -1.0);
	lupperlegy->addArc(lhipz);


	// Create lower left leg
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.dl);
	bone->setTranslation(0.0, -humanData.dl / 2, 0.0);                             // Put top of bone at origin
	SkelNode* llowerleg = new SkelNode("LLowerLeg", bone);
	SkelArc* lknee = new SkelArc("LKnee", llowerleg);
	lknee->setTranslation(0.0, -humanData.ul, 0.0);
	lknee->setRotationAxis(-1.0, 0.0, 0.0);
	lupperlegz->addArc(lknee);

	// Create left foot
	bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
	bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
	SkelNode* lfoot = new SkelNode("LFoot", bone);
	SkelArc* lankle = new SkelArc("LAnkle", lfoot);
	lankle->setTranslation(0.0, -humanData.dl, 0.0);
	lankle->setRotationAxis(-1.0, 0.0, 0.0);
	llowerleg->addArc(lankle);
}
*/

double HumanSkeleton::getArcAngle(std::string name)
{
	double angle = 0;
	Vector3 v(0, 0, 0);
	SkelArc*  tempArc = NULL;
	tempArc = findArc(name);
	if (tempArc != NULL)
		angle = tempArc->getRotationAngle()->getValue();
	//*(tempArc->getTranslation());
	return  angle;

}
void HumanSkeleton::buildSkeleton()
{
	ShapeBone* bone;
	ShapeCube* empty;

	empty = new ShapeCube();
	//empty->setScale(0.015,1.015, 0.515);
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rootNode = new SkelNode("Root", empty);
	SkelArc* rootArc = new SkelArc("Root", rootNode);
	setRootArc(rootArc);



	// Create the torso - a bone cluster  创建躯干   肩部
	BoneCluster *bones=new BoneCluster();
	bones->addBone(ShapeBone::HORIZONTALX, humanData.shoulderwidth, Vector3(0.0,  humanData.torse, 0.0));
	bones->addBone(ShapeBone::VERTICAL, humanData.torse, Vector3(0.0,  humanData.torse /2, 0.0));
	bones->addBone(ShapeBone::HORIZONTALX, humanData.hipwidth, Vector3(0.0, 0.0, 0.0));
	//bone = new ShapeBone(ShapeBone::VERTICAL,1.0);
	//bone->setTranslation(Vector3(0.0, 0.5, 0.0));
	SkelNode* torso = new SkelNode("Torso", bones);//躯干
	SkelArc* body = new SkelArc("Body", torso);
	body->setRotationAxis(1.0, 0.0, 0.0);
	rootNode->addArc(body);


	// Create the neck
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.neck);
	bone->setTranslation(0.0, humanData.neck / 2, 0.0);                         // Put bottom of bone at origin
	SkelNode* neck = new SkelNode("Neck", bone);
	SkelArc* neckBase = new SkelArc("NeckBase", neck);
	neckBase->setTranslation(0.0, humanData.torse, 0.0);
	neckBase->setRotationAxis(0.0, 1.0, 0.0);
	torso->addArc(neckBase);

	// Create the head
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.head);
	bone->setTranslation(0.0, humanData.head / 2, 0.0);                          // Put bottom of bone at origin

	SkelNode* head = new SkelNode("Head", bone);
	SkelArc* headBase = new SkelArc("HeadBase", head);
	headBase->setTranslation(0.0, humanData.neck / 2, 0.0);
	headBase->setRotationAxis(-1.0, 0.0, 0.0);
	neck->addArc(headBase);

	// Create upper right arm - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperarmx = new SkelNode("RupperArmX", empty);
	SkelArc* rshoulderx = new SkelArc("RShoulderX", rupperarmx);
	rshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
	rshoulderx->setTranslation(-humanData.shoulderwidth / 2, humanData.torse, 0.0);
	torso->addArc(rshoulderx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperarmy = new SkelNode("RupperArmY", empty);
	SkelArc* rshouldery = new SkelArc("RShoulderY", rupperarmy);
	rshouldery->setRotationAxis(0.0, -1.0, 0.0);
	rupperarmx->addArc(rshouldery);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.upperarm);
	bone->setTranslation(0.0, -humanData.upperarm / 2, 0.0);// Put top of bone at origin
	SkelNode* rupperarmz = new SkelNode("RUpperArmZ", bone);
	SkelArc* rshoulderz = new SkelArc("RShoulderZ", rupperarmz);
	rshoulderz->setRotationAxis(0.0, 0.0, -1.0);
	rupperarmy->addArc(rshoulderz);


	// Create lower right arm
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.lowerarm);
	bone->setTranslation(0.0, -humanData.lowerarm / 2, 0.0);                       // Put top of bone at origin
	SkelNode* rlowerarm = new SkelNode("RLowerArm", bone);
	SkelArc* relbow = new SkelArc("RElbow", rlowerarm);
	relbow->setTranslation(0.0, -humanData.upperarm / 2, 0.0);
	relbow->setRotationAxis(-1.0, 0.0, 0.0);
	rupperarmz->addArc(relbow);


	// Create upper left arm - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperarmx = new SkelNode("LupperArmX", empty);
	SkelArc* lshoulderx = new SkelArc("LShoulderX", lupperarmx);
	lshoulderx->setTranslation(humanData.shoulderwidth / 2, humanData.torse, 0.0);
	lshoulderx->setRotationAxis(-1.0, 0.0, 0.0);
	torso->addArc(lshoulderx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperarmy = new SkelNode("LupperArmY", empty);
	SkelArc* lshouldery = new SkelArc("LShoulderY", lupperarmy);
	lshouldery->setRotationAxis(0.0, -1.0, 0.0);
	lupperarmx->addArc(lshouldery);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.upperarm);
	bone->setTranslation(0.0, -humanData.upperarm / 2, 0.0);                       // Put top of bone at origin
	SkelNode* lupperarmz = new SkelNode("LUpperArmZ", bone);
	SkelArc* lshoulderz = new SkelArc("LShoulderZ", lupperarmz);
	lshoulderz->setRotationAxis(0.0, 0.0, -1.0);
	lupperarmy->addArc(lshoulderz);


	// Create lower left arm
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.lowerarm);
	bone->setTranslation(0.0, -humanData.lowerarm / 2, 0.0);                       // Put top of bone at origin
	SkelNode* llowerarm = new SkelNode("LLowerArm", bone);
	SkelArc* lelbow = new SkelArc("LElbow", llowerarm);
	lelbow->setTranslation(0.0, -humanData.upperarm / 2, 0.0);
	lelbow->setRotationAxis(-1.0, 0.0, 0.0);
	lupperarmz->addArc(lelbow);


	// Create upper right leg - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperlegx = new SkelNode("RUpperLegX", empty);
	SkelArc* rhipx = new SkelArc("RHipX", rupperlegx);
	rhipx->setTranslation(-0.15, 0.0, 0.0);
	rhipx->setRotationAxis(-1.0, 0.0, 0.0);
	rootNode->addArc(rhipx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* rupperlegy = new SkelNode("RUpperLegY", empty);
	SkelArc* rhipy = new SkelArc("RHipY", rupperlegy);
	rhipy->setRotationAxis(0.0, -1.0, 0.0);
	rupperlegx->addArc(rhipy);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.upperleg);
	bone->setTranslation(0.0, -humanData.upperleg/2, 0.0);
	SkelNode* rupperlegz = new SkelNode("RUpperLegZ", bone);
	SkelArc* rhipz = new SkelArc("RHipZ", rupperlegz);
	rhipz->setRotationAxis(0.0, 0.0, -1.0);
	rupperlegy->addArc(rhipz);


	// Create lower right leg
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.lowerleg);
	bone->setTranslation(0.0, -humanData.lowerleg/2, 0.0);
	SkelNode* rlowerleg = new SkelNode("RLowerLeg", bone);
	SkelArc* rknee = new SkelArc("RKnee", rlowerleg);
	rknee->setTranslation(0.0, -humanData.upperleg, 0.0);
	rknee->setRotationAxis(-1.0, 0.0, 0.0);
	rupperlegz->addArc(rknee);


	// Create right foot
	bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
	bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
	SkelNode* rfoot = new SkelNode("RFoot", bone);
	SkelArc* rankle = new SkelArc("RAnkle", rfoot);
	rankle->setTranslation(0.0, -humanData.lowerleg, 0.0);
	rankle->setRotationAxis(-1.0, 0.0, 0.0);
	rlowerleg->addArc(rankle);


	// Create upper left leg - three arcs for 3DOF with 2 zero length nodes
	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperlegx = new SkelNode("LUpperLegX", empty);
	SkelArc* lhipx = new SkelArc("LHipX", lupperlegx);
	lhipx->setTranslation(0.15, 0.0, 0.0);
	lhipx->setRotationAxis(-1.0, 0.0, 0.0);
	rootNode->addArc(lhipx);

	empty = new ShapeCube();
	empty->setScale(0.0, 0.0, 0.0);
	SkelNode* lupperlegy = new SkelNode("LUpperLegY", empty);
	SkelArc* lhipy = new SkelArc("LHipY", lupperlegy);
	lhipy->setRotationAxis(0.0, -1.0, 0.0);
	lupperlegx->addArc(lhipy);

	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.upperleg);
	bone->setTranslation(0.0, -humanData.upperleg/2, 0.0);                             // Put top of bone at origin
	SkelNode* lupperlegz = new SkelNode("LUpperLegZ", bone);
	SkelArc* lhipz = new SkelArc("LHipZ", lupperlegz);
	lhipz->setRotationAxis(0.0, 0.0, -1.0);
	lupperlegy->addArc(lhipz);


	// Create lower left leg
	bone = new ShapeBone(ShapeBone::VERTICAL, humanData.lowerleg);
	bone->setTranslation(0.0, -humanData.lowerleg / 2, 0.0);                             // Put top of bone at origin
	SkelNode* llowerleg = new SkelNode("LLowerLeg", bone);
	SkelArc* lknee = new SkelArc("LKnee", llowerleg);
	lknee->setTranslation(0.0, -humanData.upperleg, 0.0);
	lknee->setRotationAxis(-1.0, 0.0, 0.0);
	lupperlegz->addArc(lknee);

	// Create left foot
	bone = new ShapeBone(ShapeBone::HORIZONTALZ, 0.3);
	bone->setTranslation(0.0, -ShapeBone::BONEWIDTH_2, 0.15 - ShapeBone::BONEWIDTH_2);
	SkelNode* lfoot = new SkelNode("LFoot", bone);
	SkelArc* lankle = new SkelArc("LAnkle", lfoot);
	lankle->setTranslation(0.0, -humanData.lowerleg, 0.0);
	lankle->setRotationAxis(-1.0, 0.0, 0.0);
	llowerleg->addArc(lankle);
}

void HumanSkeleton::destroyChildren(const SkelArc* arc)
{
	SkelNode* node = arc->getNode();

	//Process the children
	skelarcs_t::const_iterator i;
	for (i = node->begin(); i != node->end(); ++i)
	{
		destroyChildren(*i);
		delete *i;
	}

	delete node->getShape();
	delete node;
}
