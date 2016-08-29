#pragma once
#include "System.h"

//���ł����炤
class Model{
public:
	Model();
	~Model();
	void Init(LPDIRECT3DDEVICE9 pd3dDevicde);

	void Update();

	void Model::Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4 ambientLight,
		int numDiffuseLight
		);

	void Release();

	void SetPosition(D3DXVECTOR3 pos);
protected:
	//�ǂݍ���X�t�@�C���w�肷
	virtual LPCTSTR FileName() = 0;

	D3DXVECTOR3 position;			//���W
	LPD3DXMESH mesh;				//���b�V��
	LPDIRECT3DTEXTURE9* textures;	//�e�N�X�`��
	DWORD numMaterial;				//�}�e���A��
	ID3DXEffect*	effect;			//�G�t�F�N�g
	D3DXMATRIX	mWorld;				//���[���h�s��
	D3DXMATRIX	mRotation;			//��]�s��

};