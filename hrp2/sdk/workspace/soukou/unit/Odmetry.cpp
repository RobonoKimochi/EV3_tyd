#include "Odmetry.h"

/**
 * コンストラクタ
 */
Odmetry::Odmetry()
 :  mLeftEnc(mLeftWheel.getCount()),
    mRightEnc(mRightWheel.getCount()),
    mX(0),
    mY(0),
    mTheta(0),
    mDeltaTheta(0)
    {
        mFirstFilter  = new Filter();
        mSecondFilter = new Filter();
}

 /**
  * 左車輪の各速度を計算する
  * @param mLeftEnc 左車輪のエンコーダ値
  * @param LeftEncDif 現在と前回のエンコーダ値の差
  */
float Odmetry::calcLeftAngVel(){
  float LeftEncDif;

  LeftEncDif = mLeftWheel.getCount() - mLeftEnc;
  mLeftAngVel = LeftEncDif / GEAR * 2 * PI;     // 各速度を計算する

  mLeftEnc = mLeftWheel.getCount();   // 値を更新する（少し冗長？要リファクタリング!）
  return (mLeftAngVel);
}
  /**
   * 右車輪の各速度を計算する
   * @param mRightEnc 右車輪のエンコーダ値
   * @param RightEncDif 現在と前回のエンコーダ値の差
   */
float Odmetry::calcRightAngVel(){
   float RightEncDif;

   RightEncDif = mRightWheel.getCount() - mRightEnc;
   mRightAngVel = RightEncDif / GEAR * 2 * PI;     // 各速度を計算する

   mRightEnc =  mRightWheel.getCount();   // 値を更新する
   return (mRightAngVel);
}
   /**
    * 位置情報を計算して更新する
    */
void Odmetry::updateLocation(){
   float leftAngVel;
   float rightAngVel;
   static float filteredtheta;
   static float filteredtheta_old = 0;
   float diff;

   leftAngVel  = calcLeftAngVel();
   rightAngVel = calcRightAngVel();
   mVelocity = (LEFT_WHEEL * leftAngVel
                + RIGHT_WHEEL * rightAngVel) / 2;
   mAngleVelocity = (RIGHT_WHEEL * rightAngVel - LEFT_WHEEL * leftAngVel) / TREAD;

   mX += mVelocity * mMat.Cos(mTheta);
   mY += mVelocity * mMat.Sin(mTheta);
   mTheta += mAngleVelocity;            // thetaが2PIを超えたとき、補正すべき？？->超えないからいい

   // 角度にLPFをかけたのち、時間変化率を計算し、その結果にさらにLPFをかける
   filteredtheta = mFirstFilter->LowPassFilter(mTheta); // 第一LPFをかける
   diff = filteredtheta - filteredtheta_old;            // 変化率を計算する
   mDeltaTheta = mSecondFilter->LowPassFilter(diff);    // 第二LPFをかける

   filteredtheta_old = filteredtheta;                   // 更新する
}
/**
 * X座標を取得する
 */
float Odmetry::getX(){
    return mX;
}
/**
 * Y座標を取得する
 */
float Odmetry::getY(){
    return mY;
}
/**
 * 位置情報を取得する
 * @param mTheta
 */
float Odmetry::getTheta(){
    return mTheta;
}
float Odmetry::getDeltaTheta(){
    return mDeltaTheta;
}
/**
 * 位置情報をクリアする
 */
void Odmetry::clearLocation(){
    mX = 0;
    mY = 0;
    mTheta = 0;
}
