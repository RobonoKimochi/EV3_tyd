#ifndef	EV3_UNIT_TAILMOTOR_H_
#define EV3_UNIT_TAILMOTOR_H_

// #include "Motor.h"
#include "MotorDriver.h"
#include "app.h"

class TailMotor
{
	public:
		/**
		 * インスタンスの取得
		 */
		 static TailMotor *getInstance(){
			 static TailMotor tailMotor;
			 return &tailMotor;
		 }
		// ev3api::Motor& mTailMotor;		// モータークラス

		/**
		* @brief 			テイルモータの初期化処理
		*
		* @param [IN]		offset：テイルモータオフセット値(初期値)
		*
		* @return 			無し
		*
		* @detail 			テイルモータのオフセット値の設定、制御の初期化処理を行う。
		*/
		void init( int32_t offset );

		/**
 		* @brief 			テイルモータ比例制御ゲインの設定
		*
 		* @param [IN]		p_gain：テイルモータ比例制御ゲイン設定値
 		*
 		* @return 			無し
 		*
 		* @detail 			テイルモータ制御の比例値の設定を行う。
 		*/
		void setPgain( float p_gain );

		/**
		* @brief			テイルモータの制御
		*
		* @param angle		テイルの目標角度
		*
		* @return			無し
		*
		* @detail			引数「angle」からテイルモータの目標角度を設定
		*/
		void setAngle( int32_t angle );
		
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
		void moveTail();
		void setTargetAngle(int target);

		void setAngleSlow();	// 車体傾斜用
	
		void setPWM(int ang);

	    float getDiff();
	    bool  Complete();
		int32_t  mAngle;				// モーターの角度(°)
	bool CompleteDull();
	void  setTargetAngle();
	
/**
	* @brief			テイルモータの角度取得
	*
	* @param 			無し
	*
	* @return			テイルモータの角度
	*
	* @detail			現在のテイルモータの角度を戻り値として返す。
	*/
	int32_t getAngle();
	private:
		TailMotor();					// コンストラクタ

		int32_t  mOffset;				// オフセット値
	    int      count;
		bool     flag;
		float    mPWM;					// PWM制御値
		float    mP_Gain;				// 比例制御ゲイン
		float    PWM_MAX;				// PWM制御飽和閾値
	    float    SetAngle;
		float    preTargetAngle;
	    int TimeCount;
		int    TargetAngle;

	    ev3api::Motor mTailMotor = MotorDriver::getInstance().getTailMotor();
};
#endif	// EV3_UNIT_TAILMOTOR_H_
