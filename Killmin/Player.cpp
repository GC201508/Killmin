#include "Player.h"
#include "Model.h"
#include "Camera.h"
#include "InputKey.h"
#define PLAYER_SPEED 0.03f
Player::Player()
{//�������s��
	lpModel = nullptr;
	lpCam = nullptr;
	addPos = nullptr;
	AngleY = nullptr;
	direction = Vec3(0.f, 0.f, 0.f);
}
Player::~Player(){}

void Player::Init()
{//������ �� �|�C���^�̗L���𒲂ׂ�
	if (lpModel == nullptr){
		printf("���ł�Ȃ���");
	}
	else{
		addPos = lpModel->lpPos;
		AngleY = lpModel->lpAngle;
	}

	if (lpCam == nullptr){
		printf("���߂�Ȃ���");
	}
	else{
		camToPosUpdate();
	}
}

void Player::Update()
{//�X�V�s��
	camToPosUpdate();
	Move();
}
void Player::camToPosUpdate()
{//���_-�����_�̃x�N�g�����X�V��

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
const float fPI = 3.14159265358979323846264338327950288; //�o�^�[�X�R�b�`�V�i�����߲�߲�߰����߲�"�����������������޵�
void Player::Move()
{//�v���C���[�𓮂���
	float spd = PLAYER_SPEED;
	dxFor::xzDir dir;
	InputKey Input;
	Vec3 LocalPos = *addPos;

	/*	���ړ�	*/

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

	/*�@�c�ړ��@*/

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


	/* ���͐��� */

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
		/* �i�s�������X�V */

		direction = *addPos - LocalPos;	//�ړ���̈ʒu - �ړ��O�̈ʒu�Ői�s���������߂�
		D3DXVec3Normalize(&direction, &direction); //���K����
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;
	}
}
