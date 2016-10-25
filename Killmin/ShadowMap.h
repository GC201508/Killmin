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
public:
	ShadowMap();

	void Init(LPDIRECT3DDEVICE9,Camera*,Light*,Player*);
	void Update();
	void Draw(LPDIRECT3DDEVICE9);
};