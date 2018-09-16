#include "Rotator.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
Rotator::Rotator()
    : mThetaOffset(mOdmetry->getTheta())
    {
}

/**
 * デストラクタ
 */
Rotator::~Rotator() {
}
/**
 * 回転方向を設定する
 */
// void Rotator::setDirection(int direction){
//     mDirection = direction;
// }

bool Rotator::doRotate() {
    if( mOdmetry->getTheta() - mThetaOffset < 6.28318530718f){
        return true;
    }else {
        return false;
    }
}
