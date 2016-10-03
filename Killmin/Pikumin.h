#pragma once
/*
	モデルをピクミン化させる
*/
#include "System.h"
class Model;
class Pikumin
{
public:
	Pikumin();
	~Pikumin();

	void Init();
	void Update();
	
	void StaringPlayers();
	void StalkingPlayers();
//set
	void setModel(Model* lp){ lpModel = lp; }
	void setLPPlayerPos(Vec3* lp){ lpPlayerPos = lp; LocalPlayerPos = *lpPlayerPos; }
private:
	Model* lpModel;
	Vec3* addPos;
	Vec3* lpPlayerPos;
	float* AngleY;
	Vec3 LocalPlayerPos;
	D3DXVECTOR3 direction;
};