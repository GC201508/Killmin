#include "Model.h"
#include "Light.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "OBB.h"
Model::Model()
{
	mesh              = NULL;
	textures          = NULL;
	numMaterial       = 0;
	effect            = NULL;
	position.x        = 0.0f;
	position.y        = 0.0f;
	position.z        = 0.0f;
	angle             = D3DXToRadian(0.f);
	lpShadowMap       = nullptr;
	texToonMap		  = NULL;
	setIsRenderShadowMap(false);
	setIsReversalModel(false);
	setIsToonShader(false);
	reversalModelCnt = 0;
	D3DXMatrixIdentity(&mWorld);	//�P�ʍs����쐬
	D3DXMatrixIdentity(&mRotation); //�P�ʍs����쐬
}

Model::~Model()
{
	Release();
}

void Model::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}			

D3DXVECTOR3 Model::GetPosition()
{
	return position;
}

LPCTSTR Model::FileName()
{
	return "";
}

//������
void Model::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer; //X�t�@�C�������[�h

	//ttps://msdn.microsoft.com/ja-jp/library/cc372854.aspx
	D3DXLoadMeshFromX(FileName(), D3DXMESH_SYSTEMMEM, pd3dDevice,
		NULL, &pD3DXMtrlBuffer, NULL, &numMaterial, &mesh);

	//�}�e���A���o�b�t�@�擾
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	//�e�N�X�`�������[�h
	textures = new LPDIRECT3DTEXTURE9[numMaterial];
	for (DWORD i = 0; i < numMaterial; i++)
	{
		textures[i] = NULL;
		//�e�N�X�`�����쐬����B
		D3DXCreateTextureFromFileA(pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&textures[i]);
	}
	//�}�e���A���o�b�t�@������B
	pD3DXMtrlBuffer->Release();

	//�V�F�O���O���p�C��
	LPD3DXBUFFER compileErrorBuffer = NULL;
	//�V�F�[�_�[���p�C���o���J�[
	HRESULT hr = D3DXCreateEffectFromFile(
		pd3dDevice               ,
		"Assets/Shader/basic.fx" ,
		NULL                     ,
		NULL                     ,
		D3DXSHADER_SKIPVALIDATION,
		NULL                     ,
		&effect                  ,
		&compileErrorBuffer
		);
	if (hr != S_OK) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}

	//�J���[�����i�[����}���`�����_�[�^�[�Q�b�g�Ŏg�p����e�N�X�`���[���쐬����
	D3DXCreateTexture(pd3dDevice, WindowSizeX, WindowSizeY,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pColorTexture);
	//�J���[�����i�[����}���`�����_�[�^�[�Q�b�g�T�[�t�F�C�X���쐬����
	pColorTexture->GetSurfaceLevel(0, &pColorSurface);

	//Z�l�����i�[����}���`�����_�[�^�[�Q�b�g�Ŏg�p����e�N�X�`���[���쐬����
	D3DXCreateTexture(pd3dDevice, WindowSizeX, WindowSizeY,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &pZMapTexture);
	//Z�l�����i�[����}���`�����_�[�^�[�Q�b�g�T�[�t�F�C�X���쐬����
	pZMapTexture->GetSurfaceLevel(0, &pZMapSurface);

	CollisionOBB colOBB;
	colOBB.setModel(this);
	colOBB.CreateOBB(&modelOBB);
}

//�X�V
void Model::Update()
{
	//���[���h�s��̍X�V
	D3DXMatrixTranslation(&mTrans, position.x, position.y, position.z);

	//��]�s��̍X�V
	D3DXMatrixRotationY(&mRotation, angle);
	setWorld(mRotation * mTrans);
}

void Model::SetUpTechnique(LPDIRECT3DDEVICE9 pd3dDevice, Camera camera, Light light,int loopCnt)
{
	//�A�N�e�B�u�ȃe�N�j�b�N��ݒ肷��,
	if (loopCnt == 0)
	{

		if (!isRenderShadowMap){
			
			if (!isToonShader){
				//�ʏ�`��
				effect->SetTechnique("SkinModel");
			}
			else
			{
				//�g�D�[���V�F�[�_
				effect->SetTechnique("ToonShader");
				if (texToonMap == NULL)
				{
					D3DXCreateTextureFromFileA(pd3dDevice,
						"Assets/model/sn/ToonShadow.png",         //�g�D�[���}�b�v�e�N�X�`���[�t�@�C����
						&texToonMap);
				}
			}
		}
		else{
			//�V���h�E�}�b�v�ւ̏������݃e�L�j�b�N,
			effect->SetTechnique("ShadowMap");
		}
	}

	//���ڂ̎�,
	if (isReversalModel && loopCnt == 1)
	{
		//�֊s(���]�������f��),
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		effect->SetTechnique("ReversalModel");
	//	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

}

//�`��
void Model::Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera camera,Light light)
{
	int loopCnt = 0;
	isReversalModel && !isRenderShadowMap ? loopCnt = 2 : loopCnt = 1;
	for (short i = 0; i < loopCnt; i++)
	{
		SetUpTechnique(pd3dDevice, camera, light,i);


		effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE); //�e�N�j�b�N�̓K�p���J�n����B
		effect->BeginPass(0); //�A�N�e�B�u�ȃe�N�j�b�N���ŁA�p�X���J�n���܂��B


		//�萔���W�X�^�ɐݒ肷��F�Ȃ�ł���
		D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);

		//->SetMatrix  ��]�u�s���ݒ肷��݂����B�@��]�u�s����ĂȂ񂾂您��
		effect->SetMatrix("g_worldMatrix", &mWorld);	//���[���h�s��̓]��
		if (isRenderShadowMap){
			effect->SetMatrix("g_viewMatrix", lpShadowMap->lpLightViewMatrix);		//�r���[�s��̓]��
			effect->SetMatrix("g_projectionMatrix", lpShadowMap->lpLightProjMatrix);	//�v���W�F�N�V�����s��̓]��
		}
		else{
			effect->SetMatrix("g_viewMatrix", &camera.GetViewMatrix());				//�r���[�s��̓]��
			effect->SetMatrix("g_projectionMatrix", &camera.GetProjectionMatrix());	//�v���W�F�N�V�����s��̓]��
		}
		effect->SetMatrix("g_rotationMatrix", &mRotation);	//��]�s���]��

		//->SetVectorArray�@�x�N�g���̔z���ݒ肷��,
		effect->SetVectorArray("g_diffuseLightDirection", light.getDiffuseLDir(), light.getLNum()); //���C�g�̕�����]��
		effect->SetVectorArray("g_diffuseLightColor", light.getDiffuseLColor(), light.getLNum()); //���C�g�̃J���[��]��

		//->SetVector�@�x�N�g����ݒ肷��,
		effect->SetVector("g_ambientLight", &light.getAmbientL()); //������ݒ�
		effect->SetVector("vEyePt", &Vec4(camera.GetEyePt())); //camera.Eyept

		/*ShadowReceiver*/
		if (isShadowReceiver)
		{
			effect->SetBool("g_isShadowReceiver", isShadowReceiver);
			if (lpShadowMap->lpLightViewMatrix != nullptr and lpShadowMap->lpLightProjMatrix != nullptr)
			{
				effect->SetMatrix("g_lightViewMatrix", lpShadowMap->lpLightViewMatrix);
				effect->SetMatrix("g_lightProjectionMatrix", lpShadowMap->lpLightProjMatrix);
				effect->SetTexture("g_shadowMapTexture", lpShadowMap->getShadowMapRenderTex());
			}
		}

		/*ToonSheder*/
		if (isToonShader)
		{
			effect->SetTexture("tex0", textures[0]);  //�I�u�W�F�N�g�̃e�N�X�`�����X�e�[�W0�ɃZ�b�g����
			effect->SetTexture("tex1", texToonMap);	//�g�D�[���}�b�v�e�N�X�`�����X�e�[�W�P�ɃZ�b�g����
		}

		for (DWORD i = 0; i < numMaterial; i++)
		{
			//->SetTexture�@�e�N�X�`����ݒ肷��,
			effect->SetTexture("g_diffuseTexture", textures[i]);

			//->CommitChanges�@�A�N�e�B�u�ȃp�X���Ő�����X�e�[�g�ω��������_�����O�̑O�Ƀf�o�C�X�ɓ`���邻���ł��B�@�E�E�E�ǂ�Ӗ��H
			effect->CommitChanges(); //�f�[�^�̓]�����m�肷��B �`����s���O�Ɉ�x�����Ăяo��,

			//Draw the mesh subset(�����}�}),
			//->DrawSubset�@���b�V���̃T�u�Z�b�g��`�悷��B�@�T�u�Z�b�g���ĉ��患,
			mesh->DrawSubset(i);
		}

		effect->EndPass();// ->EndPass() �A�N�e�B�u�ȃp�X���I�����܂��B
		effect->End();//->End() �A�N�e�B�u�ȃe�N�j�b�N���I�����܂��B

		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}
//�J���B
void Model::Release()
{
	//���b�V�����J���B
	if (mesh != NULL){
		mesh->Release();
		mesh = NULL;
	}

	//�e�N�X�`�����J���B
	if (textures != NULL){
		for (unsigned int i = 0; i < numMaterial; i++){
			if (textures[i] != NULL){
				textures[i]->Release();
				textures[i] = NULL;
			}
		}
		delete[] textures;
		textures = NULL;
	}
	//�G�t�F�N�g���J���B
	if (effect != NULL){
		effect->Release();
		effect = NULL;
	}
}