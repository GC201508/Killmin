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

void RenderTarget::Create( //レンダリングターゲトを作る
	LPDIRECT3DDEVICE9 pd3dDevice,		// [in] pd3dDevice デヴァイス
	int w,								// [in] w レンダリングターゲート幅,
	int h,								// [in] h レンダリンータグゲート高,
	int mipLevel,						// [in] mipLevel		ミップマップレベル,
	D3DFORMAT colorFormat,				// [in]	colorFormat	カラーバッファのフォーマット,
	D3DFORMAT depthStencilFormat,		// [in] depthStencilFormat	デプスステンシルバッファのフォーマット,
	D3DMULTISAMPLE_TYPE multiSampleType,// [in]	multiSampleType	マルチサンプリングの種類,
	int multiSampleQuality)				// [in]	multiSampleQuality	マルチサンプリングの品質。
{
	//デプスステンシルバッファの作成。
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
	//カラーバッファを作成。
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

	//トラ用のシェーダーをコンパイル。

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
	pd3dDevice->GetRenderTarget(0, &renderTargetBackup);	//元々のレンダリングターゲットを保存。後で戻す必要があるので。
	pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//元々のデプスステンシルバッファを保存。後で戻す必要があるので。

	//レンダリングターゲットを変更する。		シェーダ適用前にやる処理なの?,
	pd3dDevice->SetRenderTarget(0, GetRenderTarget());
	pd3dDevice->SetDepthStencilSurface(GetDepthStencilBuffer());

	//書き込み先を変更したのでクリア。
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	//シェーダー適用開始。
	pTigerEffect->SetTechnique("SkinModel");
	pTigerEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	pTigerEffect->BeginPass(0);

	//定数レジスタに設定するカラー。
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	D3DXMATRIX mTigerMatrix;
	D3DXMatrixRotationY(&mTigerMatrix, renderCount / -50.0f);

	//ワールド行列の転送。
	pTigerEffect->SetMatrix("g_worldMatrix", &mTigerMatrix);
	//ビュー行列の転送。
	pTigerEffect->SetMatrix("g_viewMatrix", &cam.GetViewMatrix());
	//プロジェクション行列の転送。
	pTigerEffect->SetMatrix("g_projectionMatrix", &cam.GetProjectionMatrix());
	pTigerEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

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



	//色々戻す,	EffectがEndしてからやるのかね?,
	pd3dDevice->SetRenderTarget(0, renderTargetBackup);	//戻す。
	pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//戻す。

}