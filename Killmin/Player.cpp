#include "Player.h"
#include "Model.h"
#include "Camera.h"
//#include "InputKey.h"	//TODO:2016/10/13 XInput�𓱓������̂ō폜
#define PLAYER_SPEED 0.03f
Player::Player()
{//�������s��
	lpModel     = nullptr;
	lpCam       = nullptr;
	lpShadowMap = nullptr;
	addPos      = nullptr;
	AngleY      = nullptr;
	direction   = Vec3(0.f, 0.f, 0.f);
}
Player::~Player(){}

void Player::Init()
{//������ �� �|�C���^�̗L���𒲂ׂ�
	if (lpModel == nullptr){
		printf("���ł�Ȃ���");	
	}
	else{
		addPos = lpModel->lpPos();
		AngleY = lpModel->lpAngle();
		lpModel->setIsReversalModel(true);
		lpModel->setIsToonShader(true);
		if (lpShadowMap == nullptr)
		{
			printf("����ǂ܂��ՂȂ���");
		}
		else{
			lpModel->setShadowMap(lpShadowMap);
		}
	}

	if (lpCam == nullptr){
		printf("���߂�Ȃ���");
	}
	else{
		camToPosUpdate();
		lpCam->setLpPlayerPos(lpModel->lpPos());
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

const EnButton KSK_MOVE = enButtonA;
const float fPI = 3.14159265358979323846264338327950288; //�o�^�[�X�R�b�`�V�i�����߲�߲�߰����߲�"�����������������޵�
void Player::Move()
{//�v���C���[�𓮂���
	dxFor::xzDir dir;
	InputKey Input;
	float spd     = PLAYER_SPEED;
	Vec3 LocalPos = *addPos;

	dir.x = -XInput->getLeftStickX() * spd;
	dir.z = -XInput->getLeftStickY() * spd;

	/* ���͐��� */

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
		/* �i�s�������X�V */

		D3DXVec3Subtract(&direction, addPos, &LocalPos);//�ړ���̈ʒu - �ړ��O�̈ʒu�Ői�s���������߂�
		dxFor::Vec3Normalize(&direction); //���K����
		float angle = 2 * fPI - atan2f(direction.z, direction.x);

		if (XInput->IsPress(KSK_MOVE)){ *AngleY += 0.7f; }
		else
		{
			*AngleY = angle - 0.5f * fPI;
		}
		
	}
}
