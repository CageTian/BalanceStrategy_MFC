================================================================================
 BalanceControl.h  BalanceControl.cpp ƽ�������
 ����ӿڣ�    initControl();    ƽ��������ĳ�ʼ������  ��CBalanceStrategyDlg::OnInitDialog()����
			   RecoverBalance(): ƽ��ָ�����    MFC����ѡ��ò�������øú�������ƽ��ָ�     
������ӵ�.h .cpp�ļ���MFC������͸�����ɿ���û��    
================================================================================
 BalanceStrategyDlg.h
 ��Ҫ��ӣ�   
 1.�ṹ�� ControllValue�� MFC�����ѡ�������洢�ڸýṹ��
  ģ��״̬ѡ��  ��				 bool isStatic = 0;
  ���ų��ٶȴ�С��				 double initialRate=0;
  ��Ҫ��ȡ�Ĳ��ԣ�			     Strategy stategyToTake = Strategy::AUTO;  
  �Ƿ���ʾ���Ĺ켣��             bool showCOMTrategy=0;        ���ƺ���RenderScene()�л���ݲ���ֵѡ����Ӧ���
  �Ƿ���ʾƤ����				 bool showSkin=0;              ���ƺ���RenderScene()�л���ݲ���ֵѡ����Ӧ���
  �Ƿ��ѵ������or��ͣ��ť��     bool isRun = 0;               OnTimer()���ݲ���ֵ������Ӧ����
  �Ƿ��ѵ�����ð�ť��			 bool isRestet = 0;            ��ƽ��������޹�
  �Ƿ��ѵ����հ�ť��           bool isEmpty = 0;			   ��ƽ��������޹�

 ================================================================================
  BalanceStrategyDlg.cpp
   ��Ҫ�޸ģ�
   1.void RenderScene() :����С��ģ�ͳ��� 
         ������ע��鿴�ú����е���䣺if(controlValue.showCOMTrategy == 1) 	 if (controlValue.showSkin == 1)
   2.void OnTimer()  : ���� ���Իָ���BalanceControl RecoveryBalance()����

   ��Ҫ���:
   1.���� bool OnCommand(WPARAM wParam, LPARAM lParam) ���ڶԵ����ť�¼�������Ӧ
=============================================================================