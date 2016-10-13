#include "XInputMode.h"
#include <limits.h>

// +/-32767の範囲の24％にデフォルト,
// 妥当なデフォルト値であるが、必要に応じて変更することができます,
#define INPUT_DEADZONE (0.24f * FLOAT(0x7FFF)	) 

namespace{
	// * @brief 仮想ボタンとXBoxコントローラのボタンとの関連付けを表す構造体,
	struct VirtualPadToXPad{
		EnButton	vBotton;	//!<仮想ボタン,
		DWORD		xButton;	//!<XBoxコントローラのボタン,
	}const vPadToXPadTable[enButtonNum] = {
		{ enButtonUp    , XINPUT_GAMEPAD_DPAD_UP        },
		{ enButtonDown  , XINPUT_GAMEPAD_DPAD_DOWN      },
		{ enButtonLeft  , XINPUT_GAMEPAD_DPAD_LEFT      },
		{ enButtonRight , XINPUT_GAMEPAD_DPAD_RIGHT     },
		{ enButtonA     , XINPUT_GAMEPAD_A              },
		{ enButtonB     , XINPUT_GAMEPAD_B              },
		{ enButtonY     , XINPUT_GAMEPAD_Y              },
		{ enButtonX     , XINPUT_GAMEPAD_X              },
		{ enButtonSelect, XINPUT_GAMEPAD_BACK           },
		{ enButtonStart , XINPUT_GAMEPAD_START          },
		{ enButtonRB1   , XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ enButtonRB2   , 0                             },
		{ enButtonRB3   , XINPUT_GAMEPAD_RIGHT_THUMB    },
		{ enButtonLB1   , XINPUT_GAMEPAD_LEFT_SHOULDER  },
		{ enButtonLB2   , 0                             },
		{ enButtonLB3   , XINPUT_GAMEPAD_LEFT_THUMB     },
	};

	// * @brief 仮想ボタンとキーボードとの関連付けを表す構造体,
	struct VirtualPadToKeyboard {
		EnButton vButton;		//!<仮想ボタン
		DWORD keyCoord;			//!<キーボードのキーコード。
	}const vPadToKeyboardTable[enButtonNum] = {
		{ enButtonUp    , '8'       },
		{ enButtonDown  , '2'       },
		{ enButtonLeft  , '4'       },
		{ enButtonRight , '6'       },
		{ enButtonA     , 'J'       },
		{ enButtonB     , 'K'       },
		{ enButtonY     , 'I'       },
		{ enButtonX     , 'O'       },
		{ enButtonSelect, VK_SPACE  },
		{ enButtonStart , VK_RETURN },
		{ enButtonRB1   , '7'       },
		{ enButtonRB2   , '8'       },
		{ enButtonRB3   , '9'       },
		{ enButtonLB1   , 'B'       },
		{ enButtonLB2   , 'N'       },
		{ enButtonLB3   , 'M'       },
	};
}

XInputMode::XInputMode() : padNo(0)
{
	memset(&state , 0, sizeof(state))  ;
	memset(trigger, 0, sizeof(trigger));
	memset(press  , 0, sizeof(press))  ;
	StickInit();
}

XInputMode::~XInputMode(){}

void XInputMode::Init(int padNo)
{
	this->padNo = padNo;
}

void XInputMode::Update()
{
	XInputResultConnection();
}

#define SSGP	this->state.state.Gamepad
#define	s_cast	static_cast
void XInputMode::XInputResultConnection()
{
	DWORD result = XInputGetState(padNo, &state.state);

	if (result == ERROR_SUCCESS){

		/* 接続あり, */

		this->state.bConnected = true;

		for (const VirtualPadToXPad& vPadToXPad : vPadToXPadTable)
		{

			if (SSGP.wButtons & vPadToXPad.xButton)
			{
				trigger[vPadToXPad.vBotton] = 1 ^ press[vPadToXPad.vBotton];
				press[vPadToXPad.vBotton] = 1;
			}
			else
			{
				trigger[vPadToXPad.vBotton] = 0;
				press[vPadToXPad.vBotton] = 0;
			}
		}

		LeftStickInput();
		RightStickInput();

	}
	else
	{
		/* 接続ない, */

		KeyboardInput();
	}
}

void XInputMode::LeftStickInput()
{
	if ((SSGP.sThumbLX < INPUT_DEADZONE   &&
		 SSGP.sThumbLX > -INPUT_DEADZONE) &&
		(SSGP.sThumbLY < INPUT_DEADZONE   &&
		 SSGP.sThumbLY > -INPUT_DEADZONE))
	{
		SSGP.sThumbLX = 0;
		SSGP.sThumbLY = 0;
		stick.LeftX   = 0.f;
		stick.LeftY   = 0.f;
	}
	else
	{//スティック入力量

		/*X軸*/

		if (SSGP.sThumbLX > 0){
			stick.LeftX = s_cast<float>(SSGP.sThumbLX) / SHRT_MAX;
		}
		else{
			stick.LeftX = s_cast<float>(SSGP.sThumbLX) / -SHRT_MIN;
		}

		/*Y軸*/
		if (SSGP.sThumbLY > 0){
			stick.LeftY = s_cast<float>(SSGP.sThumbLY) / SHRT_MAX;
		}
		else{
			stick.LeftY = s_cast<float>(SSGP.sThumbLY) / -SHRT_MIN;
		}
	}
}

void XInputMode::RightStickInput()
{
	if ((SSGP.sThumbRX < INPUT_DEADZONE &&
		SSGP.sThumbRX > -INPUT_DEADZONE) &&

		(SSGP.sThumbRY < INPUT_DEADZONE &&
		SSGP.sThumbRY > -INPUT_DEADZONE))
	{
		SSGP.sThumbRX = 0;
		SSGP.sThumbRY = 0;
		stick.RightX = 0.f;
		stick.RightY = 0.f;
	}
	else
	{//スティック入力量,

		/*X軸*/
		if (SSGP.sThumbRX > 0){
			stick.RightX = s_cast<float>(SSGP.sThumbRX) / SHRT_MAX;
		}
		else{
			stick.RightX = s_cast<float>(SSGP.sThumbRX) / -SHRT_MIN;
		}
		/*Y軸*/
		if (SSGP.sThumbRY > 0){
			stick.RightY = s_cast<float>(SSGP.sThumbRY) / SHRT_MAX;
		}
		else{
			stick.RightY = s_cast<float>(SSGP.sThumbRY) / -SHRT_MIN;
		}
	}
}

const int RS_LEFT  = VK_LEFT;
const int RS_RIGHT = VK_RIGHT;
const int RS_UP    = VK_UP;
const int RS_DOWN  = VK_DOWN;
const int LS_LEFT  = 'A';
const int LS_RIGHT = 'D';
const int LS_UP    = 'W';
const int LS_DOWN  = 'S';
void XInputMode::KeyboardInput()
{
	if (state.bConnected)
	{
		/* 未接続になった, */

		memset(&state,		0, sizeof(state))	;
		memset(&trigger,	0, sizeof(trigger))	;
		memset(press,		0, sizeof(press))	;
	}
	StickInit();

		 if (onKey(RS_LEFT))	{ stick.RightX = -1.f; }
	else if (onKey(RS_RIGHT))	{ stick.RightX =  1.f; }
		 if (onKey(RS_UP))		{ stick.RightY =  1.f; }
	else if (onKey(RS_DOWN))	{ stick.RightY = -1.f; }

	/* 右スティック入力量を正規化, */

	float t = fabsf(stick.RightX) + fabsf(stick.RightY);
	if (t > 0.f)
	{
		stick.RightX /= t;
		stick.RightY /= t;
	}

		 if (onKey(LS_LEFT))	{	stick.LeftX = -1.f; }
	else if (onKey(LS_RIGHT))	{	stick.LeftX =  1.f;	}
		 if (onKey(LS_UP))		{	stick.LeftY =  1.f; }
	else if (onKey(LS_DOWN))	{	stick.LeftY = -1.f; }

	/* 左スティック入力量を正規化, */
	t = fabsf(stick.LeftX) + fabsf(stick.LeftY);
	if (t > 0.f){
		stick.LeftX /= t;
		stick.LeftY /= t;
	}


	for (const VirtualPadToKeyboard& vPadToKeyboard : vPadToKeyboardTable) {
		if (onKey(vPadToKeyboard.keyCoord)) {
			trigger[vPadToKeyboard.vButton] = 1 ^ press[vPadToKeyboard.vButton];
			press[vPadToKeyboard.vButton]   = 1;
		}
		else {
			trigger[vPadToKeyboard.vButton] = 0;
			press[vPadToKeyboard.vButton]   = 0;
		}
	}
}