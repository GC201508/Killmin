#include "Player.h"
#include "Model.h"
#include "Camera.h"
#include "InputKey.h"
#define PLAYER_SPEED 0.03f
Player::Player()
{//初期化行う
	lpModel   = nullptr;
	lpCam     = nullptr;
	addPos    = nullptr;
	AngleY    = nullptr;
	direction = Vec3(0.f, 0.f, 0.f);
}
Player::~Player(){}

void Player::Init()
{//初期化 兼 ポインタの有無を調べる
	if (lpModel == nullptr){
		printf("もでるないぞ");
	}
	else{
		addPos = lpModel->lpPos;
		AngleY = lpModel->lpAngle;
	}

	if (lpCam == nullptr){
		printf("かめらないぞ");
	}
	else{
		camToPosUpdate();
		lpCam->setLpPlayerPos(lpModel->lpPos);
	}
}

void Player::Update()
{//更新行う
	camToPosUpdate();
	Move();
}
void Player::camToPosUpdate()
{//視点-注視点のベクトルを更新す
	D3DXVec3Subtract(&camToPos, &lpCam->GetEyePt(), &lpCam->GetLookatPt());
}
const struct MOVE{
	constatic int kRIGHT   = VK_RIGHT;
	constatic int kLEFT    = VK_LEFT;
	constatic int kADVANCE = VK_UP;
	constatic int kBACK    = VK_DOWN;

	constatic JoypadCode jRIGHT   = PAD_INPUT_RIGHT;
	constatic JoypadCode jLEFT    = PAD_INPUT_LEFT;
	constatic JoypadCode jADVANCE = PAD_INPUT_UP;
	constatic JoypadCode jBACK    = PAD_INPUT_DOWN;
};
const float fPI = 3.14159265358979323846264338327950288; //バタースコッチシナモンﾊﾟｲﾊﾟｲﾊﾟｰｲﾊﾟﾊﾟｲﾆ"ﾁｰﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯｽﾞｵｫ
void Player::Move()
{//プレイヤーを動かす
	dxFor::xzDir dir;
	InputKey Input;
	float spd     = PLAYER_SPEED;
	Vec3 LocalPos = *addPos;


	/*	横移動	*/

	if (Input.InputHoldCode(MOVE::kRIGHT) or
		Input.InputHoldCode(MOVE::jRIGHT))
	{
		dir.x--;
	}
	else if (Input.InputHoldCode(MOVE::kLEFT) or
		     Input.InputHoldCode(MOVE::jLEFT))
	{
		dir.x++;
	}
	

	/*　縦移動　*/

	if (Input.InputHoldCode(MOVE::kADVANCE) or
		Input.InputHoldCode(MOVE::jADVANCE))
	{
		dir.z--;
	}
	else if (Input.InputHoldCode(MOVE::kBACK) or
		     Input.InputHoldCode(MOVE::jBACK))
	{
		dir.z++;
	}


	/* 入力成立 */

	bool isMove = false;
	short ksk   = 1.f;
	if (onKey('F')){ ksk = 10.0f; }
	
	if (dir.x != 0)
	{
		Vec3 moveX;
		D3DXVec3Cross(&moveX, &vUp, &camToPos);
		dxFor::Vec3Normalize(&moveX);
		D3DXVec3Add(addPos, addPos, &(moveX * spd * dir.x * ksk));
		isMove = true;
	}
	if (dir.z != 0)
	{
		Vec3 moveZ = camToPos;
		moveZ.y    = 0.f;
		dxFor::Vec3Normalize(&moveZ);
		D3DXVec3Add(addPos, addPos, &(moveZ * spd * dir.z * ksk));
		isMove = true;

	}
	if (isMove)
	{
		/* 進行方向を更新 */

		D3DXVec3Subtract(&direction, addPos, &LocalPos);//移動後の位置 - 移動前の位置で進行方向を求める
		dxFor::Vec3Normalize(&direction); //正規化す
		float angle = 2 * fPI - atan2f(direction.z, direction.x);

		if (onKey('F')){ *AngleY += 0.7f; }
		else
		{
			*AngleY = angle - 0.5f * fPI;
		}
		
	}
}
