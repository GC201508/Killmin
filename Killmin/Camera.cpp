#include "Camera.h"
#define CamRotSpd 0.01f  //回転速さ
#define CamMoveSpd 0.04f  //動く速さ

//コンストラクタ。
Camera::Camera()
{
	Near        = 0.1f;
	Far         = 1000.0f;
	aspect      = 1.8f;
	camState    = CS$HoldPlayer;
	lpPlayerPos = nullptr;
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
//カメラの初期化。
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
{//カメラ回せ回せ
	Matrix mRot;
	Vec3 vAxis;
	D3DXVec3Cross(&vAxis, &vUp, &toPos);
	dxFor::Vec3Normalize(&vAxis);
	float spd = CamRotSpd;
	dxFor::xzDir Dir;

	Dir.x = XInput->getRightStickX();
	Dir.z = XInput->getRightStickY();

	if (Dir.x != 0.f)
	{//横回転
		D3DXMatrixRotationY(&mRot, spd * Dir.x);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}

	if (Dir.z != 0.f)
	{//縦回転
		D3DXMatrixRotationAxis(&mRot, &vAxis, spd * Dir.z);
		dxFor::Vec3Transform(&toPos, &mRot);
		D3DXVec3Add(&vEyePt, &vLookatPt, &toPos);
	}
}

void Camera::MoveCamera()
{//カメラ動かせ動かせ
/*注視点とカメラの位置を同時に動かす*/
	float spd = CamMoveSpd;
	Vec3 dir(0.f,0.f,0.f);
	Vec3 ToPos = toPos;

	if (XInput->IsPress(enButtonRight) or
		XInput->IsPress(enButtonLeft))
	{//左右
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
	{//前後
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
	{//上下
		if (XInput->IsPress(enButtonUp)){ dir.y++; }
		else if (XInput->IsPress(enButtonDown)){ dir.y--; }
		vLookatPt.y += spd * dir.y;
		vEyePt.y    += spd * dir.y;
	}
}

void Camera::HoldPlayerCamera()
{//常にプレイヤーを追尾するCAMERA

	vLookatPt = *lpPlayerPos; //Player位置に注視点を置き続ける
	vEyePt    = vLookatPt + toPos;
}