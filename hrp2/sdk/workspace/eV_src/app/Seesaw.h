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

/********************************/
/*	クラス定義					*/
/********************************/
class Seesaw
{
	public:

		Seesaw(AttitudeControl* attitudecontrol, BalancingWalker* balancingwalker, LineTracer* linetracer, PidController* pidController, \
				LineMonitor* lineMonitor, ev3api::GyroSensor& gyroSensor, Odmetry* odmetry);
		~Seesaw();
		void Initialize();		/* 初期化処理	*/
		void Seesaw_State();
		void Run();				/* シーソー走行 */

	private:

		AttitudeControl* mAttitudeControl;
		BalancingWalker* mBalancingWalker;
		LineTracer* 	 mLineTracer;
		PidController*	 mPidController;
		TailMotor*		 mTailMotor = TailMotor::getInstance();
		LineMonitor*	 mLineMonitor;
		ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
		ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
		ev3api::GyroSensor& mGyroSensor;
		Odmetry*		 mOdmetry;

		int SeesawDistance;
		char SeesawPosition;
		const int Seesaw_Forward = 10;


};


/********************************/
/*	定数定義					*/
/********************************/
#define SEESAW_ALLIVAL		 (1)							/* シーソー到着 					*/
#define SEESAW_DISTANCE 	 (25 / 0.70)					/* [LSB cm]シーソー制御切り替え位置 */
#define SEESAW_GYRO_VAL 	 (150)							/* シーソー制御ジャイロ値			*/


#endif /*  EV3_APP_SEESAW_H_ */
