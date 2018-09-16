#include "Remote.h"
/**
 * コンストラクタ
 */
 Remote::Remote(BalancingWalker* balancingwalker)
    //  :  mBT(fp)
:bt_cmdflg(false),
mbalancingwalker(balancingwalker),
bt_cmd(10),
bt_cmdfor(0),
bt_cmdturn(0),
bt_cmdend(0),
bt_cmdtail(0),
bt_cmdspd(4)

        {
            // FILE *mBT;
            OpenSerialPort();
 }

void Remote::OpenSerialPort(){
   mBT = ev3_serial_open_file(EV3_SERIAL_BT);
}

void Remote::getsr(){
		c = getChar();
        c_debug = c; // デバック用
	chgSpeed();
}


void Remote::RemoteControl(){

	mbalancingwalker->run();
	setSpeed();
	mbalancingwalker->setCommand(forward,turn);
	color_yellow(); /* デバッグ用に光らせる */

}

void Remote::chgSpeed(){

		switch(c)
        {
        case '0':
            bt_cmd = 1;
            break;
        default:
            break;
        }

        switch(c)
        {
        case '0':
            bt_cmdend++;
        	c = 10;
            break;
        default:
            break;
        }


        switch(c)
        {
        case '5':
            bt_cmdfor = 5;
            break;

        case '8':
            bt_cmdfor = 8;
            break;

        case '2':
            bt_cmdfor = 2;
            break;

        case '4':
            bt_cmdfor = 4;
            break;

        case '6':
            bt_cmdfor = 6;
            break;

        case 'f':
            bt_cmdfor = 20;
            break;

        case 'j':
            bt_cmdfor = 21;
            break;
        default:
            break;
        }

    	switch(c)
        {
        case '3':
            bt_cmdtail = 3;
            break;

        case '9':
            bt_cmdtail = 9;
            break;
        default:
            break;
        }

    	switch(c)
        {
        case '1':
        	if(bt_cmdspd >1){
            bt_cmdspd--;
        	}
        	c = 10;
            break;

        case '7':
        	if(bt_cmdspd<10){
            bt_cmdspd++;
        	}
        	c = 10;
            break;
        default:
            break;
        }

    	switch(c)
    	{
        case 'b':
    		if(bt_cmdflg == false){
    			bt_cmdflg = true;
    			c = 10;
    		}else{
    			bt_cmdflg = false;       // if bt_cmdflg == true
    			c = 10;
    		}
    		mSound->ok(); /* リモート操作用に音を鳴らす */
    		break;

    	default:
    		break;
    	}

}

void Remote::setSpeed(){


       		if (bt_cmdfor == 5){
   				forward = 0;
   				turn = 0;
   			}else if(bt_cmdfor == 8){
   				forward = bt_cmdspd*10;
   				turn = 0;
   			}else if(bt_cmdfor == 2){
   				forward = -bt_cmdspd*10;
   				turn = 0;
   			}else if(bt_cmdfor == 4){
   				turn = -20;
   				forward = 0;
   			}else if(bt_cmdfor == 6){
   				turn = 20;
   				forward = 0;
   			}else if(bt_cmdfor == 20){
   				turn = -20;
   			}else if(bt_cmdfor == 21){
   				turn = 20;
   			}else{
   				forward = 0;
   				turn = 0;
   			}

}


bool Remote::RemoteState(){

		if (bt_cmdflg == true)
		{
			return (true);
		} else {
			return (false);
		}

}

void Remote::setState(){

	bt_cmdflg = true;
	return;
}

void Remote::color_yellow(){

	/* 緑とオレンジを交互に光らせて黄色を作る */
	counter++;
	if (counter < 5) {
		ev3_led_set_color(LED_GREEN);
	} else {
		ev3_led_set_color(LED_ORANGE);
		if (counter > 4) {
			counter = 0;
		}
	}

}

