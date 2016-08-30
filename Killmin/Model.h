#pragma once
#include "System.h"
class Light;
class Camera;
//もでううらう
class Model{
public:
	Model();
	~Model();
	void Init(LPDIRECT3DDEVICE9 pd3dDevicde);

	void Update();

	void Model::Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera camera,Light light);

	void Release();

//set
	void SetPosition(D3DXVECTOR3 pos);
protected:
	//読み込むXファイル指定す
	virtual LPCTSTR FileName() = 0;

	D3DXVECTOR3 position;			//座標
	LPD3DXMESH mesh;				//メッシュ
	LPDIRECT3DTEXTURE9* textures;	//テクスチャ
	DWORD numMaterial;				//マテリアル
	ID3DXEffect*	effect;			//エフェクト
	D3DXMATRIX	mWorld;				//ワールド行列
	D3DXMATRIX	mRotation;			//回転行列

};