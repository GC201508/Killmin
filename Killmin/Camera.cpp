#include "Camera.h"
#define CamRotSpd 0.01f  //回転速さ
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
	Vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vUp, &toPos);
	dxFor::Vec3Normalize(&vAxis);
	float spd = CamRotSpd;
	dxFor::xzDir Dir;

	if (onKey(RotLEFT) or onKey(RotRIGHT))
	{//横回転
		if (onKey(RotLEFT)){ Dir.x++; }
		else if (onKey(RotRIGHT)){ Dir.x--;}
		D3DXMatrixRotationY(&mRot, spd * Dir.x);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}

	if (onKey(RotUP) or onKey(RotDOWN))
	{//縦回転
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
{//カメラ動かせ動かせ
/*注視点とカメラの位置を同時に動かす*/

	Vec3 toPos;
	float spd = CamMoveSpd;
	Vec3 dir(0.f,0.f,0.f);

	if (onKey(MoveRIGHT) or onKey(MoveLEFT))
	{//左右
		if (onKey(MoveRIGHT)){ dir.x--; }
		else if (onKey(MoveLEFT)){ dir.x++; }
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		D3DXVec3Cross(&toPos, &vUp, &toPos);
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd * dir.x;
		vEyePt += toPos * spd * dir.x;
	}

	if (onKey(MoveADVANCE) or onKey(MoveBACK))
	{//前後
		if (onKey(MoveADVANCE)){ dir.z--; }
		else if (onKey(MoveBACK)){ dir.z++; }
		D3DXVec3Subtract(&toPos, &vEyePt, &vLookatPt);
		toPos.y = 0.f;
		dxFor::Vec3Normalize(&toPos);
		vLookatPt += toPos * spd * dir.z;
		vEyePt += toPos * spd * dir.z;
	}

	if (onKey(MoveUP) or onKey(MoveDOWN))
	{//上下
		if (onKey(MoveUP)){ dir.y++; }
		else if (onKey(MoveDOWN)){ dir.y--; }
		vLookatPt.y += spd * dir.y;
		vEyePt.y += spd * dir.y;
	}
}