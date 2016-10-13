/*
	XInput(XBoxのコントローラ)での入力を処理しますよ,
	
	//TODO(Takemasa):
	※ この開発で使っているのはELECOM製[JC-U3613M], XBoxコンとほぼ同じ形で互換性もあるため2016年10月12日現在支障は無,
	※ 将来的にNintendo64コントローラでの操作を想定してDirectInputも実装する予定（開発中の動作確認はこのクラスで行うます）
*/

#pragma once
#include "System.h"
#include <XInput.h> // XInput API

namespace {
	/* Noncopyポリシー, */
	struct Noncopyable{ 
		Noncopyable()                   = default;
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator           =(const Noncopyable&) = delete;
	};

	enum EnButton{
		enButtonUp    ,		    //!<上。
		enButtonDown  ,		    //!<下。
		enButtonLeft  ,		    //!<左。
		enButtonRight ,		    //!<右。
		enButtonA     ,			//!<Aボタン。
		enButtonB     ,			//!<Bボタン。
		enButtonX     ,			//!<Xボタン。
		enButtonY     ,			//!<Yボタン。
		enButtonSelect,	        //!<セレクトボタン。
		enButtonStart ,		    //!<スタートボタン。
		enButtonRB1   ,		    //!<RB1ボタン。
		enButtonRB2   ,		    //!<RB2ボタン。
		enButtonRB3   ,		    //!<RB3ボタン。
		enButtonLB1   ,		    //!<LB1ボタン。
		enButtonLB2   ,		    //!<LB2ボタン。
		enButtonLB3   ,		    //!<LB3ボタン。
		enButtonNum   ,	        //!<ボタンの数。
	};
}

class XInputMode : Noncopyable
{	
	struct PAD_STATE{
			XINPUT_STATE state;
			bool bConnected;
	};
	struct Stick{	            //!<スティック
		float LeftX;	        //!< X軸の入力量（左スティック）
		float LeftY;	        //!< Y軸の入力量（左スティック）
		float RightX;	        //!< X軸の入力量（右スティック）
		float RightY;	        //!< Y軸の入力量（右スティック）
	}stick;

	PAD_STATE state;	        //!<パッドステート,
	int padNo;			        //!<パッド番号,
	int trigger[enButtonNum];	//!<トリガー,
	int press[enButtonNum];		//!<<press入力のフラグ,

	// * @brief		スティック構造体を初期化,
	void StickInit(){
		stick.LeftX  = 0.f;
		stick.LeftY  = 0.f;
		stick.RightX = 0.f;
		stick.RightY = 0.f;
	}

	// * @brief		左スティック入力量,
	void LeftStickInput();

	// * @brief		右スティック入力量,
	void RightStickInput();

	// * @brief		キーボード入力によるエミュレート（Xboxコン未接続）
	void KeyboardInput();

	// * @brief		Xboxコン接続結果による処理
	void XInputResultConnection();

public:
	XInputMode();
	~XInputMode();
	
	// * @brief		初期化, 
	// * @param		[in] padNo	パッド番号,
	void Init(int);

	void Update();

	// * @brief		ボタンのトリガー判定,
	// * @return	[true]	トリガー入力,
	bool ISTrigger(EnButton button) const
	{
		//button < enButtonNum ? /*めっせーじぼっくす*/
		return trigger[button] != 0;
	}


	// * @brief		ボタンが押されているか判定,
	// * @return	[true]	押されている,
	bool IsPress(EnButton button) const
	{
		//messegebox
		return press[button] != 0;
	}

//get

	// * @brief		スティックの軸の入力量を取得,
	// * @return	[-1.0～1.0の正規化された値],
		float getLeftStickX()  const { return stick.LeftX;  }
		float getLeftStickY()  const { return stick.LeftY;  }
		float getRightStickX() const { return stick.RightX; }
		float getRightStickY() const { return stick.RightY; }
};