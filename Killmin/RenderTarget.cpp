#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
	depthSurface   = NULL;
	texture        = NULL;
	surface        = NULL;
	texture        = NULL;
	dwNumMaterials = 0L;
	pMeshTextures  = NULL;
	pMesh          = NULL;
	pTigerEffect   = NULL;
}

RenderTarget::~RenderTarget()
{
	Release();
}

void RenderTarget::Release()
{
	if (texture != nullptr) {
		texture->Release();
		texture = NULL;
	}
	if (depthSurface != nullptr) {
		depthSurface->Release();
		depthSurface = NULL;
	}
	if (surface != NULL) {
		surface->Release();
		surface = NULL;
	}
}

void RenderTarget::Create( //�����_�����O�^�[�Q�g�����
	LPDIRECT3DDEVICE9 pd3dDevice,		// [in] pd3dDevice �f���@�C�X
	int w,								// [in] w �����_�����O�^�[�Q�[�g��,
	int h,								// [in] h �����_�����[�^�O�Q�[�g��,
	int mipLevel,						// [in] mipLevel		�~�b�v�}�b�v���x��,
	D3DFORMAT colorFormat,				// [in]	colorFormat	�J���[�o�b�t�@�̃t�H�[�}�b�g,
	D3DFORMAT depthStencilFormat,		// [in] depthStencilFormat	�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g,
	D3DMULTISAMPLE_TYPE multiSampleType,// [in]	multiSampleType	�}���`�T���v�����O�̎��,
	int multiSampleQuality)				// [in]	multiSampleQuality	�}���`�T���v�����O�̕i���B
{
	//�f�v�X�X�e���V���o�b�t�@�̍쐬�B
	HRESULT hr = pd3dDevice->CreateDepthStencilSurface(
		w,
		h,
		static_cast<D3DFORMAT>(depthStencilFormat),
		static_cast<D3DMULTISAMPLE_TYPE>(multiSampleType),
		multiSampleQuality,
		TRUE,
		&depthSurface,
		NULL
		);
	//�J���[�o�b�t�@���쐬�B
	hr = pd3dDevice->CreateTexture(
		w,
		h,
		mipLevel,
		D3DUSAGE_RENDERTARGET,
		static_cast<D3DFORMAT>(colorFormat),
		D3DPOOL_DEFAULT,
		&texture,
		NULL
		);
	texture->GetSurfaceLevel(0, &surface);
}

HRESULT RenderTarget::InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX("Assets/Model/Tiger.x", D3DXMESH_SYSTEMMEM,
		pd3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX("..\\Tiger.x", D3DXMESH_SYSTEMMEM,
			pd3dDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
			&pMesh)))
		{
			MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];
	if (pMeshTextures == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFileA(pd3dDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				// If texture is not in current folder, try parent folder
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, MAX_PATH, strPrefix);
				strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFileA(pd3dDevice,
					strTexture,
					&pMeshTextures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();

	//�g���p�̃V�F�[�_�[���R���p�C���B

	LPD3DXBUFFER  compileErrorBuffer = NULL;

	HRESULT hr = D3DXCreateEffectFromFile(
		pd3dDevice,
		"Assets/Shader/basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&pTigerEffect,
		&compileErrorBuffer
		);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
	return S_OK;
}
#include "Camera.h"
void RenderTarget::Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera cam)
{
	static int renderCount = 0;
	renderCount++;
	
	LPDIRECT3DSURFACE9 renderTargetBackup;
	LPDIRECT3DSURFACE9 depthBufferBackup;
	pd3dDevice->GetRenderTarget(0, &renderTargetBackup);	//���X�̃����_�����O�^�[�Q�b�g��ۑ��B��Ŗ߂��K�v������̂ŁB
	pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//���X�̃f�v�X�X�e���V���o�b�t�@��ۑ��B��Ŗ߂��K�v������̂ŁB

	//�����_�����O�^�[�Q�b�g��ύX����B		�V�F�[�_�K�p�O�ɂ�鏈���Ȃ�?,
	pd3dDevice->SetRenderTarget(0, GetRenderTarget());
	pd3dDevice->SetDepthStencilSurface(GetDepthStencilBuffer());

	//�������ݐ��ύX�����̂ŃN���A�B
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//�V�F�[�_�[�K�p�J�n�B
	pTigerEffect->SetTechnique("SkinModel");
	pTigerEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	pTigerEffect->BeginPass(0);

	//�萔���W�X�^�ɐݒ肷��J���[�B
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mTigerMatrix;
	D3DXMatrixRotationY(&mTigerMatrix, renderCount / -50.0f);

	//���[���h�s��̓]���B
	pTigerEffect->SetMatrix("g_worldMatrix", &mTigerMatrix);
	//�r���[�s��̓]���B
	pTigerEffect->SetMatrix("g_viewMatrix", &cam.GetViewMatrix());
	//�v���W�F�N�V�����s��̓]���B
	pTigerEffect->SetMatrix("g_projectionMatrix", &cam.GetProjectionMatrix());
	pTigerEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B

	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pTigerEffect->SetTexture("g_diffuseTexture", pMeshTextures[i]);
		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}

	pTigerEffect->EndPass();
	pTigerEffect->End();



	//�F�X�߂�,	Effect��End���Ă�����̂���?,
	pd3dDevice->SetRenderTarget(0, renderTargetBackup);	//�߂��B
	pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//�߂��B

}