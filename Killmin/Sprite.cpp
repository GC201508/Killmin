#include "Sprite.h"
#include "Camera.h"

Sprite::Sprite()
{
	pVB = NULL;
}

Sprite::~Sprite()
{
	Release();
}

void Sprite::Release()
{
	if (pVB){
		pVB->Release();
		pVB = NULL;
	}
}

HRESULT Sprite::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	Release();

	//頂点バッファの作成,
	SVertex vertices[] =
	{
		{ -1.5f, -1.5f, 0.0f, 1.0f, 0xFF888888, 0.0f, 1.0f },
		{ -1.5f, 1.5f, 0.0f, 1.0f, 0xFF888888, 0.0f, 0.0f },
		{ 1.5f, -1.5f, 0.0f, 1.0f, 0xFF888888, 1.0f, 1.0f },
		{ 1.5f, 1.5f, 0.0f, 1.0f, 0xFF888888, 1.0f, 0.0f },
	};

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(pd3dDevice->CreateVertexBuffer(4 * sizeof(SVertex),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &pVB, NULL)))
	{
		return E_FAIL;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	pVB->Unlock();

	//スプライト描画用のシェーダーをコンパイルバンカー！！。
	LPD3DXBUFFER compileErrorBuffer = NULL;
	//シェーダーをコンパイル。
	HRESULT hr = D3DXCreateEffectFromFile(
		pd3dDevice,
		"Assets/Shader/sprite.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&effect,
		&compileErrorBuffer
		);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}

void Sprite::Draw(
	Matrix worldMatrix,
	Camera cam,
	LPDIRECT3DTEXTURE9 texture)
{
	//シェーダー適用開始。
	effect->SetTechnique("Sprite");
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);
	effect->SetTexture("g_texture", texture);
	//ワールド行列の転送。
	effect->SetMatrix("g_worldMatrix", &worldMatrix);
	//ビュー行列の転送。
	effect->SetMatrix("g_viewMatrix", &cam.GetViewMatrix());
	//プロジェクション行列の転送。
	effect->SetMatrix("g_projectionMatrix", &cam.GetProjectionMatrix());

	effect->CommitChanges();

	g_pd3dDevice->SetStreamSource(0, pVB, 0, sizeof(SVertex));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	effect->EndPass();
	effect->End();
}