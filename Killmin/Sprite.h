#pragma once
#include "System.h"
class Camera;
class Sprite{
	/*!
	*@brief	���_�B
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
	*@brief	�R���X�g���N�^�B
	*/
	Sprite();
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~Sprite();
	/*!
	*@brief	����B
	*/
	void Release();
	/*!
	*@brief	�������B
	*/
	HRESULT Init(LPDIRECT3DDEVICE9 pd3dDevice);
	/*!
	*@brief	�`��
	*/
	void Draw(
		Matrix worldMatrix,
		Camera cam,
		LPDIRECT3DTEXTURE9 texture
		);
private:
	LPDIRECT3DVERTEXBUFFER9 pVB;		//���_�o�b�t�@�B
	ID3DXEffect*			pSpriteEffect = NULL;
	ID3DXEffect*	effect;			//�G�t�F�N�g
};