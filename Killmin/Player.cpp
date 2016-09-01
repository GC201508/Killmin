#include "Player.h"
#include "Model.h"

Player::Player()
{//�������s��
	lpModel = nullptr;
}
Player::~Player(){}

void Player::Init()
{//�������s��
	if (lpModel == nullptr)
	{
		printf("�Ȃ���");
	}
	else{
		
	}
}

void Player::Update()
{//�X�V�s��
	onKeyMove();
}

void Player::onKeyMove()
{
	static D3DXVECTOR3 addPos = lpModel->GetPosition();

	//��
	if (GetAsyncKeyState(68))
	{
		addPos.x += 0.1f;
	}
	//��
	else if (GetAsyncKeyState(65))
	{
		addPos.x += -0.1f;
	}
	//�O�i
	if (GetAsyncKeyState(87))
	{
		addPos.z += 0.1f;
	}
	//���
	else if (GetAsyncKeyState(83))
	{
		addPos.z += -0.1f;
	}
	lpModel->SetPosition(addPos);
}