#include "Player.h"
#include "Model.h"
#include "InputKey.h"
#define PLAYER_SPEED 0.03f
Player::Player()
{//初期化行う
	lpModel = nullptr;
	addPos = nullptr;
}
Player::~Player(){}

void Player::Init()
{//初期化行う
	if (lpModel == nullptr)
	{
		printf("もでるないぞ");
	}
	else{
		addPos = lpModel->getLpPos();
		AngleY = lpModel->getLpAngle();
		
	}
}

void Player::Update()
{//更新行う
	Move();
}
const struct MOVE{
	constatic int kRIGHT = VK_RIGHT;
	constatic int kLEFT = VK_LEFT;
	constatic int kADVANCE = VK_UP;
	constatic int kBACK = VK_DOWN;

	constatic JoypadCode jRIGHT = PAD_INPUT_RIGHT;
	constatic JoypadCode jLEFT = PAD_INPUT_LEFT;
	constatic JoypadCode jADVANCE = PAD_INPUT_UP;
	constatic JoypadCode jBACK = PAD_INPUT_DOWN;
};

void Player::Move()
{//プレイヤーを動かす
	float spd = PLAYER_SPEED;
	dxFor::xzDir dir;
	InputKey Input;

	/*	横移動	*/
	
	if (Input.InputKeyCode(MOVE::kRIGHT) or 
		Input.InputKeyCode(MOVE::jRIGHT))
	{ 
		dir.x++; 
		*AngleY = D3DXToRadian(-90.0f); 
	}
	else if (Input.InputKeyCode(MOVE::kLEFT) or 
		Input.InputKeyCode(MOVE::jLEFT))
	{ 
		dir.x--;
		*AngleY = D3DXToRadian(90.0f);
	}


	/*　縦移動　*/

	if (Input.InputKeyCode(MOVE::kADVANCE) or 
		Input.InputKeyCode(MOVE::jADVANCE))
	{ 
		dir.z++;
		*AngleY = D3DXToRadian(180.0f);
	}
	else if (Input.InputKeyCode(MOVE::kBACK) or 
		Input.InputKeyCode(MOVE::jBACK))
	{ 
		dir.z--; 
		*AngleY = D3DXToRadian(0.0f);
	}


	/* 入力成立 */

	if (dir.x != 0 or dir.z != 0)
	{
		Vec3 addSpd(spd * dir.x, 0.f, spd * dir.z);
		D3DXVec3Add(addPos, addPos, &addSpd);
		
		//変更行列
		Matrix mTrans;
		D3DXMatrixTranslation(&mTrans, addPos->x, addPos->y, addPos->z);
		//回転行列
		Matrix mRot;
		D3DXMatrixRotationY(&mRot, *AngleY);
		lpModel->setWorld(mRot * mTrans);
	}
}
