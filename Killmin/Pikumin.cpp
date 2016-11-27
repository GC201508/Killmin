#include "Pikumin.h"
#include "Model.h"
#include "Player.h"

Pikumin::Pikumin()
{//初期化を行う
	lpModel = nullptr;
	lpPlayerPos = nullptr;
	addPos = nullptr;
}

Pikumin::~Pikumin()
{

}

const float fPI = 3.14159265358979323846264338327950288; //バタースコッチシナモンﾊﾟｲﾊﾟｲﾊﾟｰｲﾊﾟﾊﾟｲﾆ"ﾁｰﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯﾁｯｽﾞｵｫ
void Pikumin::Init()
{//初期化行う
	if (lpModel == nullptr)
	{
		printf("もでるないぞ");
	}
	else
	{
		addPos = lpModel->lpPos();
		AngleY = lpModel->lpAngle();
		D3DXVec3Subtract(&direction, lpPlayerPos, addPos);
		dxFor::Vec3Normalize(&direction);
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;
	}
}

void Pikumin::Update()
{//更新行う
	StaringPlayers();
	StalkingPlayers();
}

void Pikumin::StaringPlayers()
{//プレイヤーを見つめる

	if (*lpPlayerPos != LocalPlayerPos)	//Playerが移動した時
	{
		D3DXVec3Subtract(&direction, lpPlayerPos, addPos);//Player移動後の位置 - 自分の位置で方向を求める
		dxFor::Vec3Normalize(&direction); //正規化す
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;

		LocalPlayerPos = *lpPlayerPos; //Player移動前位置を更新す
	}
}

void Pikumin::StalkingPlayers()
{
	Vec3 toPos; //PlayerとPikuminの距離
	Vec3 localToPos;
	static bool isStalking = false;
	static bool isMoveTarget = false;
	float searchLen = 5.f;

	//①
	D3DXVec3Subtract(&toPos, lpPlayerPos, addPos);
	localToPos = toPos;
	dxFor::Vec3Normalize(&toPos);
	//toPos.y = 0.f;

	//②ストーカー行為開始範囲に入る
	if (!isStalking and searchLen >= D3DXVec3Length(&localToPos))
	{
		isMoveTarget = true;
	}

	//③Playerの位置めがけて移動を開始
	if (isMoveTarget)
	{
		float ksk = 1.f;
		if (10.f <= D3DXVec3Length(&localToPos))
		{
			ksk = 1.9f * ( D3DXVec3Length(&localToPos) - 9.f);
		}

		*addPos += toPos * 0.03f * ksk;

		if (1.f > D3DXVec3Length(&localToPos))
		{
			isStalking = true;
			isMoveTarget = false;
		}
	}

	//④つかまえたぞ
	if (isStalking and 1.f < D3DXVec3Length(&localToPos))
	{
		*addPos = *lpPlayerPos - toPos;
	}

}