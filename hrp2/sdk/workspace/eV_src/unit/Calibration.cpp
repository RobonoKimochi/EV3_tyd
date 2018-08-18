/******************************************************************************
 *  Calibration.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Calibration.h"


// 定数宣言
const int Calibration::NUM_OF_GYRO_CALIBRATION =  255; // ★キャリブレーション演習修正箇所
const int Calibration::NUM_OF_BLACK_CALIBRATION = 255; // ★キャリブレーション演習修正箇所
const int Calibration::NUM_OF_WHITE_CALIBRATION = 255; // ★キャリブレーション演習修正箇所

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 * @param gyroSensor  ジャイロセンサ
 * @param lineMonitor ラインモニタ
 */
Calibration::Calibration(const ev3api::ColorSensor& colorSensor,
                  ev3api::GyroSensor& gyroSensor,
                  LineMonitor* lineMonitor)
    : mColorSensor(colorSensor),
      mGyroSensor(gyroSensor),
      mLineMonitor(lineMonitor),
      mIsStartedGyro(false),
      mIsStartedBlack(false),
      mIsStartedWhite(false),
      mCalCount(0),
      mSum(0) {
}

/**
 * デストラクタ
 */
Calibration::~Calibration() {
}


/**
 * キャリブレーションに必要なものをリセットする
 */
void Calibration::init() {

    mGyroSensor.setOffset(0);                           // ジャイロセンサオフセット初期化
}

/**
 * ジャイロセンサのオフセット値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateGyro(bool startTrigger) {

    int16_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mGyroSensor.getAnglerVelocity();

    if (mIsStartedGyro == false) {
        sprintf( buf, "gyro = %03d", sensor);           // ジャイロセンサ値を表示
        ev3_lcd_draw_string( buf, 0, 50);

        if (startTrigger == true) {

            mIsStartedGyro = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                               // ジャイロセンサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_GYRO_CALIBRATION) {     // 規定回数以上積算

            cal = mSum / NUM_OF_GYRO_CALIBRATION;       // 平均値 ★キャリブレーション演習修正箇所
            mGyroSensor.setOffset(cal);

            sprintf( buf, "gyroOffset = %03d", cal);    // ジャイロオフセット値を表示
            ev3_lcd_draw_string( buf, 0, 50);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 黒色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateBlack(bool startTrigger) {

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedBlack == false) {

        sprintf( buf, "black = %03d", sensor);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 70);

        if (startTrigger == true) {

            mIsStartedBlack = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_BLACK_CALIBRATION) {    // 規定回数以上積算

            cal = mSum / NUM_OF_BLACK_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setBlackThreshold(cal);

            sprintf( buf, "blackTh = %03d", cal);       // 黒しきい値を表示
            ev3_lcd_draw_string( buf, 0, 70);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 白色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateWhite(bool startTrigger) {

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedWhite == false) {

        sprintf( buf, "white = %03d", sensor);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 90);

        if (startTrigger == true) {

            mIsStartedWhite = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else {

        mSum += sensor;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) {    // 規定回数以上積算

            cal = mSum / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setWhiteThreshold(cal);

            sprintf( buf, "whiteTh = %03d", cal);       // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 90);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * ライントレースしきい値を設定
 */
void Calibration::calibrateLineThreshold() {
    mLineMonitor->calLineThreshold();
}
