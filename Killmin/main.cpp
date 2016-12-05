#include "System.h"
#include "Game.h"
#include "TestGame.h"
//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------

Game* game;
TestGame* testgame;

//TODO:シーン切り替えの確認用
enum e{eGame = 0,eTest};
int e_hoge = 1;

//タイトル画面とか選択画面とかゲーム画面とかの切り替え用
enum eSceneChange{
	eS_Title = 0      ,	// タイトル画面
	eS_DetaSelect     , // セーブデータ選択画面
	eS_Stage          , // ステージ(マリオ６４でいうピーチ城みたいなとこ)
	eS_StarHint		  , // スターのヒント 
	eS_GameStage		// ゲームのステージ
};

extern void Terminate();

//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	switch (e_hoge)
	{
	case eGame:
		game = new Game;
		game->GameInit();
		break;

	case eTest:
		testgame = new TestGame;
		testgame->Init();
		break;

	default:
		printf("ねーよ");
		break;
	}
}
//-----------------------------------------------------------------------------
// Name: 描画処理。
//-----------------------------------------------------------------------------
VOID Render()
{
	switch (e_hoge)
	{
	case eGame:
		game->GameRender();
		break;

	case eTest:
		testgame->Render();
		break;

	default:
		printf("ねーよ");
		break;
	}
	
}
/*!-----------------------------------------------------------------------------
*@brief	更新処理。
-----------------------------------------------------------------------------*/
void Update()
{
	if (onKey('P'))	//TODO:Scene切り替えスイッチ
	{
		e_hoge++;
		e_hoge %= 2;

		delete Dfont;
		Dfont = new DebugFont;
		
		Init();
	}


	switch (e_hoge)
	{
	case eGame:
		game->GameUpdate();
		break;

	case eTest:
		testgame->Update();
		break;

	default:
		printf("ねーよ");
		break;
	}
}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
	switch (e_hoge)
	{
	case eGame:
		game->GameTerminate();
		break;

	case eTest:
		testgame->Terminate();
		break;

	default:
		printf("ねーよ");
		break;
	}
}
