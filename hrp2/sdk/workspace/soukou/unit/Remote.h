#ifndef EV3_UNIT_REMOTE_H_
#define EV3_UNIT_REMOTE_H_
#include "ev3api.h"
#include "BlueTooth.h"
#include "BalancingWalker.h"
#include "Sound.h"

class Remote : public BlueTooth
{
public:
	Remote(BalancingWalker* balancingwalker);
	void RemoteControl();
	void OpenSerialPort();
	bool RemoteState();
	bool bt_cmdflg;
	void setState();
	void getsr();

    FILE *mBT;
	uint8_t c_debug; // !! デバック用;
	uint8_t c;
private:
    // FILE *mBT;
	void chgSpeed();
	void setSpeed();
	void color_yellow();
	Sound *mSound = Sound::getInstance();
	BalancingWalker* mbalancingwalker;

	int32_t   bt_cmd;      /* Bluetoothコマンド 1:リモートスタート */
	int32_t   bt_cmdfor;      /* Bluetoothコマンド 2:リモートスタート */
	int32_t   bt_cmdturn;      /* Bluetoothコマンド 3:リモートスタート */
	int32_t   bt_cmdend;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   bt_cmdtail;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   bt_cmdspd;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   forward;
	int32_t   turn;
	int32_t   counter;
};
#endif  // EV3_UNIT_BLUETOOTH_H_
