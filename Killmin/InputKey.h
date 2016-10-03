#pragma once
/*
	キーボードとゲームパッドからの入力
*/

#include "USBGamePad.h"

class InputKey
{
private:
	bool isHoldCode = false; //同じCodeが入力され続けてる時、trueになれ。
public:
	static bool InputHoldCode(int n){ return (onKey(n) != 0); }	//int VK_
	static bool InputHoldCode(JoypadCode n){ return (onJoyKey(n) != 0); }//JoypadCode PAD_INPUT_

	bool InputPushCode(int n)
	{ 
		if (onKey(n))
		{
			//直前まで何も押されてなかった
			if (!isHoldCode)
			{
				isHoldCode = true;
				return true;
			}

			//既に押されていた
			else if (isHoldCode)
			{
				return false;
			}
		}
		isHoldCode = false;
		return false;
	}
};