#include "RotaryEnc.h"

  /**
   * 左車輪のエンコーダ値を取得する
   */
  int32_t RotaryEnc::getLeftEnc(){
      mLeftEnc = mLeftWheel.getCount();// - mLeftEncOffset;
      return mLeftEnc;
  }
  /**
   * 右車輪のエンコーダ値を取得する
   */
  int32_t RotaryEnc::getRightEnc(){
      mRightEnc = mRightWheel.getCount();// - mRightEncOffset;
      return mRightEnc;
  }
