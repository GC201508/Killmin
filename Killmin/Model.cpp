#include "Model.h"

Model::Model()
{
	mesh = NULL;
	textures = NULL;
	numMaterial = 0;
	effect = NULL;
	D3DXMatrixIdentity(&mWorld);	//単位行列を作成
	D3DXMatrixIdentity(&mRotation); //単位行列を作成
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

//初期化
void Model::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer; //Xファイルをロード

	//ttps://msdn.microsoft.com/ja-jp/library/cc372854.aspx
	D3DXLoadMeshFromX(FileName(), D3DXMESH_SYSTEMMEM, pd3dDevice,
		NULL, &pD3DXMtrlBuffer, NULL, &numMaterial, &mesh);

	//マテリアルバッファ取得
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	//テクスチャをロード
	textures = new LPDIRECT3DTEXTURE9[numMaterial];
	for (DWORD i = 0; i < numMaterial; i++)
	{
		textures[i] = NULL;
		//テクスチャを作成する。
		D3DXCreateTextureFromFileA(pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&textures[i]);
	}
	//マテリアルバッファを解放。
	pD3DXMtrlBuffer->Release();

	//シェ＾だ＾をパイル
	LPD3DXBUFFER compileErrorBuffer = NULL;
	//シェーダーをパイルバンカー
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

//更新
void Model::Update()
{
	//ワールド行列の更新
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);
}

//描画
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
	effect->SetTechnique("さといも"); //アクティブなテクニックを設定する。
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE); //テクニックの適用を開始する。
	effect->BeginPass(0); //アクティブなテクニック内で、パスを開始します。


	//定数レジスタに設定する色なんですよ
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);


	//->SetMatrix  非転置行列を設定するみたい。　非転置行列ってなんだよおい
	effect->SetMatrix("g_worldMatrix", &mWorld);//ワールド行列の転送
	effect->SetMatrix("g_viewMatrix", &viewMatrix);//ビュー行列の転送
	effect->SetMatrix("g_projectionMatrix", &projMatrix);//プロジェクション行列の転送
	effect->SetMatrix("g_rotationMatrix", &mRotation);//回転行列を転送

	//->SetVectorArray　ベクトルの配列を設定する
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight); //ライトのカラーを転送

	//->SetVector　ベクトルを設定する
	effect->SetVector("g_ambientLight", &ambientLight); //環境光を設定

	//->CommitChanges　アクティブなパス内で生じるステート変化をレンダリングの前にデバイスに伝えるそうです。　・・・どゆ意味？
	effect->CommitChanges(); //データの転送が確定する。 描画を行う前に一度だけ呼び出す


	for (DWORD i = 0; i < numMaterial; i++)
	{
		//->SetTexture　テクスチャを設定する
		effect->SetTexture("g_diffuseTexture", textures[i]);

		//Draw the mesh subset(原文ママ)
		//->DrawSubset　メッシュのサブセットを描画する。　サブセットって何よぅ
		mesh->DrawSubset(i);
	}

	effect->EndPass();// ->EndPass() アクティブなパスを終了します。
	effect->End();//->End() アクティブなテクニックを終了します。
}
//開放。
void Model::Release()
{
	//メッシュを開放。
	if (mesh != NULL){
		mesh->Release();
		mesh = NULL;
	}

	//テクスチャを開放。
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
	//エフェクトを開放。
	if (effect != NULL){
		effect->Release();
		effect = NULL;
	}
}