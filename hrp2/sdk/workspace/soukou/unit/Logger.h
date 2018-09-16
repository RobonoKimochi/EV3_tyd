#ifndef EV3_UNIT_LOGGER_H_
#define EV3_UNIT_LOGGER_H_

#include "BlueTooth.h"
#include "Clock.h"
// #include "stdarg.h"
// #include "string.h"

using ev3api::Clock;

#define MAX_LABEL_LENGTH 20
#define MAX_LABEL_NUM 4
#define LOG_LENGTH 500

class Logger : public BlueTooth{
public:
    Logger();
    // void setLog(const char& label, unsigned char num);
    void sendLog(float data[], unsigned char num);
    void resetClock();
    void init();
private:
    Clock* mClock;
    uint8_t mLabelNumber;
    char mLabel[MAX_LABEL_NUM][MAX_LABEL_LENGTH] = {"Time", "X", "Y", "RPS"};
    char mBuffer[LOG_LENGTH];
    unsigned char mLabel_num;
};
#endif  // EV3_UNIT_LOGGER_H_
