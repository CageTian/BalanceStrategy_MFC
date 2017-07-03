================================================================================
 BalanceControl.h  BalanceControl.cpp 平衡控制器
 对外接口：    initControl();    平衡控制器的初始化工作  在CBalanceStrategyDlg::OnInitDialog()调用
			   RecoverBalance(): 平衡恢复函数    MFC界面选择好参数后调用该函数进行平衡恢复     
其他添加的.h .cpp文件对MFC界面框架透明，可看作没有    
================================================================================
 BalanceStrategyDlg.h
 主要添加：   
 1.结构体 ControllValue： MFC界面的选择结果将存储在该结构体
  模型状态选择  ：				 bool isStatic = 0;
  受扰初速度大小：				 double initialRate=0;
  需要采取的策略：			     Strategy stategyToTake = Strategy::AUTO;  
  是否显示质心轨迹：             bool showCOMTrategy=0;        绘制函数RenderScene()中会根据参数值选择相应输出
  是否显示皮肤：				 bool showSkin=0;              绘制函数RenderScene()中会根据参数值选择相应输出
  是否已点击运行or暂停按钮：     bool isRun = 0;               OnTimer()根据参数值进行相应控制
  是否已点击重置按钮：			 bool isRestet = 0;            与平衡控制器无关
  是否已点击清空按钮：           bool isEmpty = 0;			   与平衡控制器无关

 ================================================================================
  BalanceStrategyDlg.cpp
   主要修改：
   1.void RenderScene() :绘制小人模型场景 
         ！！！注意查看该函数中的语句：if(controlValue.showCOMTrategy == 1) 	 if (controlValue.showSkin == 1)
   2.void OnTimer()  : 调用 策略恢复类BalanceControl RecoveryBalance()函数

   主要添加:
   1.重载 bool OnCommand(WPARAM wParam, LPARAM lParam) 用于对点击按钮事件做出相应
=============================================================================