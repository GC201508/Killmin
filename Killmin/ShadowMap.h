/*
	影マップ

	※ゲームではただの●影を使う
*/
#pragma once
#include "System.h"
#include "RenderTarget.h"
class Camera;
class Light;
class Player;
class Model;

class ShadowMap
{
	RenderTarget renderTarget;	//シャドウマップを書き込むレンダリングターゲット,
	Matrix	lightViewMatrix;	//ライトビューマトリクス,
	Matrix	lightProjMatrix;	//ライトプロジェクションマトリクス,
	Vec3	viewPosition;		//ライトビューの視点,
	Vec3	viewTarget;			//ライトビューの注視点,

	Camera* lpCam;	//カメラ
	Light* lpLight;	//ライト
	Player* lpPlayer; //プレイヤー
	Model* lpModel;	//モデル
public:
	ShadowMap();

	void Init(LPDIRECT3DDEVICE9,Camera*,Light*,Model*);
	void Update();
	void Draw(LPDIRECT3DDEVICE9);

//get
	Matrix*	lpLightViewMatrix = &lightViewMatrix;	//ライトビューマトリクス,
	Matrix*	lpLightProjMatrix = &lightProjMatrix;	//ライトプロジェクションマトリクス,
	LPDIRECT3DTEXTURE9 getShadowMapRenderTex(){ return renderTarget.GetTexture(); }	//ShadowMapのRenderTexture,
};