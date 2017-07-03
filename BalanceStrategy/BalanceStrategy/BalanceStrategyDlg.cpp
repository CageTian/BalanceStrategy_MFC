
// BalanceStrategyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BalanceStrategy.h"
#include "BalanceStrategyDlg.h"
#include "afxdialogex.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



////////////////////////////////////////////////
int view_i = 0;
GLdouble x = 0;
GLdouble y = 0.0;
GLdouble z = 12.25;
GLdouble xrot = 23.0;// 2.5;
GLdouble yrot = -45.0;// 251.5;
GLdouble zrot = 0.0;
bool yeyeye = 0;
GLfloat LightPosition[4] = {15.0f,25.0f,15.0f,1.0f};
int default_radio[3] = { IDC_RADIO6,IDC_RADIO8,IDC_RADIO12 };
int clear_radio[5] = { IDC_RADIO5,IDC_RADIO7,IDC_RADIO9,IDC_RADIO10,IDC_RADIO11 };
// Position of light in the scene
						 ///////////////////////////////////////////////


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBalanceStrategyDlg 对话框

 
CBalanceStrategyDlg::CBalanceStrategyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BALANCESTRATEGY_DIALOG, pParent)
	, m_speed(0)
	, m_height(0)
	, m_legLen(0)
	, m_lf(0)
	, m_rf(0)
	, m_stepTime(0)
	, m_maxStep(0)
	, m_Qmax(0)
	, m_sampleTime(0)
{
	
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBalanceStrategyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Radio(pDX, IDC_RADIO5, m_zy);
	DDX_Control(pDX, IDC_BUTTON1, m_run);
	DDX_Text(pDX, IDC_EDIT1, m_speed);
	DDV_MinMaxDouble(pDX, m_speed, 0, 5);
	DDX_Text(pDX, IDC_EDIT3, m_height);
	DDV_MinMaxDouble(pDX, m_height, 1, 2.5);
	DDX_Text(pDX, IDC_EDIT4, m_legLen);
	DDV_MinMaxDouble(pDX, m_legLen, 0.5, 2);
	DDX_Text(pDX, IDC_EDIT7, m_lf);
	DDV_MinMaxDouble(pDX, m_lf, 0, 1);
	DDX_Text(pDX, IDC_EDIT8, m_rf);
	DDV_MinMaxDouble(pDX, m_rf, 0, 1);
	DDX_Text(pDX, IDC_EDIT5, m_stepTime);
	DDV_MinMaxDouble(pDX, m_stepTime, 0, 20);
	DDX_Text(pDX, IDC_EDIT6, m_maxStep);
	DDV_MinMaxInt(pDX, m_maxStep, 0, 100);
	DDX_Text(pDX, IDC_EDIT9, m_Qmax);
	DDX_Control(pDX, IDC_TCHART1, m_chart1);
	DDX_Control(pDX, IDC_TCHART2, m_chart2);
	DDX_Text(pDX, IDC_EDIT10, m_sampleTime);
	DDV_MinMaxDouble(pDX, m_sampleTime, 0, 5);
}

BEGIN_MESSAGE_MAP(CBalanceStrategyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CBalanceStrategyDlg::OnKillfocusEdit1)
	ON_BN_CLICKED(IDCANCEL, &CBalanceStrategyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// Reshaping routine called whenever window is resized or moved
void myreshape(GLsizei w, GLsizei h)
{
	//glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void CBalanceStrategyDlg::SpecialKeys(int key, int x, int y)
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
void CBalanceStrategyDlg::drawSrcs()
{
		std::map<std::string, Vector3> positions;
		controller.hskel.getArcPositions(positions);
		std::map<std::string, Vector3>::iterator it;
		for (it = positions.begin(); it != positions.end(); ++it)
		{  
			glColor3f(1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glTranslatef((it->second).getX(), (it->second).getY(), (it->second).getZ());
			glutWireSphere(0.025, 100, 100);
			glPopMatrix();
		}
}
void CBalanceStrategyDlg::drawXYZ()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3d(0 + 1, 0, 0);
	glVertex3d(0 - 1, 0, 0);
	glVertex3d(0 + 1, 0, 0);
	glVertex3d(0 + 0.95, 0, 0 + 0.05);
	glVertex3d(0 + 1, 0, 0);
	glVertex3d(0 + 0.95, 0, 0 - 0.05);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3d(0, 0 - 1, 0);
	glVertex3d(0, 0 + 1.15, 0);
	glVertex3d(0, 0 + 1.15, 0);
	glVertex3d(0 - 0.05, 0 + 1.1, 0);
	glVertex3d(0, 0 + 1.15, 0);
	glVertex3d(0 + 0.05, 0 + 1.1, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3d(0, 0, 0 - 1);
	glVertex3d(0, 0, 0 + 1);
	glVertex3d(0, 0, 0 + 1);
	glVertex3d(0 + 0.05, 0, 0 + 0.95);
	glVertex3d(0, 0, 0 + 1);
	glVertex3d(0 - 0.05, 0, 0 + 0.95);
	glEnd();

} 
// CBalanceStrategyDlg 消息处理程序

// Set initial OpenGL configuration
void myinit(void)
{
	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

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

}
BOOL CBalanceStrategyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

									// TODO: 在此添加额外的初始化代码

	lineSeries = (CSeries)m_chart1.Series(0);
	lineSeries2 = (CSeries)m_chart2.Series(0);  

	lineSeries.Clear();
	lineSeries2.Clear();
									///////////////////////OPENGL INIT///////////////////////// 
	
	
	for(int i=0;i<3;i++){
		CButton* radio = (CButton*)GetDlgItem(default_radio[i]);
		radio->SetCheck(1);
	}
	
	CWnd *wnd = GetDlgItem(IDC_RENDER);
	hrenderDC = ::GetDC(wnd->m_hWnd);
	if (SetWindowPixelFormat(hrenderDC) == FALSE)
		return 0;

	if (CreateViewGLContext(hrenderDC) == FALSE)
		return 0;

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	/////////////////////////////////////////// 
	glEnable(GL_TEXTURE_2D);
	myinit();//带开光源 使得立体更加立体
	glViewport(600, 0, 230* 2, 190 * 2);
	//glViewport(500, 0, 259*2, 231*2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);       // Enable Smooth Shading 
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background 
	glClearDepth(1.0f);         // Depth Buffer Setup 
	glEnable(GL_DEPTH_TEST);       // Enables Depth Testing 
	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do 
							///////////////////////////////////////////////////////////////////////// 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	SetTimer(1, 10, 0);
	controller.initControl();
	//////////////////////////////////////////////////////////////// 
	//UpdateData(false);
	
	controlValue.isRun = 0;
	m_height = controller.humanData.height;
	m_legLen = controller.humanData.legLen;
	m_lf = controller.humanData.lf;
	m_maxStep = controller.humanData.maxSteps;
	m_rf = controller.humanData.rf;
	m_stepTime = controller.humanData.stepTime;
	m_Qmax = controller.humanData.Qmax;
	m_sampleTime = controller.humanData.sampleTime;
	UpdateData(false);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBalanceStrategyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBalanceStrategyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBalanceStrategyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBalanceStrategyDlg::OnTimer(UINT nIDEvent) //实时绘制场景 
{
	if (controlValue.isRun == 1)   //如果不为0 则暂停
	{
		
		if (controller.isFinished)
		{

			controlValue.isRun = 0;
			controlValue.stategyToTake = AUTO;
		}
		else

		{
			controller.RecoverBalance(0, controlValue.initialRate, controlValue.stategyToTake);
			controlValue.xRate = controlValue.initialRate;
			controller.getxPosAndxRate(controlValue.xPos, controlValue.xRate, controlValue.time);
			if (controlValue.time >= 0)
			{
				lineSeries.AddXY((double)controlValue.time, controlValue.xPos, NULL, NULL);
				lineSeries2.AddXY((double)controlValue.time, controlValue.xRate, NULL, NULL);
			}
		}
	}
	if (controlValue.isRestet == 1)
	{
		controller.Reset();
		controlValue.isRestet = 0;
	
		

	}
	

	// TODO: Add your message handler code here and/or call default 
	RenderScene();
	CDialog::OnTimer(nIDEvent);
}

BOOL CBalanceStrategyDlg::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 0;
	pixelDesc.cRedShift = 0;
	pixelDesc.cGreenBits = 0;
	pixelDesc.cGreenShift = 0;
	pixelDesc.cBlueBits = 0;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 0;
	pixelDesc.cAccumRedBits = 0;
	pixelDesc.cAccumGreenBits = 0;
	pixelDesc.cAccumBlueBits = 0;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 0;
	pixelDesc.cStencilBits = 1;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	PixelFormat = ChoosePixelFormat(hDC, &pixelDesc);
	if (PixelFormat == 0) // Choose default 
	{
		PixelFormat = 1;
		if (DescribePixelFormat(hDC, PixelFormat,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{
			return FALSE;
		}
	}

	if (SetPixelFormat(hDC, PixelFormat, &pixelDesc) == FALSE)

	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBalanceStrategyDlg::CreateViewGLContext(HDC hDC)
{
	hrenderRC = wglCreateContext(hDC);

	if (hrenderRC == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, hrenderRC) == FALSE)
		return FALSE;



	return TRUE;
}

void CBalanceStrategyDlg::RenderScene()
{
	/////////TODO 每次更新时 绘制下图
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(controller.bodypos.getX(), controller.bodypos.getY()+0.78, controller.bodypos.getZ() + CAMERA_DISTANCE,
		controller.bodypos.getX(), controller.bodypos.getY() + 0.78, controller.bodypos.getZ(),0.0, 1.0, 0.0);
	
	
	// 根据xrot,yrot,zrot旋转场景
	glRotated(0, 1.0, 0.0, 0.0);
	glRotated(-90, 0.0, 1.0, 0.0);
	glRotated(0, 0.0, 0.0, 1.0);

	drawXYZ();  //绘制xyz轴
	drawSrcs();  //绘制小人关节节点

	
	//绘制质心和脚踝运动轨迹
	if (controlValue.showCOMTrategy == 1)
	{   glColor3f(1.0f, 0.0f, 0.0f);//红色
	    std::vector<Point>::iterator iter;
		for (iter = controller.markAnkleProfile.begin(); iter != controller.markAnkleProfile.end(); iter++)
		{
			glPushMatrix();
			glTranslatef(iter->px, iter->py, iter->pz);
			glutWireSphere(0.025, 100, 100);
			glPopMatrix();
		}
		for (iter = controller.markCOMProfile.begin(); iter != controller.markCOMProfile.end(); iter++)
		{
			glPushMatrix();
			glTranslatef(iter->px, iter->py, iter->pz);
			glutWireSphere(0.025, 100, 100);
			glPopMatrix();
		}
	}
	
	
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	glColor3f(0.5, 1.0, 1.0);
	controller.humanAnimator.draw();

	if (controlValue.showSkin == 1)
	{
		controller.hskin.update(&controller.hskel);
		controller.hskin.draw();// Draw the skeleton
	}

	glPopMatrix();
	glFlush();
	
	SwapBuffers(hrenderDC);
}



BOOL CBalanceStrategyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (wParam) {
	case IDC_RADIO9:
		controlValue.stategyToTake = HIP;
		break;
	case IDC_RADIO10:
		controlValue.stategyToTake = ANKLE;
		break;
	case IDC_RADIO11:
		controlValue.stategyToTake = STEP;
		break;
	case IDC_RADIO12:
		controlValue.stategyToTake = AUTO;
		break;
	case IDC_RADIO5:
		controlValue.showCOMTrategy = 1;
		break;
	case IDC_RADIO6:
		controlValue.showCOMTrategy = 0;
		break;
	case IDC_RADIO7:
		controlValue.showSkin = 1;
		break;
	case IDC_RADIO8:
		controlValue.showSkin = 0;
		break;
	case IDC_BUTTON1:   //运行按钮
	{
		controller.humanData.height= m_height;
	    controller.humanData.legLen= m_legLen;
	    controller.humanData.lf= m_lf;
	    controller.humanData.maxSteps= m_maxStep;
	    controller.humanData.rf= m_rf;
	    controller.humanData.stepTime= m_stepTime;
		controller.humanData.Qmax = m_Qmax;
		controller.humanData.sampleTime= m_sampleTime;
		UpdateData(true);
		controlValue.initialRate = m_speed;
		if (0 == controlValue.initialRate) {
			MessageBox(TEXT("please input the initialRate"),TEXT("Warning"),MB_OK);
			m_run.EnableWindow(0);
		}
		else {
			controlValue.isRun = 1; //下次调用OnTimer()时,开始调用策略恢复
			controller.isFinished = false;
			lineSeries = (CSeries)m_chart1.Series(0);
			lineSeries2 = (CSeries)m_chart2.Series(0);

			lineSeries.Clear();
			lineSeries2.Clear();
		}
			
		break;
	}
		//TODO: 若初速度未设置，策略未选择等情况(2种方法选其一)：
		 //  1.s可弹出弹框，提示输入速度，选择策略等；
		 //  2.按钮置灰，等上述选择完成，点击该按钮才生效
	case IDC_BUTTON2:
		m_speed = 0;
		controlValue.isRestet = 1;
		for (int i = 0; i<3; i++) {
			CButton* radio = (CButton*)GetDlgItem(default_radio[i]);
			radio->SetCheck(1);
		}
		for (int i = 0; i<5; i++) {
			CButton* radio = (CButton*)GetDlgItem(clear_radio[i]);
			radio->SetCheck(0);
		}
		UpdateData(false);
		break;
	case IDC_BUTTON3:  //暂停按钮
		
		break;
	default:
		break;
		}
	
	return CDialogEx::OnCommand(wParam, lParam);
}




void CBalanceStrategyDlg::OnKillfocusEdit1()
{
	UpdateData(true);
	if (m_speed >= 0)
		m_run.EnableWindow(1);
	// TODO: 在此添加控件通知处理程序代码
}



void CBalanceStrategyDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
 