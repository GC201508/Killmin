#pragma once

#include "System.h"

class Tex2d
{
public:
	Tex2d();
	~Tex2d();
	virtual void Initialize();	//
	virtual void Update() = 0;	//	この３つはシーンの元となるインタフェース
	void Draw(LPD3DXSPRITE);	//

	void Draw(LPD3DXSPRITE, D3DXVECTOR3& position, RECT& rect, D3DXVECTOR2& center);

	void SetupMatrices();
protected:

	D3DXIMAGE_INFO imgInfo;

	D3DXVECTOR2        position;			    //位置情報
	float              angle;					//回転情報
	D3DXVECTOR2        scale;			        //スケール情報
	LPDIRECT3DTEXTURE9 pTexture;	            //テクスチャ
	D3DCOLOR           backColor;			    //背景カラー
	LPCTSTR            texFileName;				//テクスチャファイル名
	D3DXVECTOR2        texCenter;		        //テクスチャの中点
	RECT               rect;					//描画矩形
	D3DXMATRIX         transformMatrix;		    //変換行列
};
