#include "Camera.h"
#define CamRotSpd 0.01f  //��]����
#define CamMoveSpd 0.04f  //��������

//�R���X�g���N�^�B
Camera::Camera()
{
	Near        = 0.1f;
	Far         = 1000.0f;
	aspect      = 1.8f;
	camState    = CS$HoldPlayer;
	lpPlayerPos = nullptr;
}
//�f�X�g���N�^
Camera::~Camera()
{
}
//�A�X�y�N�g���ݒ�B
void Camera::SetAspect(float _aspect)
{
	aspect = _aspect;
}
//�A�X�y�N�g����擾�B
float Camera::GetAspect()
{
	return aspect;
}
//�t�@�[��ݒ�B
void Camera::SetFar(float _far)
{
	Far = _far;
}
//�j�A�[�̎擾�B
float Camera::GetNear()
{
	return Near;
}
//�t�@�[�̎擾�B
float Camera::GetFar()
{
	return Far;
}
//�j�A�[��ݒ�B
void Camera::SetNear(float _near)
{
	Near = _near;
}
//���_�̃Z�b�^�[
void Camera::SetEyePt(D3DXVECTOR3 pt)
{
	vEyePt = pt;
}
//���_�̃Q�b�^�[
D3DXVECTOR3 Camera::GetEyePt()
{
	return vEyePt;
}
//�����_�̃Z�b�^�[
void Camera::SetLookatPt(D3DXVECTOR3 pt)
{
	vLookatPt = pt;
}
//�����_�̃Q�b�^�[�B
D3DXVECTOR3 Camera::GetLookatPt()
{
	return vLookatPt;
}
//������̃Z�b�^�[
void Camera::SetUpVec(D3DXVECTOR3 up)
{
	vUpVec = up;
}
//������̃Q�b�^�[
D3DXVECTOR3 Camera::GetUpVec()
{
	return vUpVec;
}
//�r���[�s��̃Z�b�^�[�B
void Camera::SetViewMatrix(D3DXMATRIX mView)
{
	viewMatrix = mView;
}
//�r���[�s��̃Q�b�^�[
D3DXMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}
//�v���W�F�N�V�����s��̃Z�b�^�[
void Camera::SetProjectionMatrix(D3DXMATRIX mProj)
{
	projectionMatrix = mProj;
}
//�v���W�F�N�V�����s��̃Q�b�^�[
D3DXMATRIX Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
//�J�����̍X�V�����B
const int camChange = 'Q';
void Camera::Update()
{
	switch (camState)
	{
	case CS$Free:
		KaitenCamera();
		MoveCamera();
		if (Input.InputPushCode(camChange)){ camState = CS$HoldPlayer; }
		break;

	case CS$HoldPlayer:
		HoldPlayerCamera();
		KaitenCamera();
		if (Input.InputPushCode(camChange)){ camState = CS$Free; }
		break;

	default:
		break;
	}
	D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, aspect, Near, Far);
}
//�J�����̏������B
void Camera::Init()
{
	//vEyePt = D3DXVECTOR3(0.0f, 2.0f, -10.0f);
	vEyePt    = D3DXVECTOR3(0.0f, 1.0f, -5.0f);
	vLookatPt = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	vUpVec    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
	Update();
}

void Camera::KaitenCamera()
{//�J�����񂹉�
	Matrix mRot;
	Vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vUp, &toPos);
	dxFor::Vec3Normalize(&vAxis);
	float spd = CamRotSpd;
	dxFor::xzDir Dir;

	Dir.x = XInput->getRightStickX();
	Dir.z = XInput->getRightStickY();

	if (Dir.x != 0.f)
	{//����]
		D3DXMatrixRotationY(&mRot, spd * Dir.x);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}

	if (Dir.z != 0.f)
	{//�c��]
		D3DXMatrixRotationAxis(&mRot, &vAxis, spd * Dir.z);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}
}

void Camera::MoveCamera()
{//�J����������������
/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
	float spd = CamMoveSpd;
	Vec3 dir(0.f,0.f,0.f);
	Vec3 ToPos = toPos;

	if (XInput->IsPress(enButtonRight) or
		XInput->IsPress(enButtonLeft))
	{//���E
		if (XInput->IsPress(enButtonRight)){ dir.x--; }
		else if (XInput->IsPress(enButtonLeft)){ dir.x++; }
		D3DXVec3Subtract(&ToPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&ToPos, &vUp, &ToPos);
		dxFor::Vec3Normalize(&ToPos);
		vLookatPt += ToPos * spd * dir.x;
		vEyePt += ToPos * spd * dir.x;
	}

	if (
		(!XInput->IsPress(enButtonRB3) and
		  XInput->IsPress(enButtonUp)) or

		(!XInput->IsPress(enButtonRB3) and
		  XInput->IsPress(enButtonDown))
		)
	{//�O��
		if ((!XInput->IsPress(enButtonRB3) and
			  XInput->IsPress(enButtonUp)))
		{ dir.z--; }
		else if ((!XInput->IsPress(enButtonRB3) and
			       XInput->IsPress(enButtonDown)))
		{ dir.z++; }

		D3DXVec3Subtract(&ToPos, &vEyePt, &vLookatPt);
		ToPos.y = 0.f;
		dxFor::Vec3Normalize(&ToPos);
		vLookatPt += ToPos * spd * dir.z;
		vEyePt    += ToPos * spd * dir.z;
	}

	if (
		(XInput->IsPress(enButtonRB3) and
		 XInput->IsPress(enButtonUp)) or

		(XInput->IsPress(enButtonRB3) and
		 XInput->IsPress(enButtonDown))
		)
	{//�㉺
		if (XInput->IsPress(enButtonUp)){ dir.y++; }
		else if (XInput->IsPress(enButtonDown)){ dir.y--; }
		vLookatPt.y += spd * dir.y;
		vEyePt.y    += spd * dir.y;
	}
}

void Camera::HoldPlayerCamera()
{//��Ƀv���C���[��ǔ�����CAMERA

	vLookatPt = *lpPlayerPos; //Player�ʒu�ɒ����_��u��������
	vEyePt    = vLookatPt + toPos;
}