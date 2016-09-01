#include "Player.h"
#include "Model.h"

Player::Player()
{//初期化行う
	lpModel = nullptr;
}
Player::~Player(){}

void Player::Init()
{//初期化行う
	if (lpModel == nullptr)
	{
		printf("ないぞ");
	}
	else{
		
	}
}

void Player::Update()
{//更新行う
	onKeyMove();
}

void Player::onKeyMove()
{
	static D3DXVECTOR3 addPos = lpModel->GetPosition();

	//→
	if (GetAsyncKeyState(68))
	{
		addPos.x += 0.1f;
	}
	//←
	else if (GetAsyncKeyState(65))
	{
		addPos.x += -0.1f;
	}
	//前進
	if (GetAsyncKeyState(87))
	{
		addPos.z += 0.1f;
	}
	//後退
	else if (GetAsyncKeyState(83))
	{
		addPos.z += -0.1f;
	}
	lpModel->SetPosition(addPos);
}