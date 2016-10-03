#pragma once
/*
	�L�[�{�[�h�ƃQ�[���p�b�h����̓���
*/

#include "USBGamePad.h"

class InputKey
{
private:
	bool isHoldCode = false; //����Code�����͂��ꑱ���Ă鎞�Atrue�ɂȂ�B
public:
	static bool InputHoldCode(int n){ return (onKey(n) != 0); }	//int VK_
	static bool InputHoldCode(JoypadCode n){ return (onJoyKey(n) != 0); }//JoypadCode PAD_INPUT_

	bool InputPushCode(int n)
	{ 
		if (onKey(n))
		{
			//���O�܂ŉ���������ĂȂ�����
			if (!isHoldCode)
			{
				isHoldCode = true;
				return true;
			}

			//���ɉ�����Ă���
			else if (isHoldCode)
			{
				return false;
			}
		}
		isHoldCode = false;
		return false;
	}
};