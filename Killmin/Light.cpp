#include "Light.h"

Light::Light(){}
Light::~Light(){}

void Light::Init()
{//ライト初期化す
	ZeroMemory(diffuseLightDirection, sizeof(diffuseLightDirection));
	ZeroMemory(diffuseLightColor, sizeof(diffuseLightColor));

	diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 0.0f);
	diffuseLightDirection[1] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	diffuseLightDirection[2] = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 1.0f);
	diffuseLightDirection[3] = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);

	//ディフューズライト。
	diffuseLightColor[0] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 2.0f);
	diffuseLightColor[1] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	diffuseLightColor[2] = D3DXVECTOR4(0.5f, 0.4f, 0.6f, 1.0f);
	diffuseLightColor[3] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//環境光。
	ambientLight = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
}

void Light::Update()
{//ライト更新す
	D3DXMATRIX mRot;
	//単位行列に初期化する。
	D3DXMatrixIdentity(&mRot);
	//ライトを回す。
	D3DXMatrixRotationY(&mRot, 0.007f);

	for (int i = 0; i < lightNum; i++)
	{
		D3DXVec4Transform(&diffuseLightDirection[i], &diffuseLightDirection[i], &mRot);
	}
	
}