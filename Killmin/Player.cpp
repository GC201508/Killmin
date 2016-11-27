#include "Player.h"
#include "Model.h"
#include "Camera.h"
//#include "InputKey.h"	//TODO:2016/10/13 XInputを導入したので削除
#define PLAYER_SPEED 0.03f
Player::Player()
{//初期化行う
	lpModel     = nullptr;
	lpCam       = nullptr;
	lpShadowMap = nullptr;
	addPos      = nullptr;
	AngleY      = nullptr;
	direction   = Vec3(0.f, 0.f, 0.f);
}
Player::~Player(){}

void Player::Init()
{//初期化 兼 ポインタの有無を調べる
	if (lpModel == nullptr){
		printf("もでるないぞ");	
	}
	else{
		addPos = lpModel->lpPos();
		AngleY = lpModel->lpAngle();
		lpModel->setIsReversalModel(true);
		lpModel->setIsToonShader(true);
		if (lpShadowMap == nullptr)
		{
			printf("しゃどまっぷないぞ");
		}
		else{
			lpModel->setShadowMap(lpShadowMap);
		}
	}

	if (lpCam == nullptr){
		printf("かめらないぞ");
	}
	else{
		camToPosUpdate();
		lpCam->setLpPlayerPos(lpModel->lpPos());
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

const EnButton KSK_MOVE = enButtonA;
const float fPI = 3.14159265358979323846264338327950288; //バタースコッチシナモンﾊﾟｲﾊﾟｲﾊﾟｰｲﾊﾟﾊﾟｲﾆ"ﾁｰﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯｽﾞｵｫ
void Player::Move()
{//プレイヤーを動かす
	dxFor::xzDir dir;
	InputKey Input;
	float spd     = PLAYER_SPEED;
	Vec3 LocalPos = *addPos;

	dir.x = -XInput->getLeftStickX() * spd;
	dir.z = -XInput->getLeftStickY() * spd;

	/* 入力成立 */

	bool isMove = false;
	short ksk   = 1.f;
	if (XInput->IsPress(KSK_MOVE)){ ksk = 10.0f; }
	
	if (dir.x != 0)
	{
		Vec3 moveX;
		D3DXVec3Cross(&moveX, &vUp, &camToPos);
		dxFor::Vec3Normalize(&moveX);
		D3DXVec3Add(addPos, addPos, &(moveX * dir.x * ksk));
		isMove = true;
	}
	if (dir.z != 0)
	{
		Vec3 moveZ = camToPos;
		moveZ.y    = 0.f;
		dxFor::Vec3Normalize(&moveZ);
		D3DXVec3Add(addPos, addPos, &(moveZ  * dir.z * ksk));
		isMove = true;

	}
	if (isMove)
	{
		/* 進行方向を更新 */

		D3DXVec3Subtract(&direction, addPos, &LocalPos);//移動後の位置 - 移動前の位置で進行方向を求める
		dxFor::Vec3Normalize(&direction); //正規化す
		float angle = 2 * fPI - atan2f(direction.z, direction.x);

		if (XInput->IsPress(KSK_MOVE)){ *AngleY += 0.7f; }
		else
		{
			*AngleY = angle - 0.5f * fPI;
		}
		
	}
}
