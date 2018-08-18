#ifndef EV3_DEVICE_SENSORDRIVER_H_
#define EV3_DEVICE_SENSORDRIVER_H_

#include "ev3api.h"
#include "ColorSensor.h"
#include "GyroSensor.h"
#include "SonarSensor.h"
#include "TouchSensor.h"
// #include "Sensor.h"

// オブジェクトを静的に確保する
const ev3api::TouchSensor _gTouchSensor(PORT_1);
const ev3api::ColorSensor _gColorSensor(PORT_3);
const ev3api::SonarSensor _gSonarSensor(PORT_2);
const ev3api::GyroSensor  _gGyroSensor(PORT_4);

class SensorDriver {
public:
    static SensorDriver& getInstance(){
        static SensorDriver sensorDriver;
        return sensorDriver;
    }
    ev3api::TouchSensor getTouchSensor(){return _gTouchSensor;};
    ev3api::ColorSensor getColorSensor(){return _gColorSensor;};
    ev3api::SonarSensor getSonarSensor(){return _gSonarSensor;};
    ev3api::GyroSensor getGyroSensor(){return _gGyroSensor;};

private:
    SensorDriver(){};
    ~SensorDriver(){};
};

#endif  // EV3_DEVICE_SENSORDRIVER_H_
