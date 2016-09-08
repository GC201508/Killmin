#pragma once
/*
	モデルにプレイヤー権限を与える
*/
#include "System.h"
class Model;
class Player
{
public:
	Player();
	~Player();
	
	void Init();
	void Update();

	void Move();
	void ChangeModel(Model* lp);
//set
	void setModel(Model* lp){ lpModel = lp; Init(); }
private:
	Model* lpModel;
	Vec3* addPos;
	float* AngleY;
};