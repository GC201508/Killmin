#pragma once
/*
	モデルをピクミン化させる
*/
#include "System.h"
class Model;
class Player;
class Pikumin
{
public:
	Pikumin();
	~Pikumin();

	void Init();
	void Update();
	void StaringPlayer();
//set
	void setModel(Model* lp){ lpModel = lp; }
	void setPlayer(Player* lp){ lpPlayer = lp; }
private:
	Model* lpModel;
	Player* lpPlayer;
	Vec3 addPos;
	D3DXMATRIX Rot;            // 板の回転行列
	D3DXMATRIX Offset;         // 板のオフセット行列
};