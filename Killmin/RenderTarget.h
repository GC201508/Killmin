#pragma once
#include "System.h"

/*
*@brief	レンダリングターゲット。
*/
class Camera;
class RenderTarget {

	LPDIRECT3DSURFACE9	depthSurface;	//!<深度バッファ用のサーフェイス
	LPDIRECT3DTEXTURE9	texture;	//!<書き込み先のテクスチャ。
	LPDIRECT3DSURFACE9	surface;	//!<サーフェイス

	DWORD dwNumMaterials;
	LPDIRECT3DTEXTURE9*	pMeshTextures; 	// Textures for our mesh
	LPD3DXMESH pMesh;

	ID3DXEffect*	pTigerEffect;

public:
	RenderTarget();
	~RenderTarget();

	void Release(); //解放,

	void Create( //レンダリングターゲトを作る
		LPDIRECT3DDEVICE9 pd3dDevice,		// [in] pd3dDevice でゔぁいす
		int w,								// [in] w レンダリングターゲート幅,
		int h,								// [in] h レンダリンータグゲート高,
		int mipLevel,						// [in] mipLevel		ミップマップレベル,
		D3DFORMAT colorFormat,				// [in]	colorFormat	カラーバッファのフォーマット,
		D3DFORMAT depthStencilFormat,		// [in] depthStencilFormat	デプスステンシルバッファのフォーマット,
		D3DMULTISAMPLE_TYPE multiSampleType,// [in]	multiSampleType	マルチサンプリングの種類,
		int multiSampleQuality);			// [in]	multiSampleQuality	マルチサンプリングの品質。

	HRESULT InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice); //Xファイルを読み込んでジオメトリを初期化。

	void Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera cam);
	
	/*!
	*@brief	レンダリングターゲットを取得。
	*/
	LPDIRECT3DSURFACE9 GetRenderTarget()
	{
		return surface;
	}
	/*!
	*@brief	デプスステンシルバッファを取得。
	*/
	LPDIRECT3DSURFACE9 GetDepthStencilBuffer()
	{
		return depthSurface;
	}
	/*!
	*@brief	テクスチャを取得。
	*/
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return texture;
	}
};