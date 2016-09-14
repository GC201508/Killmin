#pragma once
/*
	�R����f�[�^
*/
#include "System.h"

//D3DXFRAME�̔h���\����
struct D3DXFRAME_DERIVED : public D3DXFRAME
{//D3DXFRAME.. �g�����X�t�H�[�� �t���[���K�w����
//�g�����X�t�H�[�� �t���[�����J�v�Z��������B
	D3DXMATRIXA16	CombinedTransformationMatrix; //�ϊ������s������������z
};

//D3DXMESHCONTAINER�̔h���\����
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{//D3DXMESHCONTAINER.. �g�����X�t�H�[�� �t���[���K�w����
//					���b�V�� �I�u�W�F�N�g���J�v�Z��������B
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
	ModelData();  //���񂷂�
	~ModelData();  //�ł���

	//�������F[����]�t�@�C���p�X
	//�������F[�o��]�A�j�[���V�����t�����f���f�[�^�̏ꍇ�A�A�j���[�V�����N���X���\�z�����܂�
	void LoadModelData(const char* filePath, Animation* anim);
	
	void Release();	//���[��

	void UpdateBoneMatrix(const Matrix& matWorld); //�{�[���s����X�V

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
	LPD3DXFRAME frameRoot;  //�t���[�����[�g
	ID3DXAnimationController* lpAnimController;  //�A�j���[�V�����R���g���[��
};
