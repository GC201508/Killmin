/*
	XInput(XBox�̃R���g���[��)�ł̓��͂��������܂���,
	
	//TODO(Takemasa):
	�� ���̊J���Ŏg���Ă���̂�ELECOM��[JC-U3613M], XBox�R���Ƃقړ����`�Ō݊��������邽��2016�N10��12�����ݎx��͖�,
	�� �����I��Nintendo64�R���g���[���ł̑����z�肵��DirectInput����������\��i�J�����̓���m�F�͂��̃N���X�ōs���܂��j
*/

#pragma once
#include "System.h"
#include <XInput.h> // XInput API

namespace {
	/* Noncopy�|���V�[, */
	struct Noncopyable{ 
		Noncopyable()                   = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator           =(const Noncopyable&) = delete;
	};

	enum EnButton{
		enButtonUp    ,		    //!<��B
		enButtonDown  ,		    //!<���B
		enButtonLeft  ,		    //!<���B
		enButtonRight ,		    //!<�E�B
		enButtonA     ,			//!<A�{�^���B
		enButtonB     ,			//!<B�{�^���B
		enButtonX     ,			//!<X�{�^���B
		enButtonY     ,			//!<Y�{�^���B
		enButtonSelect,	        //!<�Z���N�g�{�^���B
		enButtonStart ,		    //!<�X�^�[�g�{�^���B
		enButtonRB1   ,		    //!<RB1�{�^���B
		enButtonRB2   ,		    //!<RB2�{�^���B
		enButtonRB3   ,		    //!<RB3�{�^���B
		enButtonLB1   ,		    //!<LB1�{�^���B
		enButtonLB2   ,		    //!<LB2�{�^���B
		enButtonLB3   ,		    //!<LB3�{�^���B
		enButtonNum   ,	        //!<�{�^���̐��B
	};
}

class XInputMode : Noncopyable
{	
	struct PAD_STATE{
			XINPUT_STATE state;
			bool bConnected;
	};
	struct Stick{	            //!<�X�e�B�b�N
		float LeftX;	        //!< X���̓��͗ʁi���X�e�B�b�N�j
		float LeftY;	        //!< Y���̓��͗ʁi���X�e�B�b�N�j
		float RightX;	        //!< X���̓��͗ʁi�E�X�e�B�b�N�j
		float RightY;	        //!< Y���̓��͗ʁi�E�X�e�B�b�N�j
	}stick;

	PAD_STATE state;	        //!<�p�b�h�X�e�[�g,
	int padNo;			        //!<�p�b�h�ԍ�,
	int trigger[enButtonNum];	//!<�g���K�[,
	int press[enButtonNum];		//!<<press���͂̃t���O,

	// * @brief		�X�e�B�b�N�\���̂�������,
	void StickInit(){
		stick.LeftX  = 0.f;
		stick.LeftY  = 0.f;
		stick.RightX = 0.f;
		stick.RightY = 0.f;
	}

	// * @brief		���X�e�B�b�N���͗�,
	void LeftStickInput();

	// * @brief		�E�X�e�B�b�N���͗�,
	void RightStickInput();

	// * @brief		�L�[�{�[�h���͂ɂ��G�~�����[�g�iXbox�R�����ڑ��j
	void KeyboardInput();

	// * @brief		Xbox�R���ڑ����ʂɂ�鏈��
	void XInputResultConnection();

public:
	XInputMode();
	~XInputMode();
	
	// * @brief		������, 
	// * @param		[in] padNo	�p�b�h�ԍ�,
	void Init(int);

	void Update();

	// * @brief		�{�^���̃g���K�[����,
	// * @return	[true]	�g���K�[����,
	bool ISTrigger(EnButton button) const
	{
		//button < enButtonNum ? /*�߂����[���ڂ�����*/
		return trigger[button] != 0;
	}


	// * @brief		�{�^����������Ă��邩����,
	// * @return	[true]	������Ă���,
	bool IsPress(EnButton button) const
	{
		//messegebox
		return press[button] != 0;
	}

//get

	// * @brief		�X�e�B�b�N�̎��̓��͗ʂ��擾,
	// * @return	[-1.0�`1.0�̐��K�����ꂽ�l],
		float getLeftStickX()  const { return stick.LeftX;  }
		float getLeftStickY()  const { return stick.LeftY;  }
		float getRightStickX() const { return stick.RightX; }
		float getRightStickY() const { return stick.RightY; }
};