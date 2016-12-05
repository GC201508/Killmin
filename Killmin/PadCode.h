// padcode.h
#include "System.h"
#include <mmsystem.h>

/*	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-
	☆動作確認コントローラ: 
	【ELECOM】JC-U2410Tシリーズ １０ボタン配列USBゲームパッド
	http://www2.elecom.co.jp/peripheral/gamepad/jc-u2410t/image/ex1.gif
-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	*/

enum JoypadID {
	PAD_ID1 = JOYSTICKID1,
	PAD_ID2 = JOYSTICKID2
};

enum JoypadCode {
	PAD_INPUT_1 = JOY_BUTTON1,
	PAD_INPUT_2 = JOY_BUTTON2,
	PAD_INPUT_3 = JOY_BUTTON3,
	PAD_INPUT_4 = JOY_BUTTON4,
	PAD_INPUT_5 = JOY_BUTTON5,
	PAD_INPUT_6 = JOY_BUTTON6,
	PAD_INPUT_7 = JOY_BUTTON7,
	PAD_INPUT_8 = JOY_BUTTON8,
	PAD_INPUT_9 = JOY_BUTTON9,
	PAD_INPUT_10 = JOY_BUTTON10,
	PAD_INPUT_UP = JOY_BUTTON11,
	PAD_INPUT_DOWN = JOY_BUTTON12,
	PAD_INPUT_LEFT = JOY_BUTTON13,
	PAD_INPUT_RIGHT = JOY_BUTTON14,
};