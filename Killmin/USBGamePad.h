#pragma once
//USB接続されたコントローラーから入力情報を取得
//抜粋：ttp://d.hatena.ne.jp/nagoya313/20100322/1269256657　「名古屋３１３の日記　第10回～ジョイパッド入力～」
#include "padcode.h"
namespace Input {
	bool joypadKeyDown(JoypadCode joypadcode, JoypadID id = PAD_ID1);
	float joypadAnalogX(JoypadID id);
	float joypadAnalogY(JoypadID id);
}	