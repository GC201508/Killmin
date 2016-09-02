#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "Testiger.h"
#include "Ground.h"
#include "Player.h"
#include "Sonya.h"
//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------

//lib
Camera camera;//カメラ
Light light;//ライト

//model
Tiger tora;//とらちゃん
Ground ground;//地面
Sonya sonya;//ソーニャちゃん

//なんとかなんとか
Player player;//プレイヤー

//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	//ライトを初期化。
	light.Init();

	//もでうしょきか
	tora.Init(g_pd3dDevice);
	tora.SetPosition(D3DXVECTOR3(0.5f, 0.77f, 0.0f));
	ground.Init(g_pd3dDevice);
	ground.SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	sonya.Init(g_pd3dDevice);
	sonya.SetPosition(D3DXVECTOR3(-3.0f, 1.04f, 0.0f));
	
	//ぷれいや情報追加
	player.setModel(&tora);
	player.Init();

	//かえらしょきあ
	camera.Init();
}
//-----------------------------------------------------------------------------
// Name: 描画処理。
//-----------------------------------------------------------------------------
VOID Render()
{
	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();

	/*	-	-	-	-	-	-	-	-	*/
	tora.Render( g_pd3dDevice,camera,light); //とらちゃん
	ground.Render(g_pd3dDevice, camera, light);//地面
	sonya.Render(g_pd3dDevice, camera, light);//ソーニャちゃん
	/*	-	-	-	-	-	-	-	-	*/
	
	// シーンの描画終了。
	g_pd3dDevice->EndScene();
	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
/*!-----------------------------------------------------------------------------
*@brief	更新処理。
-----------------------------------------------------------------------------*/
void Update()
{
	light.Update(); //らいと
	camera.Update();//かめら
	tora.Update();//とらちゃん
	sonya.Update();//そにゃちゃん
	
	player.Update();//ぷれいや
}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
	tora.Release();
	ground.Release();
	sonya.Update();
}
