#include "Game.h"

//------------------------------------------------
// Nane: 初期化,	
//------------------------------------------------
void Game::GameInit()
{
	//ライトを初期化。
	light.Init();

	//もでうしょきか,
	tora     .Init(g_pd3dDevice);
	ground   .Init(g_pd3dDevice);
	sonya    .Init(g_pd3dDevice);
	testball .Init(g_pd3dDevice);



	//もでうぽじしょんしてい,
	tora     .SetPosition(Vec3(0.5f	, 0.77f	, 0.0f	)) ;
	ground   .SetPosition(Vec3(0.0f	, -3.5f	, 0.0f	)) ;
	sonya    .SetPosition(Vec3(-3.0f, 1.04f	, 0.0f	)) ;
	testball .SetPosition(Vec3(10.f	, 20.f	, 200.f	)) ;

	//しゃどうま
	ground.setShadowMap(&shadowMap);
	ground.setIsShadowReceiver(true);

	//オフスクリーン
	sprite.Init(g_pd3dDevice);
	renderTarget.Create(g_pd3dDevice,
		400                ,    //横幅,
		400                ,    //高さ,
		1                  ,	//レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_A8R8G8B8    ,	//カラーバッファのフォーマットはARGBの32bit
		D3DFMT_D16         ,	//学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0	                    //マルチサンプリングしないので０を指定。
		);
	renderTarget.InitGeometry(g_pd3dDevice);


	//ぷれいや情報追加,
	player.setInit(&tora, &camera, &shadowMap);

	//シャドマップ
	shadowMap.Init(g_pd3dDevice, &camera, &light, player.getLpPlayerModel());

	//かえらしょきあ,
	camera.Init();

	//モデルロード,
	modeldata.LoadModelData("Assets/model/Unity.X", &animation);
	skinmodel.Init(&modeldata);
	skinmodel.SetLight(&light);
	animation.PlayAnimation(0);

#if debuGmodE
	//フォント初期化
	Dfont->Init(g_pd3dDevice);
#endif //__debuGmodE
}

//--------------------------------------------------------------
// Name: 描画処理,
//--------------------------------------------------------------
void Game::GameRender()
{
#if debuGmodE
	//デバック用変数
	double time = sw->GetElapsed(); //FPS
	float leftStickX = XInput->getLeftStickX();
	float leftStickY = XInput->getLeftStickY();
#endif //_debuGmodE

	//シーンの描画開始。
	g_pd3dDevice->BeginScene();

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

#if debuGmodE
	//デバッグテキスト表示,
	char timetext [256];	//FPS
	char testText [256];	//テスト0
	char testText2[256];    //テスト1
	sprintf(timetext , "fps = %lf\n", 1.0f / time);
	sprintf(testText , "てすときるみー");
	sprintf(testText2, "LStick : X = %f Y = %f", leftStickX, leftStickY);
	if (XInput->IsPress(enButtonB)){ sprintf(testText, "わーい"); }
	Dfont->AddText(timetext);
	Dfont->AddText(testText);
	Dfont->AddText(testText2);
	Dfont->Render();
#endif //_debuGmodE

	/*	ShadowMap	*/
	shadowMap.Draw(g_pd3dDevice);

	/*	-	-	-	-	-	-	-	-	*/
	tora     .Render(g_pd3dDevice, camera, light); //とらちゃん,
	ground   .Render(g_pd3dDevice, camera, light); //地面,
	sonya    .Render(g_pd3dDevice, camera, light); //ソーニャちゃん,
	testball .Render(g_pd3dDevice, camera, light); //テストボール,

	skinmodel.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
	/*	-	-	-	-	-	-	-	-	*/



	/*	オフスクリーン,	*/

	renderTarget.Render(g_pd3dDevice, camera)               ;
	static int renderCount = 0                              ;
	renderCount++                                           ;
	D3DXMATRIXA16 matWorld                                  ;
	D3DXMatrixRotationY(&matWorld, renderCount / 500.0f)    ;
	sprite.Draw(matWorld, camera, renderTarget.GetTexture());

	// シーンの描画終了。
	g_pd3dDevice->EndScene();

	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

#if debuGmodE
	//FPSの計測,
	sw->Stop();
#endif //_debuGmodE

}

//---------------------------------------------------------------
// Name: 更新,
//---------------------------------------------------------------
void Game::GameUpdate()
{
#if debuGmodE
	//FPSの計測,
	sw->Start();
#endif //_debuGmodE
	animation.Update(1.0f / 60.0f);//anime

	light .Update();    //らいと
	camera.Update();   //かめら


	tora     .Update();         //とらちゃん,
	sonya    .Update();        //そにゃちゃん,
	ground   .Update();       //じめん,
	testball .Update();		//テストボアール,


	player .Update();   //ぷれいや
	pikumin.Update();  //ぴくみん

	if (XInput->IsPress(enButtonA))
	{
		animation.PlayAnimation(2);
	}

	skinmodel.UpdateWorldMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	shadowMap.Update();
}

//---------------------------------------------------------
//ゲームが終了するときに呼ばれる処理,
//---------------------------------------------------------
void Game::GameTerminate()
{
	tora     .Release();
	ground   .Release();
	sonya    .Release();
	testball .Release();
}