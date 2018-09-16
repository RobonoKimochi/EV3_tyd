#ifndef EV3_UNIT_ROTATION_H_
#define EV3_UNIT_ROTATION_H_

#include "Odmetry.h"

class Rotator {
public:
    // static const int RIGHT = 1;
    // static const int LEFT = -1;
    Rotator();
    ~Rotator();
    bool doRotate();
    void setDirection(int direction);
private:
    Odmetry *mOdmetry = Odmetry::getInstance();
    float mThetaOffset;
};

#endif  // EV3_UNIT_ROTATION_H_
