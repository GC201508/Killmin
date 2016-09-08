#include "Pikumin.h"
#include "Model.h"
#include "Player.h"

Pikumin::Pikumin()
{//初期化を行う
	lpModel = nullptr;
	lpPlayerPos = nullptr;
	addPos = Vec3(0.f, 0.f, 0.f);
}

Pikumin::~Pikumin()
{

}

void Pikumin::Init()
{//初期化行う
	if (lpModel == nullptr)
	{
		printf("もでるないぞ");
	}
	else
	{
		addPos = lpModel->GetPosition();
	}
}

void Pikumin::Update()
{//更新行う
	StaringPlayer();
}

void Pikumin::StaringPlayer()
{//プレイヤーを見つめる

}