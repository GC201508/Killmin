#pragma once
#include "System.h"
class Model;
class Player
{
public:
	Player();
	~Player();
	
	void Init();
	void Update();

	void onKeyMove();
//set
	void setModel(Model* lp){ lpModel = lp; }
private:
	Model* lpModel;
	D3DXVECTOR3* lpPos;
};