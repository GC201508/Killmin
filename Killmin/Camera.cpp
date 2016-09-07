#include "Camera.h"
#define CamRotSpd 0.03f  //回転速さ
#define CamMoveSpd 0.04f  //動く速さ

//コンストラクタ。
Camera::Camera()
{
	Near = 0.1f;
	Far = 100.0f;
	aspect = 1.8f;
}
//デストラクタ
Camera::~Camera()
{
}
//アスペクト比を設定。
void Camera::SetAspect(float _aspect)
{
	aspect = _aspect;
}
//アスペクト比を取得。
float Camera::GetAspect()
{
	return aspect;
}
//ファーを設定。
void Camera::SetFar(float _far)
{
	Far = _far;
}
//ニアーの取得。
float Camera::GetNear()
{
	return Near;
}
//ファーの取得。
float Camera::GetFar()
{
	return Far;
}
//ニアーを設定。
void Camera::SetNear(float _near)
{
	Near = _near;
}
//視点のセッター
void Camera::SetEyePt(D3DXVECTOR3 pt)
{
	vEyePt = pt;
}
//視点のゲッター
D3DXVECTOR3 Camera::GetEyePt()
{
	return vEyePt;
}
//注視点のセッター
void Camera::SetLookatPt(D3DXVECTOR3 pt)
{
	vLookatPt = pt;
}
//注視点のゲッター。
D3DXVECTOR3 Camera::GetLookatPt()
{
	return vLookatPt;
}
//上方向のセッター
void Camera::SetUpVec(D3DXVECTOR3 up)
{
	vUpVec = up;
}
//上方向のゲッター
D3DXVECTOR3 Camera::GetUpVec()
{
	return vUpVec;
}
//ビュー行列のセッター。
void Camera::SetViewMatrix(D3DXMATRIX mView)
{
	viewMatrix = mView;
}
//ビュー行列のゲッター
D3DXMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}
//プロジェクション行列のセッター
void Camera::SetProjectionMatrix(D3DXMATRIX mProj)
{
	projectionMatrix = mProj;
}
//プロジェクション行列のゲッター
D3DXMATRIX Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
//カメラの更新処理。
void Camera::Update()
{
	KaitenCamera();
	MoveCamera();
	D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, aspect, Near, Far);
}
//カメラの初期化。
void Camera::Init()
{
	vEyePt = D3DXVECTOR3(0.0f, 2.0f, -10.0f);
	vLookatPt = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Update();
}
const int RotLEFT = 188; //左回転 ＜キー
const int RotRIGHT = 190;//右回転 ＞キー
const int RotUP = 219;	//上回転 [キー
const int RotDOWN = 221;//下回転 ]キー
void Camera::KaitenCamera()
{//カメラ回せ回せ
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
		{// ,キー、左回転
			spd *= 1.f;
		}
		else if (onKey(RotRIGHT))
		{// .キー、右回転
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
		{// [キー、上回転
			spd *= -1.f;
		}
		else if (onKey(RotDOWN))
		{// ]キー、下回転
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
{//カメラ動かせ動かせ
	Vec3 toPos;
	Vec3 vUP(0.0f, 1.0f, 0.0f);
	float spd = CamMoveSpd;

	//→
	if (onKey(MoveRIGHT))
	{/*注視点とカメラの位置を同時に動かす*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUP, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * -spd;
		vEyePt += toPos * -spd;
	}
	//←
	else if (onKey(MoveLEFT))
	{/*注視点とカメラの位置を同時に動かす*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUP, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd;
		vEyePt += toPos * spd;
	}

	//全身
	if (onKey(MoveADVANCE))
	{/*注視点とカメラの位置を同時に動かす*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * -spd;
		vEyePt += toPos * -spd;
	}
	//鋼体
	else if (onKey(MoveBACK))
	{/*注視点とカメラの位置を同時に動かす*/
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd;
		vEyePt += toPos * spd;
	}

	//上野
	if (onKey(MoveUP))
	{/*注視点とカメラの位置を同時に動かす*/
		vLookatPt.y += spd;
		vEyePt.y += spd;
	}
	//下北沢
	else if (onKey(MoveDOWN))
	{/*注視点とカメラの位置を同時に動かす*/
		vLookatPt.y += -spd;
		vEyePt.y += -spd;
	}
}