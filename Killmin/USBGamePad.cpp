#include "USBGamePad.h"

namespace Input {//実装上インスタンス化する意味があまりないのと、グローバル関数は嫌われるのでInput名前空間で括ります。

	// Input::joypadKeyDown関数では、指定したIDのジョイパッドの指定したボタンが押されていればtrueを返す関数です。joyGetDevCaps関数で、ジョイパッドの性能を調べ、JOYCAPS構造体のdwSizeメンバにサイズとdwFlagsメンバにボタン入力を得るフラグを立てた後joyGetPosExを使って現在の入力状態を調べます。
	// 返り値がJOYERR_NOERROR以外だったらエラーなのでfalseを返しときます。
	// ボタン入力は、JOYINFOEX構造体のdwButtonsメンバには各キーの状態が入っています。
	// ボタンが押されていればそのボタン番号のビットが立っているので、ボタン番号と&した結果で調べます。
	// 十字キーの方は傾き具合が縦方向、横方向それぞれdwXpos、dwYposメンバに入っています。
	// なんか変な計算をしているのは、遊びの部分を入力ととらないようにする工夫的な何かです。　←アナログスティックはないので削除

	//十字キー　dwXpos,dwYpos 中央値（32511） 
	//上方向　dwYpos = 0;
	//下方向　dwYpos = 65535;
	//左方向　dwXPos = 0;
	//右方向　dwXpos = 65535;

	bool joypadKeyDown(JoypadCode joypadcode, JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNBUTTONS;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			if (joypadcode <= PAD_INPUT_10) {
				return (joypad_info.dwButtons & joypadcode) != 0;
			}
			else if (joypadcode == PAD_INPUT_UP) {
				return (joypad_info.dwYpos == 0);
			}
			else if (joypadcode == PAD_INPUT_DOWN) {
				return (joypad_info.dwYpos == 65535);
			}
			else if (joypadcode == PAD_INPUT_LEFT) {
				return (joypad_info.dwXpos == 0);
			}
			else if (joypadcode == PAD_INPUT_RIGHT) {
				return (joypad_info.dwXpos == 65535);
			}
			else {
				return false;
			}
		}
		return false;
	}

	// joypadAnalogX関数とjoypadAnalogY関数はそれぞれアナログスティックのX軸、Y軸の傾き具合を返します。
	// JOYCAPS構造体のdwFlagsメンバには、アナログスティックの軸の傾き具合を
	//取得するフラグJOY_RETURNX、JOY_RETURNYを入れてからjoyGetPosExを使って傾き具合を調べます。
	// 変な計算をしているのはまた例によって遊び部分を傾きとして取らないようにする工夫的な何かです。
	// 微妙な傾きは無視してあげた方が多分快適に操作できます。

	float joypadAnalogX(JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNX;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			const float center((static_cast<float>(joypad_caps.wXmin + joypad_caps.wXmax)) / 2.f);
			const float diff(static_cast<float>(joypad_caps.wXmax - joypad_caps.wXmin));
			const float val((static_cast<float>(joypad_info.dwXpos) - center) * 2.f / diff);
			return fabsf(val) < 0.1f ? 0.f : val;
		}
		return false;
	}

	float joypadAnalogY(JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNY;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			const float center((static_cast<float>(joypad_caps.wYmin + joypad_caps.wYmax)) / 2.f);
			const float diff(static_cast<float>(joypad_caps.wYmax - joypad_caps.wYmin));
			const float val((static_cast<float>(joypad_info.dwYpos) - center) * 2.f / diff);
			return fabsf(val) < 0.1f ? 0.f : val;
		}
		return false;
	}
}