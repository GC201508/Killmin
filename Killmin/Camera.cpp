#include "Camera.h"
#define CamRotSpd 0.03f  //��]����
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
	Vec3 vUP(0.0f, 1.0f, 0.0f);
	Vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vUP, &toPos);
	dxFor::Vec3Normalize(&vAxis);
	float spd = CamRotSpd;

	if (onKey(RotLEFT) || onKey(RotRIGHT))
	{
		if (onKey(RotLEFT))
		{// ,�L�[�A����]
			spd *= 1.f;
		}
		else if (onKey(RotRIGHT))
		{// .�L�[�A�E��]
			spd *= -1.f;
		}
		D3DXMatrixRotationY(&mRot, spd);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}

	if (onKey(RotUP) || onKey(RotDOWN))
	{
		spd = CamRotSpd;

		if (onKey(RotUP))
		{// [�L�[�A���]
			spd *= -1.f;
		}
		else if (onKey(RotDOWN))
		{// ]�L�[�A����]
			spd *= 1.f;
		}
		D3DXMatrixRotationAxis(&mRot, &vAxis, spd);
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
	Vec3 toPos;
	Vec3 vUP(0.0f, 1.0f, 0.0f);
	float spd = CamMoveSpd;

	//��
	if (onKey(MoveRIGHT))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUP, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * -spd;
		vEyePt += toPos * -spd;
	}
	//��
	else if (onKey(MoveLEFT))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUP, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd;
		vEyePt += toPos * spd;
	}

	//�S�g
	if (onKey(MoveADVANCE))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * -spd;
		vEyePt += toPos * -spd;
	}
	//�|��
	else if (onKey(MoveBACK))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd;
		vEyePt += toPos * spd;
	}

	//���
	if (onKey(MoveUP))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		vLookatPt.y += spd;
		vEyePt.y += spd;
	}
	//���k��
	else if (onKey(MoveDOWN))
	{/*�����_�ƃJ�����̈ʒu�𓯎��ɓ�����*/
		vLookatPt.y += -spd;
		vEyePt.y += -spd;
	}
}