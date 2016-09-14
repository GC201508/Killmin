#include "Player.h"
#include "Model.h"
#include "Camera.h"
#include "InputKey.h"
#define PLAYER_SPEED 0.03f
Player::Player()
{//初期化行う
	lpModel = nullptr;
	lpCam = nullptr;
	addPos = nullptr;
	AngleY = nullptr;
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
	constatic int kRIGHT = VK_RIGHT;
	constatic int kLEFT = VK_LEFT;
	constatic int kADVANCE = VK_UP;
	constatic int kBACK = VK_DOWN;

	constatic JoypadCode jRIGHT = PAD_INPUT_RIGHT;
	constatic JoypadCode jLEFT = PAD_INPUT_LEFT;
	constatic JoypadCode jADVANCE = PAD_INPUT_UP;
	constatic JoypadCode jBACK = PAD_INPUT_DOWN;
};
const float fPI = 3.14159265358979323846264338327950288; //バタースコッチシナモンﾊﾟｲﾊﾟｲﾊﾟｰｲﾊﾟﾊﾟｲﾆ"ﾁｰﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯｽﾞｵｫ
void Player::Move()
{//プレイヤーを動かす
	float spd = PLAYER_SPEED;
	dxFor::xzDir dir;
	InputKey Input;
	Vec3 LocalPos = *addPos;

	/*	横移動	*/

	if (Input.InputKeyCode(MOVE::kRIGHT) or
		Input.InputKeyCode(MOVE::jRIGHT))
	{
		dir.x--;
	}
	else if (Input.InputKeyCode(MOVE::kLEFT) or
		Input.InputKeyCode(MOVE::jLEFT))
	{
		dir.x++;
	}

	/*　縦移動　*/

	if (Input.InputKeyCode(MOVE::kADVANCE) or
		Input.InputKeyCode(MOVE::jADVANCE))
	{
		dir.z--;
	}
	else if (Input.InputKeyCode(MOVE::kBACK) or
		Input.InputKeyCode(MOVE::jBACK))
	{
		dir.z++;
	}


	/* 入力成立 */

	bool isMove = false;
	
	if (dir.x != 0)
	{
		Vec3 moveX;
		D3DXVec3Cross(&moveX, &vUp, &camToPos);
		dxFor::Vec3Normalize(&moveX);
		D3DXVec3Add(addPos, addPos, &(moveX * spd * dir.x));
		isMove = true;
	}
	if (dir.z != 0)
	{
		Vec3 moveZ = camToPos;
		moveZ.y = 0.f;
		dxFor::Vec3Normalize(&moveZ);
		D3DXVec3Add(addPos, addPos, &(moveZ * spd * dir.z));
		isMove = true;
	}
	if (isMove)
	{
		/* 進行方向を更新 */

		direction = *addPos - LocalPos;	//移動後の位置 - 移動前の位置で進行方向を求める
		D3DXVec3Normalize(&direction, &direction); //正規化す
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;
	}
}
