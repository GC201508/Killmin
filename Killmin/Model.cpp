#include "Model.h"

Model::Model()
{
	mesh = NULL;
	textures = NULL;
	numMaterial = 0;
	effect = NULL;
	D3DXMatrixIdentity(&mWorld);	//�P�ʍs����쐬
	D3DXMatrixIdentity(&mRotation); //�P�ʍs����쐬
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}

Model::~Model()
{
	Release();
}

void Model::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
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
		pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&effect,
		&compileErrorBuffer
		);
	if (hr != S_OK) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}

//�X�V
void Model::Update()
{
	//���[���h�s��̍X�V
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);
}

//�`��
void Model::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4 ambientLight,
	int numDiffuseLight
	)
{
	effect->SetTechnique("���Ƃ���"); //�A�N�e�B�u�ȃe�N�j�b�N��ݒ肷��B
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE); //�e�N�j�b�N�̓K�p���J�n����B
	effect->BeginPass(0); //�A�N�e�B�u�ȃe�N�j�b�N���ŁA�p�X���J�n���܂��B


	//�萔���W�X�^�ɐݒ肷��F�Ȃ�ł���
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);


	//->SetMatrix  ��]�u�s���ݒ肷��݂����B�@��]�u�s����ĂȂ񂾂您��
	effect->SetMatrix("g_worldMatrix", &mWorld);//���[���h�s��̓]��
	effect->SetMatrix("g_viewMatrix", &viewMatrix);//�r���[�s��̓]��
	effect->SetMatrix("g_projectionMatrix", &projMatrix);//�v���W�F�N�V�����s��̓]��
	effect->SetMatrix("g_rotationMatrix", &mRotation);//��]�s���]��

	//->SetVectorArray�@�x�N�g���̔z���ݒ肷��
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight); //���C�g�̃J���[��]��

	//->SetVector�@�x�N�g����ݒ肷��
	effect->SetVector("g_ambientLight", &ambientLight); //������ݒ�

	//->CommitChanges�@�A�N�e�B�u�ȃp�X���Ő�����X�e�[�g�ω��������_�����O�̑O�Ƀf�o�C�X�ɓ`���邻���ł��B�@�E�E�E�ǂ�Ӗ��H
	effect->CommitChanges(); //�f�[�^�̓]�����m�肷��B �`����s���O�Ɉ�x�����Ăяo��


	for (DWORD i = 0; i < numMaterial; i++)
	{
		//->SetTexture�@�e�N�X�`����ݒ肷��
		effect->SetTexture("g_diffuseTexture", textures[i]);

		//Draw the mesh subset(�����}�})
		//->DrawSubset�@���b�V���̃T�u�Z�b�g��`�悷��B�@�T�u�Z�b�g���ĉ��患
		mesh->DrawSubset(i);
	}

	effect->EndPass();// ->EndPass() �A�N�e�B�u�ȃp�X���I�����܂��B
	effect->End();//->End() �A�N�e�B�u�ȃe�N�j�b�N���I�����܂��B
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