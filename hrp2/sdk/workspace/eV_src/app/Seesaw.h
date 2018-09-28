/******************************************************************************
 *	LineTracer.h (for LEGO Mindstorms EV3)
 *	Created on: 2018/08/14
 *	Definition of the Class LineTracer
 *	Author: Kazuhiro Kawachi
 *	Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_SEESAW_H_
#define EV3_APP_SEESAW_H_

#include "ev3api.h"
#include "GyroSensor.h"
#include "Odmetry.h"
#include "BalancingWalker.h"
#include "TailMotor.h"
#include "LineTracer.h"
#include "AttitudeControl.h"
#include "PidController.h"
#include "GyroSensor.h"
#include "balancer.h"


/* ---------------------------- */
/*	定数定義					*/
/* ---------------------------- */
#define SEESAW_ALLIVAL		   (0x01)						/* シーソー到着 					*/
#define SEESAW_GO_ON		   (0x02)						/* シーソー乗った 					*/
#define SEESAW_GO_OFF		   (0x03)						/* シーソー降りた 					*/

#define SEESAW_ADVANCE		   (0x3C)						/* シーソー前進走行	 				*/
#define SEESAW_BACK 		   (0xA5)						/* シーソー後退走行 				*/
#define SEESAW_BRAKE		   (0xFF)						/* シーソーブレーキ 				*/

#define SEESAW_DISTANCE_ONE    (5 / 0.95)					/* [LSB cm]シーソー制御切り替え位置 */
#define SEESAW_DISTANCE_TWO    (30 / 0.95)					/* [LSB cm]シーソー制御切り替え位置 */
#define SEESAW_DISTANCE_THREE  (50 / 0.95)					/* [LSB cm]シーソー制御切り替え位置 */
#define SEESAW_DISTANCE_END    (60.5 / 0.95)				/* [LSB cm]シーソー制御切り替え位置 */

#define SEESAW_GYRO_VAL 	   (180)						/* シーソー制御ジャイロ値			*/
#define SEESAW_GYRO_SAMPUL	   (10)							/* ジャイロ値サンプル数 			*/

#define SEESAW_STOP_TIME	   (5000 / 4)					/* 停止時間				 			*/


/* ---------------------------- */
/*	クラス定義					*/
/* ---------------------------- */
class Seesaw
{
	public:

		Seesaw(AttitudeControl* attitudecontrol, BalancingWalker* balancingwalker, LineTracer* linetracer, PidController* pidController, \
				LineMonitor* lineMonitor, ev3api::GyroSensor& gyroSensor, Odmetry* odmetry, ev3api::ColorSensor& colorsensor);
		~Seesaw();
		void Initialize();									/* 初期化処理					*/
		void State();										/* シーソー状態判定				*/
		void Run_Manage();									/* シーソー走行切り替え制御		*/
		void Run_Advance();									/* シーソー前進走行				*/
		void Run_Back();									/* シーソー後退走行				*/
		void Run_Brake();									/* シーソーブレーキ				*/

	private:

		AttitudeControl* mAttitudeControl;
		BalancingWalker* mBalancingWalker;
		LineTracer* 	 mLineTracer;
		PidController*	 mPidController;
		TailMotor*		 mTailMotor = TailMotor::getInstance();
		LineMonitor*	 mLineMonitor;
		ev3api::GyroSensor& mGyroSensor;
		Odmetry*		 mOdmetry = Odmetry::getInstance();
		ev3api::ColorSensor&	 mColorSensor;

		ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
		ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();

		int SeesawDistance;									/* 走行距離						*/
		int16_t SeesawDirection;							/* 旋回値						*/
		unsigned char SeesawPosition;						/* 現在位置						*/
		unsigned char SeesawRunMode;						/* 走行モード					*/
		int16_t GyroVal[SEESAW_GYRO_SAMPUL];				/* ジャイロセンサ値				*/
		unsigned char GyroValPoint;							/* ジャイロセンサ値ポインタ		*/
		int GyroValTotal;									/* ジャイロセンサ積算値			*/
		int StayTimer;										/* 通常走行停止タイマ			*/
		unsigned char InitFlag;								/* 初期化設定フラグ 			*/
};


#endif /*  EV3_APP_SEESAW_H_ */
