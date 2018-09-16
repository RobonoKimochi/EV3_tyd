#ifndef _RUN_STAIRS_H_
#define _RUN_STAIRS_H_


#include "ev3api.h"
#include "BalancerCpp.h"
#include "LineTracer.h"
#include "BalancingWalker.h"
#include "Odmetry.h"
#include "Motor.h"
#include "TailMotor.h"
#include "PidController.h"
#include "LineMonitor.h"



/********************************/
/*	クラス定義					*/
/********************************/
class Run_Stairs
{
	public:

		Run_Stairs( BalancingWalker* balancingWalker, PidController* pidController, LineMonitor* LineMonitor ) ;
		~Run_Stairs() ;
		char Stairs_Main() ;
		void Stairs_Run() ;
		void Stairs_Up() ;
		void Stairs_Stop() ;
		void Stairs_Down() ;
		void Stairs_Run_2nd() ;

	private:

	    Balancer* mBalancer ;
		LineTracer* mLineTracer ;
		BalancingWalker* mBalancingWalker ;
		Odmetry *mOdmetry = Odmetry::getInstance() ;
		ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel() ;
		ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel() ;
    	TailMotor *mTailMotor = TailMotor::getInstance() ;
		PidController* mPidController ;
		LineMonitor* mLineMonitor ;
		RunManager* mRunManager = RunManager::getInstance() ;

		bool FirstTime_Flg ;
		float First_Position ;
		unsigned char Tail_Down_Count ;
		float Now_Position ;
		char sts ;
		char Stairs_Count ;
		unsigned short Stand_Tail_Timer ;
		char Tail_Angle ;
		unsigned short Reverse_Time ;
		char Reverse_Comp ;
		char Init_Flg ;
		float Prev_Position ;
		unsigned short Stairs_Stop_Timer ;
		char Stop_Comp ;
		char Run_Ctrl ;
		char Cyc_Count ;
		char Init_Flg_2nd ;
		char Contrl_2nd ;
};


/********************************/
/*	制御関連定義				*/
/********************************/
#define STAIRS_CTRL				(0x00)										/* 階段制御中						*/
#define OTHER_CTRL				(0x01)										/* 他の制御に切り替え				*/


/********************************/
/*	階段処理定義				*/
/********************************/
#define FIRST_STAIR_OFFSET		(50)										/* １段目の距離						*/
#define SECOND_STAIR_OFFSET		(100)										/* ２段目の距離						*/


/********************************/
/*	階段時間定義				*/
/********************************/
#define CYCTIME_1S				(250)										/* １ｓ制御(4ms * 250)				*/
#define CYCTIME_2S				(500)										/* ２ｓ制御(4ms * 500)				*/
#define CYCTIME_INIT			(0)										/* タイマ初期化						*/


#endif  /* _RUN_STAIRS_H_ */
