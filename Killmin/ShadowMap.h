/*
	�e�}�b�v

	���Q�[���ł͂����́��e���g��
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
	RenderTarget renderTarget;	//�V���h�E�}�b�v���������ރ����_�����O�^�[�Q�b�g,
	Matrix	lightViewMatrix;	//���C�g�r���[�}�g���N�X,
	Matrix	lightProjMatrix;	//���C�g�v���W�F�N�V�����}�g���N�X,
	Vec3	viewPosition;		//���C�g�r���[�̎��_,
	Vec3	viewTarget;			//���C�g�r���[�̒����_,

	Camera* lpCam;	//�J����
	Light* lpLight;	//���C�g
	Player* lpPlayer; //�v���C���[
	Model* lpModel;	//���f��
public:
	ShadowMap();

	void Init(LPDIRECT3DDEVICE9,Camera*,Light*,Model*);
	void Update();
	void Draw(LPDIRECT3DDEVICE9);

//get
	Matrix*	lpLightViewMatrix = &lightViewMatrix;	//���C�g�r���[�}�g���N�X,
	Matrix*	lpLightProjMatrix = &lightProjMatrix;	//���C�g�v���W�F�N�V�����}�g���N�X,
	LPDIRECT3DTEXTURE9 getShadowMapRenderTex(){ return renderTarget.GetTexture(); }	//ShadowMap��RenderTexture,
};