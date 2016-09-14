#include "Camera.h"
#define CamRotSpd 0.01f  //��]����
#define CamMoveSpd 0.04f  //��������

//�R���X�g���N�^�B
Camera::Camera()
{
	Near = 0.1f;
	Far = 100.0f;
	aspect = 1.8f;
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
void Camera::Update()
{
	KaitenCamera();
	MoveCamera();
	D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, aspect, Near, Far);
}
//�J�����̏������B
void Camera::Init()
{
	vEyePt = D3DXVECTOR3(0.0f, 2.0f, -10.0f);
	vLookatPt = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Update();
}
const int RotLEFT = 188; //����] ���L�[
const int RotRIGHT = 190;//�E��] ���L�[
const int RotUP = 219;	//���] [�L�[
const int RotDOWN = 221;//����] ]�L�[
void Camera::KaitenCamera()
{//�J�����񂹉�
	Matrix mRot;
	Vec3 toPos;
	D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
	Vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vUp, &toPos);
	dxFor::Vec3Normalize(&vAxis);
	float spd = CamRotSpd;
	dxFor::xzDir Dir;

	if (onKey(RotLEFT) or onKey(RotRIGHT))
	{//����]
		if (onKey(RotLEFT)){ Dir.x++; }
		else if (onKey(RotRIGHT)){ Dir.x--;}
		D3DXMatrixRotationY(&mRot, spd * Dir.x);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}

	if (onKey(RotUP) or onKey(RotDOWN))
	{//�c��]
		if (onKey(RotUP)){ Dir.z--; }
		else if (onKey(RotDOWN)){ Dir.z++; }
		D3DXMatrixRotationAxis(&mRot, &vAxis, spd * Dir.z);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}
}
const char MoveLEFT = 'A';
const char MoveRIGHT = 'D';
const char MoveADVANCE = 'W';
const char MoveBACK = 'S';
const int MoveUP = VK_SPACE;
const int MoveDOWN = VK_LSHIFT;
void Camera::MoveCamera()
{//�J����������������
/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/

	Vec3 toPos;
	float spd = CamMoveSpd;
	Vec3 dir(0.f,0.f,0.f);

	if (onKey(MoveRIGHT) or onKey(MoveLEFT))
	{//���E
		if (onKey(MoveRIGHT)){ dir.x--; }
		else if (onKey(MoveLEFT)){ dir.x++; }
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUp, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd * dir.x;
		vEyePt += toPos * spd * dir.x;
	}

	if (onKey(MoveADVANCE) or onKey(MoveBACK))
	{//�O��
		if (onKey(MoveADVANCE)){ dir.z--; }
		else if (onKey(MoveBACK)){ dir.z++; }
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd * dir.z;
		vEyePt += toPos * spd * dir.z;
	}

	if (onKey(MoveUP) or onKey(MoveDOWN))
	{//�㉺
		if (onKey(MoveUP)){ dir.y++; }
		else if (onKey(MoveDOWN)){ dir.y--; }
		vLookatPt.y += spd * dir.y;
		vEyePt.y += spd * dir.y;
	}
}