#include "TailMotor.h"
#include <math.h>

/**
 * コンストラクタ
 */

TailMotor::TailMotor()
	: //mTailMotor(tailMotor),
	  mAngle(0),
	  mOffset(0),
	  count(0),
	  mPWM(0),
	  mP_Gain(0.50F),
	  mI_Gain(0.10F),
	  PWM_MAX(60),
	  preTargetAngle(0),
	  TimeCount(0),
      TargetAngle(0)
	  {
}

/**
 * @brief 			テールモータの初期化処理
 *
 * @param [IN]		offset：テールモータオフセット値(初期値)
 *
 * @return 			無し
 *
 * @detail 			テールモータのオフセット値の設定、制御の初期化処理を行う。
 */
void TailMotor::init( int32_t offset )
{
	mTailMotor.reset();	// モータリセット
	mOffset = offset;	// テールモータのオフセット値設定

	return;				// 【戻り値】無し
}

/**
 * @brief 			テールモータ比例制御ゲインの設定
 *
 * @param [IN]		p_gain：テールモータ比例制御ゲイン設定値
 *
 * @return 			無し
 *
 * @detail 			テールモータ制御の比例値の設定を行う。
 */
void TailMotor::setPgain( float p_gain )
{
	mP_Gain = p_gain;	// テールモータのオフセット値設定

	return;				// 【戻り値】無し
}

/**
 * @brief			テールモータの制御
 *
 * @param angle		テールの目標角度
 *
 * @return			無し
 *
 * @detail			引数「angle」からテイルモータの目標角度を設定
 */

void TailMotor::setAngle( int32_t angle )
{
	mAngle = angle;
	return;	// 【戻り値】無し
}

/**
 * @brief			テイルモータの制御
 *
 * @param 			無し
 *
 * @return			無し
 *
 * @detail			テイルモータに設定するPWM制御を現在のmAngleから算出し
 * 					制御PWM最大値以上なら最大値、最小値以下なら最小値に修正後
 *					テイルモータにPWM制御値を与える。
 */

void TailMotor::moveTail()
{
	float s_err;	// 偏差
	float s_p,s_i;
	static float s_integral,s_preerr;
	
	s_err =  mAngle + mOffset - mTailMotor.getCount()  ;	
	
	s_p = s_err * mP_Gain;
	s_i = s_integral * mI_Gain;
	
#ifdef USE_PI
	mPWM = s_p + s_i;
#else
	mPWM = s_p ;
#endif
	
	// 累積積分
	s_preerr = s_err;
	s_integral = s_integral + (s_err + s_preerr) / 2.0 * 0.004;
	
	// 目標に達したら累積積分リセット
	if ( fabs(s_err) < 2.0f)
	{
		s_integral = 0;
	}

//	mPWM = (float)(( mAngle + mOffset - mTailMotor.getCount() ) * mP_Gain );	// 比例制御
	
	// PWM出力飽和処理
	if( mPWM > PWM_MAX )		// 制御PWM絶対最大値より大きい？
	{
		mPWM = PWM_MAX;			// 最大値設定(最大値を超えさせない)
	}
	else if( mPWM < -PWM_MAX )	// 制御PWM絶対最大値より小さい？
	{
		mPWM = -PWM_MAX;		// 最小値設定(最小値未満にさせない)
	}

	mTailMotor.setPWM( mPWM );	// テイルモータのPWM制御値を設定

	return;	// 【戻り値】無し
}

void TailMotor::setTargetAngle(int target)
{
	
	TargetAngle = target;
	
	return;
}

void TailMotor::setAngleSlow()
{
	if(preTargetAngle != TargetAngle) {
		SetAngle = mTailMotor.getCount();
	}
	if (SetAngle > TargetAngle){
		
    	if (TimeCount > 29){
    		SetAngle--;
    		TimeCount = 0;
    	}
    	TimeCount++;
	} else if( SetAngle < TargetAngle) {
		
		if (TimeCount > 29){
    		SetAngle++;
    		TimeCount = 0;
    	}
    	TimeCount++;
	}
	mAngle = SetAngle;
	preTargetAngle = TargetAngle;
#if(0)
	mPWM = (float)(( SetAngle + mOffset - mTailMotor.getCount() ) * mP_Gain );	// 比例制御
	
	// PWM出力飽和処理
	if( mPWM > PWM_MAX )		// 制御PWM絶対最大値より大きい？
	{
		mPWM = PWM_MAX;			// 最大値設定(最大値を超えさせない)
	}
	else if( mPWM < -PWM_MAX )	// 制御PWM絶対最大値より小さい？
	{
		mPWM = -PWM_MAX;		// 最小値設定(最小値未満にさせない)
	}

	
	mTailMotor.setPWM( mPWM );	// テイルモータのPWM制御値を設定
#endif
	return;	// 【戻り値】無し
}

float TailMotor::getDiff(){
	
	return(mP_Gain - mTailMotor.getCount()) ;
}


bool TailMotor::Complete(){
	
	if (mPWM < 1.0f) {
		count++;
	} else {
		count = 0;
	}
	
	if (count >20) {
		flag = true;
	} else {
		flag = false;
	}
	return(flag) ;
}

bool TailMotor::CompleteDull(){
	
	if (ABS(TargetAngle - mTailMotor.getCount()) <=  2.0f) {
		count++;
	} else {
		count = 0;
	}
	
	if (count >80) {
		flag = true;
	} else {
		flag = false;
	}
	return(flag) ;
}

/**
 * @brief			テールモータの角度取得
 *
 * @param angle		無し
 *
 * @return			テ－ルモータの角度
 *
 * @detail			現在のテールモータの角度を戻り値として返す。
 */

int32_t TailMotor::getAngle()
{
	return	mTailMotor.getCount();	// 【戻り値】テールモータの角度
}

void TailMotor::setPWM(int ang)
{
	mTailMotor.setPWM(ang);	// 【戻り値】テールモータの角度
}

//以下検討中
//bool TailMotor::KeepTorque(){
//
//	
//	mTailMotor.setPWM( mPWM );
//	return() ;
//}