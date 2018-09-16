#include "Run_Stairs.h"
#include "ev3api.h"

/**
 * コンストラクタ
*/
Run_Stairs::Run_Stairs( BalancingWalker* balancingWalker, PidController* pidController, LineMonitor* LineMonitor )
		:   mBalancingWalker( balancingWalker ), mPidController( pidController ),
			mLineMonitor( LineMonitor ), FirstTime_Flg( 0 ),
			First_Position( 0 ), Tail_Down_Count( 0 ),
			Now_Position( 0 ), Stairs_Count( 0 ),
			Stand_Tail_Timer( 0 ), Tail_Angle( 80 ), Reverse_Time( 0 ),
			Reverse_Comp( 0 ), Init_Flg( 0 ), Prev_Position( 0 ),
			Stairs_Stop_Timer( 0 ), Run_Ctrl( 0 ), Cyc_Count( 0 ), Init_Flg_2nd( 0 )
{

}

/**
 * デストラクタ
 */

Run_Stairs::~Run_Stairs	()
{

}

char Run_Stairs::Stairs_Main()
{
	sts = STAIRS_CTRL ;

	if ( FirstTime_Flg == false )												/* １回目のみTail制御をする		*/
	{
		mTailMotor->setAngle( Tail_Angle ) ; 									/* 尻尾を下す					*/
		Tail_Down_Count++ ;

		if ( Tail_Down_Count >= CYCTIME_1S )
		{
			// mLineMonitor->LeanModecalLineThreshold();	// !! 必要？
			mRunManager->setOrigin();
			mLineMonitor->setLineThreshold(10);
			FirstTime_Flg = true ;
			Tail_Down_Count = CYCTIME_INIT ;
			First_Position = 0;//Now_Position ;										/* 制御開始の位置を保存			*/
		}
	}
	else
	{
		Now_Position = mRunManager->getDistanceFromOrigin() ;						/* 現在の位置を取得する			*/
		if ( Cyc_Count == 1 )
		{
			Stairs_Down() ;
			Stairs_Count = 3 ;
		}

		if ( (FIRST_STAIR_OFFSET <= (Now_Position - First_Position) )
		  && (Stairs_Count == 0) )
		{
			Run_Ctrl = 1 ;
			Stairs_Count = 1 ;
			Prev_Position = Now_Position ;
		}
		else if ( (Reverse_Time == 175)
				&& (Run_Ctrl == 1) )
		{
			Run_Ctrl = 2 ;
			Stairs_Count = 2 ;
		}
		else if ( (Stop_Comp == 1) && (Stairs_Count == 2) )
		{
			Stairs_Up() ;
		}
		else if ( (SECOND_STAIR_OFFSET <= (Now_Position - Prev_Position) )
		      && (Stairs_Count == 3) )
		{
			Run_Ctrl = 1 ;
			Stairs_Count = 1 ;
			Prev_Position = Now_Position ;
		}
		else if ( (Reverse_Time == 175)
				&& (Run_Ctrl == 1) )
		{
			Run_Ctrl = 2 ;
			Stairs_Count = 2 ;
		}
		else if ( (Stop_Comp == 1) && (Stairs_Count == 2) )
		{
			Stairs_Up() ;
		}
		else
		{
			/* 処理なし		*/
		}

		Stairs_Run() ;
	}

	return ( sts ) ;
}


void Run_Stairs::Stairs_Run()
{
	if ( Run_Ctrl == 0 )
	{
		mLeftWheel.setPWM( 10 + 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
		mRightWheel.setPWM( 10 - 0.2 * mPidController->LeancalControlledVariable(mLineMonitor->getDeviation()) );
	}
	else if ( Run_Ctrl == 1 )
	{
		Reverse_Time++ ;
		if ( Reverse_Time <= CYCTIME_1S )
		{
			mLeftWheel.setPWM( -5 );
			mRightWheel.setPWM( -5 );
			if ( Reverse_Time >= CYCTIME_1S )
			{
				Reverse_Time = 0 ;
			}
		}
	}
	else
	{
		mLeftWheel.setPWM( -1 );
		mRightWheel.setPWM( -1 );
		Stop_Comp = 1 ;
	}
}


void Run_Stairs::Stairs_Up()
{
	Stand_Tail_Timer++ ;
	if ( Init_Flg == 0 )
	{
		// mBalancingWalker->init() ;		// !! バランスウォーカ（倒立制御）を初期化するのはなぜ？
		Init_Flg = 1 ;
	}

	if ( Stand_Tail_Timer >= CYCTIME_1S )
	{
		Tail_Angle += 2 ;
		Stand_Tail_Timer = 0 ;
	}
	else
	{
		mTailMotor->setPgain( 20.0F );
		mTailMotor->setAngle( Tail_Angle ) ; 							/* 尻尾を下す					*/
	//	mBalancingWalker->setCommand( -20, 0 ) ;
	//	mBalancingWalker->run() ;
	}

	if ( Tail_Angle >= 107 )
	{
		Contrl_2nd = 1 ;
		Run_Ctrl = 0 ;
		Stairs_Count = 2 ;
		mTailMotor->setPgain( 5.0F );
		Cyc_Count = 1 ;
	}
}


void Run_Stairs::Stairs_Down()
{
	if ( Tail_Angle >= 85 )
	{
		Tail_Angle -- ;
	}
	mTailMotor->setAngle( Tail_Angle ) ; 							/* 尻尾を下す					*/
}


void Run_Stairs::Stairs_Run_2nd()
{
	if ( Init_Flg_2nd == 0 )
	{
		// mBalancingWalker->init() ;
	}
	mBalancingWalker->setCommand( 10, 0 ) ;
	mBalancingWalker->run() ;
}

#if 0
/* test */
ev3_led_set_color(LED_RED);
/* test */
ev3_led_set_color(LED_GREEN);

#endif
