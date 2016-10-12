#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "InputKey.h"
//model
#include "Model.h"

//オフスクリーン
#include "Sprite.h"
#include "RenderTarget.h"
//役割
#include "Player.h"
#include "Pikumin.h"
//スキンモデル
#include "SkinModel.h"
#include "ModelData.h"
#include "Animation.h"

#if debuGmodE
//デバッグ
#include "DebugFont.h"
DebugFont Dfont;
#endif //_debuGmodE

//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------

//lib
Camera camera;//カメラ
Light light;//ライト

//model
class Tiger : public Model{ LPCTSTR FileName()override { return "Assets/model/tiger.X"; } }tora;//とらちゃん
class Ground : public Model{ LPCTSTR FileName()override { return "Assets/model/ground.X"; } }ground;//地面
class Sonya : public Model{ LPCTSTR FileName()override { return "Assets/model/sn/ソーニャ.X"; } }sonya;//ソーニャちゃん
class TestPikumin : public Model{ LPCTSTR FileName()override { return "Assets/model/TyokurituTiger.X"; } }pikuTiger;//ピクミンタイガーちゃん

//TODO:(たけまさ,)素敵なライト確認用,
class ball : public Model{ LPCTSTR FileName()override { return "Assets/model/sphere.X"; } }testball;//テストボール


//オフスクリーン
Sprite sprite;
RenderTarget renderTarget;

//なんとかなんとか
Player player;//プレイヤー
Pikumin pikumin;//ぴくみん

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

	//もでうしょきか,
	tora.Init(g_pd3dDevice);
	tora.SetPosition(D3DXVECTOR3(0.5f, 0.77f, 0.0f));
	ground.Init(g_pd3dDevice);
	ground.SetPosition(D3DXVECTOR3(0.0f, -3.5f, 0.0f));
	sonya.Init(g_pd3dDevice);
	sonya.SetPosition(D3DXVECTOR3(-3.0f, 1.04f, 0.0f));
	pikuTiger.Init(g_pd3dDevice);
	pikuTiger.SetPosition(Vec3(0.f, 0.f, 10.f));
	
	testball.Init(g_pd3dDevice);
	testball.SetPosition(Vec3(10.f, 20.f, 200.f));

	//オフスクリーン
	sprite.Init(g_pd3dDevice);
	renderTarget.Create(g_pd3dDevice,
		400,//横幅,
		400,//高さ,
		1,	//レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_A8R8G8B8,	//カラーバッファのフォーマットはARGBの32bit
		D3DFMT_D16,	//学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0	//マルチサンプリングしないので０を指定。
		);
	renderTarget.InitGeometry(g_pd3dDevice);

	//ぷれいや情報追加,
	player.setInit(&tora, &camera);

	//ぴくみん情報追加,
	pikumin.setModel(&pikuTiger);
	pikumin.setLPPlayerPos(player.getLpPlayerModel()->lpPos);
	pikumin.Init();

	//かえらしょきあ,
	camera.Init();

	//モデルロード,
	modeldata.LoadModelData("Assets/model/Unity.X", &animation);
	skinmodel.Init(&modeldata);
	skinmodel.SetLight(&light);
	animation.PlayAnimation(0);

#if debuGmodE
	//フォント初期化
	Dfont.Init(g_pd3dDevice);
#endif //__debuGmodE

}
//TODO:時間
CStopwatch sw;
//-----------------------------------------------------------------------------
// Name: 描画処理。
//-----------------------------------------------------------------------------
VOID Render()
{
	double time = sw.GetElapsed();


	//シーンの描画開始。
	g_pd3dDevice->BeginScene();	

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
#if debuGmodE

	Dfont.Render(time);
#endif

	/*	-	-	-	-	-	-	-	-	*/
	tora.Render( g_pd3dDevice,camera,light); //とらちゃん,
	ground.Render(g_pd3dDevice, camera, light);//地面,
	sonya.Render(g_pd3dDevice, camera, light);//ソーニャちゃん,
	pikuTiger.Render(g_pd3dDevice, camera, light);//ピクミン虎ちゃん,
	testball.Render(g_pd3dDevice, camera, light);//テストボール,
	skinmodel.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
	/*	-	-	-	-	-	-	-	-	*/
	
	/*オフスクリーン*/

	renderTarget.Render(g_pd3dDevice,camera);

	static int renderCount = 0;
	renderCount++;
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, renderCount / 500.0f);
	sprite.Draw(matWorld,camera,renderTarget.GetTexture());

	// シーンの描画終了。
	g_pd3dDevice->EndScene();


	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
#if debuGmodE
	//デバッグテキスト表示
	sw.Stop();
#endif //_debuGmodE

}
/*!-----------------------------------------------------------------------------
*@brief	更新処理。
-----------------------------------------------------------------------------*/
void Update()
{
	sw.Start();
	animation.Update(1.0f / 60.0f);//anime

	light.Update(); //らいと
	camera.Update();//かめら


	tora.Update();//とらちゃん
	sonya.Update();//そにゃちゃん
	ground.Update();//じめん
	pikuTiger.Update();//ぴくぴくとらちゃん


	testball.Update();//テストボアール

	
	player.Update();//ぷれいや
	pikumin.Update();//ぴくみん

	if (InputKey::InputHoldCode(VK_SPACE))
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
	sonya.Release();
	pikuTiger.Release();

	testball.Release();
}
