#pragma once
/*
	プレイヤーの体力を管理する
*/
#include "Tex2d.h"

#define MAX_NUM 9	//体力最大値(9で固定)

class PLifeBer : public Tex2d
{
public:
	PLifeBer();
	~PLifeBer();
	void Initialize()override;
	void Update()override;

	void DrawLifeBer();

	void SetValue(const int value)	//数値の代入
	{
		this->value = value;
	}

	void AddValue(const int value)	//引数で受け取った値を加算
	{
		//値が０以下を避ける条件
		if (this->value + value < 0)
		{
			this->value = 0;
		}
		else if (this->value + value > MAX_NUM - 1)
		{
			this->value = MAX_NUM - 1;
		}
		else
		{
			this->value += value;
		}
	}

	int GetValue();	//現在のvalue

	HRESULT CreateSprite();	//スプライト生成関数

private:
	LPD3DXSPRITE pSprite;	//スプライト
	int value; //数値
};