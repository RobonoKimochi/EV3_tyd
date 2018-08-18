#ifndef EV3_DEVICE_MOTORDRIVER_H_
#define EV3_DEVICE_MOTORDRIVER_H_

#include "ev3api.h"
#include "Motor.h"

// Device objects
// オブジェクトを静的に確保する
const ev3api::Motor _gTailMotor(PORT_D);
const ev3api::Motor _gRightWheel(PORT_B);
const ev3api::Motor _gLeftWheel(PORT_C);

class MotorDriver {
public:
    static MotorDriver& getInstance(){
        static MotorDriver motorDriver;
        return motorDriver;
    }
    ev3api::Motor getTailMotor(){return _gTailMotor;};
    ev3api::Motor getRightWheel(){return _gRightWheel;};
    ev3api::Motor getLeftWheel(){return _gLeftWheel;};
private:
    MotorDriver(){;};
    ~MotorDriver(){;};
};

#endif  // EV3_DEVICE_MOTORDRIVER_H_
