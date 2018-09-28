#include "Seesaw.h"


/**
 * コンストラクタ
 */
Seesaw::Seesaw(AttitudeControl* attitudeControl, BalancingWalker* balancingwalker, LineTracer* linetracer, PidController* pidController, \
				LineMonitor* lineMonitor, ev3api::GyroSensor& gyroSensor, Odmetry* odmetry, ev3api::ColorSensor& colorsensor)
		:  	mAttitudeControl(attitudeControl),
			mBalancingWalker(balancingwalker),
			mLineTracer(linetracer),
			mPidController(pidController),
			mLineMonitor(lineMonitor),
			mGyroSensor(gyroSensor),
			mOdmetry(odmetry),
			mColorSensor(colorsensor),
			SeesawDistance(0),
			SeesawDirection(0),
			SeesawPosition(0),
			SeesawRunMode(0),
			GyroValTotal(0),
			StayTimer(0),
			InitFlag(0)
{

}


/**
 * デストラクタ
 */
Seesaw::~Seesaw()
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
	mTailMotor->setAngle(0);											/* シーソー走行時 尻尾0°	*/
	mPidController->setPID(0.2, 0, 9.0);


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
void Seesaw::State()
{
	unsigned char i;

	/* ------------------------ */
	/*	旋回値取得				*/
	/* ------------------------ */
	SeesawDirection = mLineTracer->calcDirection();							/* 旋回値取得				*/

	/* ------------------------ */
	/*	ジャイロ値積算			*/
	/* ------------------------ */
	if (GyroValPoint > SEESAW_GYRO_SAMPUL)
	{
		GyroValPoint = 0;
	}

	GyroVal[GyroValPoint] = mGyroSensor.getAnglerVelocity();				/* ジャイロ値取得			*/
	GyroValPoint++;

	for(i = 0; i < (SEESAW_GYRO_SAMPUL - 1); i++)
	{
		GyroValTotal +=  GyroVal[i];
	}

	GyroValTotal = (GyroValTotal / SEESAW_GYRO_SAMPUL);

	/* ------------------------ */
	/*	ジャイロ値判定			*/
	/* ------------------------ */
	if (	(SeesawPosition != SEESAW_GO_ON)
		&&	(	(GyroValTotal < - SEESAW_GYRO_VAL)
			 || (GyroValTotal > SEESAW_GYRO_VAL)))
	{
		SeesawPosition = SEESAW_ALLIVAL;
		SeesawRunMode  = SEESAW_ADVANCE;
		SeesawDistance = mOdmetry->getX();									/* シーソー位置X座標ラッチ	*/
	}
}


/**
* @brief 			シーソー前進走行制御
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/


void Seesaw::Run_Manage()
{
	if (SeesawRunMode == SEESAW_ADVANCE)
	{
		Run_Advance();
	}
	else if (SeesawRunMode == SEESAW_BACK)
	{
		Run_Back();
	}
	else if (SeesawRunMode == SEESAW_BRAKE)
	{
		Run_Brake();
	}
	else
	{
		mBalancingWalker->setCommand(20, SeesawDirection);
	}
	mTailMotor->moveTail();
	mBalancingWalker->run();
}


/**
* @brief 			シーソー前進走行制御
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/
void Seesaw::Run_Advance()
{
	int16_t now_distance = mOdmetry->getX();								/* X座標位置取得			*/

	/* ------------------------ */
	/*	シーソー上りはじめ		*/
	/* ------------------------ */
	if (SeesawPosition == SEESAW_ALLIVAL)
	{
		SeesawPosition = 0x00;
		mBalancingWalker->setCommand(-60, 0);
		ev3_led_set_color(LED_ORANGE); 										/* 通常走行は橙				*/

		StayTimer = 0;
	}
	else if (  ((now_distance - SeesawDistance) > SEESAW_DISTANCE_ONE)
			&& ((now_distance - SeesawDistance) < SEESAW_DISTANCE_TWO))
		{
		SeesawPosition = SEESAW_GO_ON;
			mBalancingWalker->setCommand(30, SeesawDirection);
		mTailMotor->setAngle(85);											/* シーソー走行時 尻尾0°	*/

			ev3_led_set_color(LED_RED); 										/* 通常走行は赤				*/

		if ((now_distance - SeesawDistance) > SEESAW_DISTANCE_TWO)			/* シーソー位置判定			*/
		{
			SeesawRunMode = SEESAW_BRAKE;
		}
		else
		{
			/* No Operation */
		}
	}
	else
	{
		if (StayTimer < (4000 / 4))
		{
			mBalancingWalker->setCommand(75, 0);
		}
		else
		{
			mBalancingWalker->setCommand(20, SeesawDirection);
		}

		StayTimer++;
	}
}


/**
* @brief 			シーソーブレーキ制御
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/
void Seesaw::Run_Brake()
{
	int16_t now_distance = mOdmetry->getX();								/* X座標位置取得			*/
#if 0
	mGyroSensor.setOffset(-15);
#endif

	if ((now_distance - SeesawDistance) > SEESAW_DISTANCE_END)
	{
		while(1)
		{
			mRightWheel.setPWM(0);
			mLeftWheel.setPWM(0);

			mBalancingWalker->setCommand(0, SeesawDirection);
			mTailMotor->setAngle(80);										/* シーソー走行時 尻尾0°	*/
			mTailMotor->moveTail();
			mBalancingWalker->run();
		}
	}
}


/**
* @brief 			シーソー後退走行制御
*
* @param			無し
*
* @return 			無し
*
* @detail
*
*/
void Seesaw::Run_Back()
{
	mBalancingWalker->setCommand(-50, 0);
}


