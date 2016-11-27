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
	D3DXMatrixIdentity(&mWorld);	//単位行列を作成
	D3DXMatrixIdentity(&mRotation); //単位行列を作成
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

	//カラー情報を格納するマルチレンダーターゲットで使用するテクスチャーを作成する
	D3DXCreateTexture(pd3dDevice, WindowSizeX, WindowSizeY,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pColorTexture);
	//カラー情報を格納するマルチレンダーターゲットサーフェイスを作成する
	pColorTexture->GetSurfaceLevel(0, &pColorSurface);

	//Z値情報を格納するマルチレンダーターゲットで使用するテクスチャーを作成する
	D3DXCreateTexture(pd3dDevice, WindowSizeX, WindowSizeY,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &pZMapTexture);
	//Z値情報を格納するマルチレンダーターゲットサーフェイスを作成する
	pZMapTexture->GetSurfaceLevel(0, &pZMapSurface);

	CollisionOBB colOBB;
	colOBB.setModel(this);
	colOBB.CreateOBB(&modelOBB);
}

//更新
void Model::Update()
{
	//ワールド行列の更新
	D3DXMatrixTranslation(&mTrans, position.x, position.y, position.z);

	//回転行列の更新
	D3DXMatrixRotationY(&mRotation, angle);
	setWorld(mRotation * mTrans);
}

void Model::SetUpTechnique(LPDIRECT3DDEVICE9 pd3dDevice, Camera camera, Light light,int loopCnt)
{
	//アクティブなテクニックを設定する,
	if (loopCnt == 0)
	{

		if (!isRenderShadowMap){
			
			if (!isToonShader){
				//通常描画
				effect->SetTechnique("SkinModel");
			}
			else
			{
				//トゥーンシェーダ
				effect->SetTechnique("ToonShader");
				if (texToonMap == NULL)
				{
					D3DXCreateTextureFromFileA(pd3dDevice,
						"Assets/model/sn/ToonShadow.png",         //トゥーンマップテクスチャーファイル名
						&texToonMap);
				}
			}
		}
		else{
			//シャドウマップへの書き込みテキニック,
			effect->SetTechnique("ShadowMap");
		}
	}

	//二回目の時,
	if (isReversalModel && loopCnt == 1)
	{
		//輪郭(反転したモデル),
		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		effect->SetTechnique("ReversalModel");
	//	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

}

//描画
void Model::Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera camera,Light light)
{
	int loopCnt = 0;
	isReversalModel && !isRenderShadowMap ? loopCnt = 2 : loopCnt = 1;
	for (short i = 0; i < loopCnt; i++)
	{
		SetUpTechnique(pd3dDevice, camera, light,i);


		effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE); //テクニックの適用を開始する。
		effect->BeginPass(0); //アクティブなテクニック内で、パスを開始します。


		//定数レジスタに設定する色なんですよ
		D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);

		//->SetMatrix  非転置行列を設定するみたい。　非転置行列ってなんだよおい
		effect->SetMatrix("g_worldMatrix", &mWorld);	//ワールド行列の転送
		if (isRenderShadowMap){
			effect->SetMatrix("g_viewMatrix", lpShadowMap->lpLightViewMatrix);		//ビュー行列の転送
			effect->SetMatrix("g_projectionMatrix", lpShadowMap->lpLightProjMatrix);	//プロジェクション行列の転送
		}
		else{
			effect->SetMatrix("g_viewMatrix", &camera.GetViewMatrix());				//ビュー行列の転送
			effect->SetMatrix("g_projectionMatrix", &camera.GetProjectionMatrix());	//プロジェクション行列の転送
		}
		effect->SetMatrix("g_rotationMatrix", &mRotation);	//回転行列を転送

		//->SetVectorArray　ベクトルの配列を設定する,
		effect->SetVectorArray("g_diffuseLightDirection", light.getDiffuseLDir(), light.getLNum()); //ライトの方向を転送
		effect->SetVectorArray("g_diffuseLightColor", light.getDiffuseLColor(), light.getLNum()); //ライトのカラーを転送

		//->SetVector　ベクトルを設定する,
		effect->SetVector("g_ambientLight", &light.getAmbientL()); //環境光を設定
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
			effect->SetTexture("tex0", textures[0]);  //オブジェクトのテクスチャをステージ0にセットする
			effect->SetTexture("tex1", texToonMap);	//トゥーンマップテクスチャをステージ１にセットする
		}

		for (DWORD i = 0; i < numMaterial; i++)
		{
			//->SetTexture　テクスチャを設定する,
			effect->SetTexture("g_diffuseTexture", textures[i]);

			//->CommitChanges　アクティブなパス内で生じるステート変化をレンダリングの前にデバイスに伝えるそうです。　・・・どゆ意味？
			effect->CommitChanges(); //データの転送が確定する。 描画を行う前に一度だけ呼び出す,

			//Draw the mesh subset(原文ママ),
			//->DrawSubset　メッシュのサブセットを描画する。　サブセットって何よぅ,
			mesh->DrawSubset(i);
		}

		effect->EndPass();// ->EndPass() アクティブなパスを終了します。
		effect->End();//->End() アクティブなテクニックを終了します。

		pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
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