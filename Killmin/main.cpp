#include "System.h"
#include "Game.h"
#include "TestGame.h"
//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

Game* game;
TestGame* testgame;
enum e{eGame = 0,eTest};
int e_hoge = 1;

extern void Terminate();

//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
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
		printf("�ˁ[��");
		break;
	}
}
//-----------------------------------------------------------------------------
// Name: �`�揈���B
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
		printf("�ˁ[��");
		break;
	}
	
}
/*!-----------------------------------------------------------------------------
*@brief	�X�V�����B
-----------------------------------------------------------------------------*/
void Update()
{
	if (onKey('P'))	//TODO:Scene�؂�ւ��X�C�b�`
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
		printf("�ˁ[��");
		break;
	}
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
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
		printf("�ˁ[��");
		break;
	}
}
