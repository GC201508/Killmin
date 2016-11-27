#pragma once

#include "System.h"

class Tex2d
{
public:
	Tex2d();
	~Tex2d();
	virtual void Initialize();	//
	virtual void Update() = 0;	//	���̂R�̓V�[���̌��ƂȂ�C���^�t�F�[�X
	void Draw(LPD3DXSPRITE);	//

	void Draw(LPD3DXSPRITE, D3DXVECTOR3& position, RECT& rect, D3DXVECTOR2& center);

	void SetupMatrices();
protected:

	D3DXIMAGE_INFO imgInfo;

	D3DXVECTOR2        position;			    //�ʒu���
	float              angle;					//��]���
	D3DXVECTOR2        scale;			        //�X�P�[�����
	LPDIRECT3DTEXTURE9 pTexture;	            //�e�N�X�`��
	D3DCOLOR           backColor;			    //�w�i�J���[
	LPCTSTR            texFileName;				//�e�N�X�`���t�@�C����
	D3DXVECTOR2        texCenter;		        //�e�N�X�`���̒��_
	RECT               rect;					//�`���`
	D3DXMATRIX         transformMatrix;		    //�ϊ��s��
};
