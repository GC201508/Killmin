#include "ShadowMap.h"

#include "Camera.h"
#include "Light.h"
#include "Player.h"
#include "Model.h"

ShadowMap::ShadowMap()
{
	lpCam = nullptr;
	lpLight = nullptr;
	lpPlayer = nullptr;
}

void ShadowMap::Init(LPDIRECT3DDEVICE9 pd3d, Camera* lpCam, Light* lpLight, Player* lpPlayer)
{
	this->lpCam = lpCam;
	this->lpLight = lpLight;
	this->lpPlayer = lpPlayer;
	renderTarget.Create(pd3d,
		400,                    //����,
		400,                    //����,
		1,	                    //�����_�����O�^�[�Q�b�g�ɂ̓~�b�v�}�b�v�͕s�v�Ȃ̂ňꖇ�̂݁B
		D3DFMT_A8R8G8B8,	    //�J���[�o�b�t�@�̃t�H�[�}�b�g��ARGB��32bit
		D3DFMT_D16,	            //�w���̂o�b��24bit�̐[�x�o�b�t�@���쐬�ł��Ȃ������̂ŁA16�r�b�g�Ő[�x�o�b�t�@���쐬����B
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�͂Ȃ��B
		0	                    //�}���`�T���v�����O���Ȃ��̂łO���w��B
		);
	renderTarget.InitGeometry(pd3d);
}

void ShadowMap::Update()	//���C�g�r���[�v���W�F�N�V�����s����X�V,
{
	//�J�����̏���������߂�v�Z���������Ă���,
	Vec3 tmp = viewTarget - viewPosition;
	dxFor::Vec3Normalize(&tmp);
	if (fabsf(tmp.y) > 0.99999f)
	{
		//�J�������^�ォ�^���������Ă���,
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &Vec3(1.0f, 0.0f, 0.0f));

	}

	else
	{
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &Vec3(0.0f, 1.0f, 0.0f));
	}

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&lightProjMatrix, D3DXToRadian(60.0f), 1.0f, 0.1f, 100.0f);
}

void ShadowMap::Draw(LPDIRECT3DDEVICE9 pd3d)
{
	LPDIRECT3DSURFACE9 renderTargetBackup;
	LPDIRECT3DSURFACE9 depthBufferBackup;
	//�ɂ�ɂ�̃����_�����O�^�[�Q�b�g��ۑ�,
	//��Ŗ߂��K�v������̂Ńo�b�N�A�b�v���s��,
	pd3d->GetRenderTarget(0, &renderTargetBackup);
	//�ɂ�ɂ�̃f�u�X�X�e���V���o�b�t�@���o�b�N�A�b�v����
	pd3d->GetDepthStencilSurface(&depthBufferBackup);
	
	//�����_�����O�^�[�Q�b�g��`�悷,
	//renderTarget.Render(pd3d, *lpCam);

	//�����_�����O�^�[�Q�b�g��ύX����,
	pd3d->SetRenderTarget(0, renderTarget.GetRenderTarget());
	pd3d->SetDepthStencilSurface(renderTarget.GetDepthStencilBuffer());

	//�������ݐ��ύX�����̂ŃN���A,
	pd3d->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255),1.0f,0);

	//�V���h�}�b�v�Ƀ����_�����M,
	lpPlayer->getLpPlayerModel()->setIsRenderShadowMap(true);
	lpPlayer->getLpPlayerModel()->Render(pd3d, *lpCam, *lpLight);
	lpPlayer->getLpPlayerModel()->setIsRenderShadowMap(false);


	//�߂�
	pd3d->SetRenderTarget(0, renderTargetBackup);
	pd3d->SetDepthStencilSurface(depthBufferBackup);
}