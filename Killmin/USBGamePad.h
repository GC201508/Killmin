#pragma once
//USB�ڑ����ꂽ�R���g���[���[������͏����擾
//�����Fttp://d.hatena.ne.jp/nagoya313/20100322/1269256657�@�u���É��R�P�R�̓��L�@��10��`�W���C�p�b�h���́`�v
#include "padcode.h"
namespace Input {
	bool joypadKeyDown(JoypadCode joypadcode, JoypadID id = PAD_ID1);
	float joypadAnalogX(JoypadID id);
	float joypadAnalogY(JoypadID id);
}	