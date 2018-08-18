#ifndef EV3_UNIT_BLUETOOTH_H_
#define EV3_UNIT_BLUETOOTH_H_
#include "ev3api.h"

#define BT_MESSAGE_LEN  40
#define MAX_LEN  3

class BlueTooth {
public:
    BlueTooth();
    // ~BlueTooth();
    bool isConnected();
    void OpenSerialPort();
    uint8_t getChar();
    uint8_t getNumber();
    void putChar(char& c);
    void putString(const char *s);
    uint8_t getValue();
    FILE *mBT;
private:
    // FILE *mBT;
};
#endif  // EV3_UNIT_BLUETOOTH_H_
