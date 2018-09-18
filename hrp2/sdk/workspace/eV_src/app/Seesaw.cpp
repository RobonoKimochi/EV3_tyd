#include "Seesaw.h"


/**
 * コンストラクタ
 */
Seesaw::Seesaw(AttitudeControl* attitudeControl, BalancingWalker* balancingwalker, LineTracer* linetracer, PidController* pidController, \
				LineMonitor* lineMonitor, ev3api::GyroSensor& gyroSensor, Odmetry* odmetry)
		:  	mAttitudeControl(attitudeControl),
			mBalancingWalker(balancingwalker),
			mLineTracer(linetracer),
			mPidController(pidController),
			mLineMonitor(lineMonitor),
			mGyroSensor(gyroSensor),
			mOdmetry(odmetry),
			SeesawDistance(0),
			SeesawPosition(0)
{

}


/**
 * デストラクタ
 */
Seesaw::~Seesaw()
{

}


/**
* @brief 			状態に対応した処理を実行。
*
* @param			無し
*
* @return 			無し
*
* @detail 			状態に応じた処理関数を呼び出す。
*/
void Seesaw::Seesaw_State()
{

}


/**
* @brief 			初期化処理
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/
void Seesaw::Initialize()
{
	ev3_led_set_color(LED_RED); /* for debug */
	mTailMotor->setAngle(80);	// 開始時尻尾80°

#if 0
	mLeftWheel.setPWM(10);
	mRightWheel.setPWM(10);
#endif
}


/**
* @brief 			シーソー走行制御
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/
void Seesaw::Run()
{
	int16_t now_distance;
	int direction = mLineTracer->calcDirection();
#if 0
	int16_t gyro_val = mGyroSensor.getAnglerVelocity();
#endif
int16_t gyro_val;
	if (gyro_val > SEESAW_GYRO_VAL)
	{
		SeesawDistance = mOdmetry->getX();
		SeesawPosition = SEESAW_ALLIVAL;
	}

	now_distance = mOdmetry->getX();

	if (   ((now_distance - SeesawDistance) > SEESAW_DISTANCE)
		&& (SeesawPosition == SEESAW_ALLIVAL))
	{
		mBalancingWalker->setCommand(10, direction);
	}
	else
	{
		mBalancingWalker->setCommand(25, direction);
	}

	mTailMotor->moveTail();
	mBalancingWalker->run();

}

/* for_buckup */
#if 0
void Seesaw::Run()
{
	int direction = mLineTracer->calcDirection();
#if 0

	if ( Seesaw_Timer <= 50 )
	{
		mBalancingWalker->setCommand(25, direction);
	}
	else
	{
		mBalancingWalker->setCommand(-10, direction);

		if ( Seesaw_Timer == 100 )
		{
			Seesaw_Timer = 0;
		}
	}
#endif

	mTailMotor->moveTail();
	mBalancingWalker->setCommand(25, direction);
	mBalancingWalker->run();

	Seesaw_Timer++;
#if 0
	/* (void)mAttitudeControl->Stand(); */
	mTailMotor->moveTail();
	mBalancingWalker->setCommand(-10, 0);	//■■速度は暫定
	mBalancingWalker->setCommand(10, 0);
	mBalancingWalker->run();
	mLeftWheel.setPWM(10);
	mRightWheel.setPWM(10);
	mRightWheel.setPWM(Seesaw_Forward - mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
	mLeftWheel.setPWM(Seesaw_Forward + mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()));
		SeesawDistance = gOdmetry->getX();

	/* mLineTracer->run(); */
#endif
}
#endif

