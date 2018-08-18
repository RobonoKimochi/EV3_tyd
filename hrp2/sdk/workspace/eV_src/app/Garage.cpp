#include "Garage.h"


/**
 * コンストラクタ
 */
Garage::Garage(	BalancingWalker* balancingWalker,
				LineTracer* lineTracer,
				PidController* pidController,
				LineMonitor* lineMonitor)
    :   mBalancingWalker( balancingWalker ),
    	mLineTracer( lineTracer ),
    	mPidController( pidController ),
    	mLineMonitor( lineMonitor ),
    	mTailAngle( 80 ),			// 尻尾目標角度80°
    	mForward( 5 ),				// PID制御値
    	mGaragePoint( 40 ),			// 停止させる地点までの距離
    	mAdjustSpeed( -1 ),			// 調整時の移動速度
    	mState( INIT )				// 初期状態
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
void Garage::StateGarage()
{
	// 状態に応じた処理を実行
	switch( mState ){
	case INIT:
		this->Initialize();			// 初期化処理
		break;
	case WALKING:
		this->GoToGarage();			// ガレージ内の停止位置まで走行処理関数
		break;
	case ADJUST:
		this->Adjust();				// 調整止処理
		break;
	case STOP:
		this->Stop();				// 停止処理
	default:
		break;
	}
}

/**
* @brief 			初期化処理
*
* @param			無し
*
* @return 			無し
*
* @detail 			ガレージ処理開始時をRunManagerの原点とし、
*					ガレージ内の停止位置まで走行状態遷移
*/
void Garage::Initialize()
{
	mRunManager->setOrigin();					// 原点を設定（停止地点までの距離判断に使用）

	mLineMonitor->setLineThreshold( 10 );

	mPidController->setPID( 2, 0, 5 );

	mState = WALKING;							// 状態遷移：ガレージまで進行
}

/**
* @brief 			ガレージ内の停止位置まで走行処理
*
* @param			無し
*
* @return 			無し
*
* @detail 			停止地点（mGaragePoint）までライントレース制御で進行。
*					停止地点到達後、停止前調整処理に遷移。
*/
void Garage::GoToGarage()
{
	if( mRunManager->getDistanceFromOrigin() <= mGaragePoint )	// 停止地点まで進んでない？
	{
		// 決め打ちの値（黒：0、白：10、トレース値：5）でPID制御をさせる
		// 右車輪にはmForward(決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を引いた値。
		// 左車輪にはmForward（決め打ちの前進値）から現在のカラーセンサ値とライン閾値の差分を加えた値。
		mRightWheel.setPWM( mForward - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
		mLeftWheel.setPWM( mForward + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
	}
	else	// 停止距離まで通過
	{
		mState = ADJUST;			// 状態遷移：停止調整処理
	}
}

/**
* @brief 			停止前調整処理
*
* @param			無し
*
* @return 			無し
*
* @detail 			急停止時の転倒を防止するための調整処理を行う。
*/
void Garage::Adjust()
{
	// 急停止時の前方転倒を防止のため、後方に車輪を回す。
	mRightWheel.setPWM( mAdjustSpeed );		// 右車輪後退方向に回す
	mLeftWheel.setPWM( mAdjustSpeed );		// 左車輪後退方向に回す

	// 車輪を急停止させないためブレーキ解除
	mRightWheel.setBrake( false );	// 右車輪ブレーキ状態
	mLeftWheel.setBrake( false );	// 左車輪ブレーキ状態

//	mTailMotor->setAngle( mTailMotor->getAngle() + 0 );	// 走行体の傾きを緩やかにする

	mState = STOP;					// 状態遷移：停止
}

/**
* @brief 			停止処理
*
* @param			無し
*
* @return 			無し
*
* @detail 			ガレージ停止後の処理。
*/
void Garage::Stop()
{
	mRightWheel.setPWM( 0 );		// 右車輪停止
	mLeftWheel.setPWM( 0 );			// 左車輪停止

	mRightWheel.setBrake( true );	// 右車輪ブレーキ状態
	mLeftWheel.setBrake( true );	// 左車輪ブレーキ状態
}
