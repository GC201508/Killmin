#pragma once
/*
	�L�[�{�[�h�ƃQ�[���p�b�h����̓���
*/

#include "USBGamePad.h"

class InputKey
{
public:
	static bool InputKeyCode(int n){ return (onKey(n) != 0); }	//int VK_
	static bool InputKeyCode(JoypadCode n){ return (onJoyKey(n) != 0); }//JoypadCode PAD_INPUT_
};