#include "ShadowMap.h"

#include "Camera.h"
#include "Light.h"
#include "Player.h"
#include "Model.h"

ShadowMap::ShadowMap()
{
	lpCam = nullptr;
	lpLight = nullptr;
	lpPlayer = nullptr;
}

void ShadowMap::Init(LPDIRECT3DDEVICE9 pd3d, Camera* lpCam, Light* lpLight, Player* lpPlayer)
{
	this->lpCam = lpCam;
	this->lpLight = lpLight;
	this->lpPlayer = lpPlayer;
	renderTarget.Create(pd3d,
		400,                    //横幅,
		400,                    //高さ,
		1,	                    //レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_A8R8G8B8,	    //カラーバッファのフォーマットはARGBの32bit
		D3DFMT_D16,	            //学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0	                    //マルチサンプリングしないので０を指定。
		);
	renderTarget.InitGeometry(pd3d);
}

void ShadowMap::Update()	//ライトビュープロジェクション行列を更新,
{
	//カメラの上方向を決める計算だけを入れておく,
	Vec3 tmp = viewTarget - viewPosition;
	dxFor::Vec3Normalize(&tmp);
	if (fabsf(tmp.y) > 0.99999f)
	{
		//カメラが真上か真下を向いている,
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &Vec3(1.0f, 0.0f, 0.0f));

	}

	else
	{
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &Vec3(0.0f, 1.0f, 0.0f));
	}

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&lightProjMatrix, D3DXToRadian(60.0f), 1.0f, 0.1f, 100.0f);
}

void ShadowMap::Draw(LPDIRECT3DDEVICE9 pd3d)
{
	LPDIRECT3DSURFACE9 renderTargetBackup;
	LPDIRECT3DSURFACE9 depthBufferBackup;
	//にりにりのレンダリングターゲットを保存,
	//後で戻す必要があるのでバックアップを行う,
	pd3d->GetRenderTarget(0, &renderTargetBackup);
	//にりにりのデブスステンシルバッファをバックアップする
	pd3d->GetDepthStencilSurface(&depthBufferBackup);
	
	//レンダリングターゲットを描画す,
	//renderTarget.Render(pd3d, *lpCam);

	//レンダリングターゲットを変更する,
	pd3d->SetRenderTarget(0, renderTarget.GetRenderTarget());
	pd3d->SetDepthStencilSurface(renderTarget.GetDepthStencilBuffer());

	//書き込み先を変更したのでクリア,
	pd3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255),1.0f,0);

	//シャドマップにレンダリンギ,
	lpPlayer->getLpPlayerModel()->setIsRenderShadowMap(true);
	lpPlayer->getLpPlayerModel()->Render(pd3d, *lpCam, *lpLight);
	lpPlayer->getLpPlayerModel()->setIsRenderShadowMap(false);


	//戻す
	pd3d->SetRenderTarget(0, renderTargetBackup);
	pd3d->SetDepthStencilSurface(depthBufferBackup);
}