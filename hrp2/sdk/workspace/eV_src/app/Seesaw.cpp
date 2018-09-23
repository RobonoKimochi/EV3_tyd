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
			SeesawDirection(0),
			SeesawPosition(0),
			SeesawRunMode(0),
			GyroValTotal(0)
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
	/*	シーソー判定			*/
	/* ------------------------ */
	for(i = 0; i < (SEESAW_GYRO_SAMPUL - 1); i++)
	{
		GyroVal[i+1] = GyroVal[i];
		GyroValTotal +=  GyroVal[i];
	}

	GyroVal[0] = mGyroSensor.getAnglerVelocity();							/* ジャイロ値取得			*/
	GyroValTotal = (GyroValTotal / SEESAW_GYRO_SAMPUL);

	if (GyroValTotal > SEESAW_GYRO_VAL)
	{
		SeesawDistance = mOdmetry->getX();									/* シーソー位置X座標ラッチ	*/
		SeesawPosition = SEESAW_ALLIVAL;
		SeesawRunMode  = SEESAW_ADVANCE;
	}
	else if (GyroValTotal <= (SEESAW_GYRO_VAL / 4))
	{
		SeesawPosition = SEESAW_GO_ON;
	}
	else
	{
		/* No Operation */
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

	if (SeesawPosition == SEESAW_ALLIVAL)
	{
		mBalancingWalker->setCommand(-60, 0);
		ev3_led_set_color(LED_ORANGE); 										/* 通常走行は橙				*/
		mTailMotor->setAngle(0);											/* シーソー走行時 尻尾0°	*/

		StayTimer = 0;
	}
	else if (  (SeesawPosition == SEESAW_GO_ON)
			&& (now_distance > SeesawDistance))
	{
		if (   ((now_distance - SeesawDistance) > SEESAW_DISTANCE_ONE)			/* シーソー位置判定			*/
			&& ((now_distance - SeesawDistance) < SEESAW_DISTANCE_LAST))
		{
			mBalancingWalker->setCommand(30, SeesawDirection);
			mTailMotor->setAngle(0);											/* シーソー走行時 尻尾0°	*/

			ev3_led_set_color(LED_RED); 										/* 通常走行は赤				*/
		}
		else if ((now_distance - SeesawDistance) > SEESAW_DISTANCE_LAST)		/* シーソー位置判定			*/
		{
			mBalancingWalker->setCommand(100, 0);
			ev3_led_set_color(LED_GREEN); 										/* 通常走行は赤				*/
			SeesawRunMode = SEESAW_BRAKE;
		}
		else
		{
			/* No Operation */
		}
	}
	else
	{
		if (StayTimer < (2000 / 4))
		{
			mBalancingWalker->setCommand(80, 0);
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
	if (BrakeTimer < (3000 / 4))
	{
		mBalancingWalker->setCommand(100, 0);
	}
	else
	{
		mBalancingWalker->setCommand(0, 0);
		mTailMotor->setAngle(89);											/* シーソー走行時 尻尾89°	*/
	}

	BrakeTimer++;
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
	mTailMotor->setAngle(0);												/* シーソー進入時 尻尾0°	*/
	mGyroSensor.setOffset(0);												/* ジャイロオフセット変更	*/
}

#if 0
void Seesaw::Run()
{
	static unsigned char Flag;

	int SeesawDirection = mLineTracer->calcDirection();							/* 旋回値取得				*/
	int16_t now_distance = mOdmetry->getX();								/* X座標位置取得			*/
	int16_t gyro_val = mGyroSensor.getAnglerVelocity();						/* ジャイロ値取得			*/

	mTailMotor->setAngle(0);												/* シーソー進入時 尻尾0°	*/
	mGyroSensor.setOffset(0);												/* ジャイロオフセット変更	*/

	/* ------------------------ */
	/*	シーソー判定			*/
	/* ------------------------ */
	if (gyro_val > SEESAW_GYRO_VAL)
	{
		SeesawDistance = mOdmetry->getX();									/* シーソー位置X座標ラッチ	*/
		SeesawPosition = SEESAW_ALLIVAL;
	}

	/* ------------------------ */
	/*	シーソー走行制御		*/
	/* ------------------------ */
	if (SeesawPosition == SEESAW_ALLIVAL)									/* シーソー到着				*/
	{
		if (SeesawRunMode == SEESAW_ADVANCE)
		{
			if (   ((now_distance - SeesawDistance) > SEESAW_DISTANCE_ONE)		/* シーソー位置判定			*/
				&& ((now_distance - SeesawDistance) < SEESAW_DISTANCE_DUBLE))
			{
				mBalancingWalker->setCommand(10, SeesawDirection);
				mTailMotor->setAngle(0);										/* シーソー走行時 尻尾0°	*/
				mGyroSensor.setOffset(0);									/* ジャイロオフセット変更	*/

				ev3_led_set_color(LED_RED); 									/* 通常走行は赤				*/
			}
			else if ((now_distance - SeesawDistance) > SEESAW_DISTANCE_DUBLE)
			{
				mBalancingWalker->setCommand(0, SeesawDirection);
				mBalancingWalker->setCommand(-48, 0);
				SeesawRunMode = SEESAW_BACK;

				mBalancingWalker->setCommand(10, SeesawDirection);
				mTailMotor->setAngle(0);										/* シーソー走行時 尻尾0°	*/
				mGyroSensor.setOffset(0);									/* ジャイロオフセット変更	*/

				ev3_led_set_color(LED_RED); 									/* 通常走行は赤				*/
			}
			else
			{
				mBalancingWalker->setCommand(80, 0);
				ev3_led_set_color(LED_ORANGE); 									/* 通常走行は橙				*/

				if (Flag == 0)
				{
					mGyroSensor.setOffset(-25);									/* ジャイロオフセット変更	*/
					Flag ^= 1;
				}
				else
				{
					mGyroSensor.setOffset(0);									/* ジャイロオフセット変更	*/
					Flag ^= 1;
				}
			}
		}
		else if (SeesawRunMode == SEESAW_BACK)
		{
			if (   ((now_distance - SeesawDistance) > SEESAW_DISTANCE_ONE)		/* シーソー位置判定			*/
				&& ((now_distance - SeesawDistance) < SEESAW_DISTANCE_DUBLE))
			{
				mBalancingWalker->setCommand(-10, (-1 * SeesawDirection));
				mTailMotor->setAngle(0);										/* シーソー走行時 尻尾0°	*/
				ev3_led_set_color(LED_RED); 									/* 通常走行は赤				*/
			}
			else if ((now_distance - SeesawDistance) < SEESAW_DISTANCE_ONE)
			{
				mBalancingWalker->setCommand(10, SeesawDirection);
				SeesawRunMode = SEESAW_FINISH;
			}
			else
			{
				mBalancingWalker->setCommand(-10, (-1 * SeesawDirection));
				mTailMotor->setAngle(0);										/* シーソー走行時 尻尾0°	*/
				ev3_led_set_color(LED_RED); 									/* 通常走行は赤				*/
			}
		}
		else
		{
			mBalancingWalker->setCommand(10, SeesawDirection);
			mTailMotor->setAngle(0);										/* シーソー走行時 尻尾0°	*/
		}
	}
	else
	{
		mBalancingWalker->setCommand(60, SeesawDirection);
	}

	/* ------------------------ */
	/*	走行パターン切り替え	*/
	/* ------------------------ */
	if (SeesawRunNoBalance == SEESAW_ALLIVAL)
	{
		mBalancingWalker->runWithoutBalancing();
	}
	else
	{
		mTailMotor->moveTail();
	}
	mTailMotor->moveTail();
	mBalancingWalker->run();
}
#endif
