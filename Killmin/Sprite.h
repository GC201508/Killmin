#pragma once
#include "System.h"
class Camera;
class Sprite{
	/*!
	*@brief	頂点。
	*/
	struct SVertex
	{
		FLOAT x, y, z, w;
		DWORD color;
		FLOAT u, v;
	};

public:
	static const DWORD	D3DFVF_CUSTOMVERTEX = D3DFVF_XYZW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	/*!
	*@brief	コンストラクタ。
	*/
	Sprite();
	/*!
	*@brief	デストラクタ。
	*/
	~Sprite();
	/*!
	*@brief	解放。
	*/
	void Release();
	/*!
	*@brief	初期化。
	*/
	HRESULT Init(LPDIRECT3DDEVICE9 pd3dDevice);
	/*!
	*@brief	描画
	*/
	void Draw(
		Matrix worldMatrix,
		Camera cam,
		LPDIRECT3DTEXTURE9 texture
		);
private:
	LPDIRECT3DVERTEXBUFFER9 pVB;		//頂点バッファ。
	ID3DXEffect*			pSpriteEffect = NULL;
	ID3DXEffect*	effect;			//エフェクト
};