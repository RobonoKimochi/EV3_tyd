#ifndef EV3_UNIT_ROTARYENC_H_
#define EV3_UNIT_ROTARYENC_H_

#include "MotorDriver.h"

class RotaryEnc {
public:
    RotaryEnc(){};
    void initEnc();
    int32_t getRightEnc();
    int32_t getLeftEnc();
private:
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    int32_t mRightEnc;
    int32_t mLeftEnc;
    int32_t mRightEncOffset;
    int32_t mLeftEncOffset;
};

#endif  // EV3_UNIT_ROTARYENC_H_
