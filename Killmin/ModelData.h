#pragma once
/*
	燃えるデータ
*/
#include "System.h"

//D3DXFRAMEの派生構造体
struct D3DXFRAME_DERIVED : public D3DXFRAME
{//D3DXFRAME.. トランスフォーム フレーム階層内の
//トランスフォーム フレームをカプセル化する。
	D3DXMATRIXA16	CombinedTransformationMatrix; //変換した行列を合成した奴
};

//D3DXMESHCONTAINERの派生構造体
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{//D3DXMESHCONTAINER.. トランスフォーム フレーム階層内の
//					メッシュ オブジェクトをカプセル化する。
	LPDIRECT3DTEXTURE9* ppTextures;
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD NumAttributeGroups;
	DWORD NumInfl;
	LPD3DXBUFFER pBoneCombinationBuf;
	Matrix** ppBoneMatrixPtrs;
	Matrix* pBoneOffsetMatrices;
	DWORD NumPaletteEntries;
	bool UseSoftwareVP;
	DWORD iAttributeSW;
};

class Animation;

class ModelData
{
	ModelData();  //こんすと
	~ModelData();  //ですと

	//第一引数：[入力]ファイルパス
	//第二引数：[出力]アニーメション付きモデルデータの場合、アニメーションクラスも構築されるます
	void LoadModelData(const char* filePath, Animation* anim);
	
	void Release();	//りりーす

	void UpdateBoneMatrix(const Matrix& matWorld); //ボーン行列を更新

//get
	LPD3DXFRAME getFrameRoot()
	{
		return frameRoot;
	}
	ID3DXAnimationController* getAnimationController()
	{
		return lpAnimController;
	}
private:
	LPD3DXFRAME frameRoot;  //フレームルート
	ID3DXAnimationController* lpAnimController;  //アニメーションコントローラ
};
