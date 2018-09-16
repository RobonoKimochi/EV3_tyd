#ifndef EV3_UNIT_ODMETRY_H_
#define EV3_UNIT_ODMETRY_H_

// #include "RotaryEnc.h"
#include "Mat.h"
#include "MotorDriver.h"
#include "Filter.h"

#define GEAR 360.
#define LEFT_WHEEL  4.05F//8.1F // 左車輪直径 [cm]
#define RIGHT_WHEEL 4.05F//8.1F // 右車輪直径 [cm]
#define TREAD       16.00F////13.26F // トレッド（車輪間距離） [cm]
#define SAMP_RATE   0.004F  // サンプリングレート [s]

class Odmetry {
public:
    static Odmetry *getInstance(){
        static Odmetry odmetry;
        return &odmetry;
    }
    float getX();                                   // X座標を取得する
    float getY();                                   // Y座標を取得する
    float getTheta();                               // 角度情報を取得する
    void updateLocation();                          // 位置情報を更新する
    void clearLocation();                           // 位置情報をクリアする
    float getDeltaTheta();                          // 角度の時間変化率（LPFでフィルタ済み）
private:
    Odmetry();
    // ~Odmetry(){};
    float calcLeftAngVel();                         // 左車輪の角速度を計算する
    float calcRightAngVel();                        // 右車輪の角速度を計算する
private:
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    Mat mMat;
    float mLeftAngVel;
    float mRightAngVel;
    int32_t mLeftEnc;
    int32_t mRightEnc;
    float mVelocity;        // ロボットの速度
    float mAngleVelocity;   // ロボットの角速度
    float mX;
    float mY;
    float mTheta;
    float mDeltaTheta;
    Filter *mFirstFilter;
    Filter *mSecondFilter;
};

#endif  // EV3_UNIT_ODMETRY_H_
