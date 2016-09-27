#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "InputKey.h"
//使用モデル
#include "Testiger.h"
#include "Ground.h"
#include "Sonya.h"
//役割
#include "Player.h"
#include "Pikumin.h"
//スキンモデル
#include "SkinModel.h"
#include "ModelData.h"
#include "Animation.h"

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
Pikumin pikumin;

//スキンモデル
SkinModel skinmodel;
ModelData modeldata;
Animation animation;

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
	ground.SetPosition(D3DXVECTOR3(0.0f, -3.5f, 0.0f));
	sonya.Init(g_pd3dDevice);
	sonya.SetPosition(D3DXVECTOR3(-3.0f, 1.04f, 0.0f));

	//ぷれいや情報追加
	player.setInit(&tora, &camera);

	//ぴくみん情報追加
	//pikumin.setModel(&tora);

	//かえらしょきあ
	camera.Init();

	//モデルロード
	modeldata.LoadModelData("Assets/model/Unity.X", &animation);
	skinmodel.Init(&modeldata);
	skinmodel.SetLight(&light);
	animation.PlayAnimation(0);

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


	skinmodel.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
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
	animation.Update(1.0f / 60.0f);//anime

	light.Update(); //らいと
	camera.Update();//かめら
	tora.Update();//とらちゃん
	sonya.Update();//そにゃちゃん
	ground.Update();//じめん
	
	player.Update();//ぷれいや
	pikumin.Update();//ぴくみん

	if (InputKey::InputKeyCode(VK_SPACE))
	{
		animation.PlayAnimation(3);
	}

	skinmodel.UpdateWorldMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
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
