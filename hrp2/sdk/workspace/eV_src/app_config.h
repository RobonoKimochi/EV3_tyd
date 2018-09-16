/*
 * app_config.h
 *
 *  Created on: 2018/07/31
 *      Author: toyoda
 */

/* コンパイルSW用ヘッダファイル */

#ifndef EV_SRC_APP_CONFIG_H_
#define EV_SRC_APP_CONFIG_H_

/* コースの設定 */
/* 1:左コース	2:右コース */
#define RUN_LEFT_COURSE	(1)
#define RUN_RIGHT_COURSE	(2)

#define RUN_COURSE (RUN_LEFT_COURSE) /* ←ここにコースを記載 */

/* デバッグモード */
/* 0:通常走行モード	1:デバッグ走行モード */
#define USE_DEBUG_MODE		(1)


/* リモコン操作設定 */
/* 0:リモコン操作なし	1:リモコン操作あり */
#define USE_REMOTE_CONTROL_MODE	(1)


/* ロガー設定 */
/* 0:ログ出力なし	1:ログ出力あり */
#define USE_OUTPUT_LOG		(1)


/* ライントレースON */
/* 0:ライントレースOFF	1:ライントレースON */
#define USE_LINE_TRACE		(1)


/* PID切り替え無効 */
/* 0:PID切り替えあり	1:PID切り替え無し */
#define USE_CONSTANT_PID		(1)


/* FORWARD値設定 */
#define CONSTANT_FORWARD_VAL	(70)

/* FORWARD値設定 */
#define SOUND_VOL				(4)

/* ルックアップゲートテスト用設定 */
/* 0:テストモードOFF	1:テストモードON */
#define LOOKUPGATE_TEST_MODE	(1)

/* グレー対応閾値設定 */
/* 0:グレー対応OFF	1:グレー対応ON */
#define USE_GRAY_THRESH	(1)


#endif /* EV_SRC_APP_CONFIG_H_ */
