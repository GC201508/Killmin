#pragma once
/*
	モデルにプレイヤー権限を与える
*/
#include "System.h"
class Model;
class Camera;
class Player
{
public:
	Player();
	~Player();
	
	void Init();
	void Update();
//set
	void setInit(Model* lpM = nullptr, Camera* lpC = nullptr){ lpModel = lpM; lpCam = lpC; Init(); }
	void setModel(Model* lp){ lpModel = lp; Init(); }
	void setCam(Camera* lp){ lpCam = lp; Init(); }
	
private:

	void Move();
	void camToPosUpdate();

	Model* lpModel;
	Camera* lpCam;
	Vec3* addPos;
	Vec3 camToPos; //Eye(視覚) - Lookat(注視点)のベクトルを格納する
	float* AngleY;
	D3DXVECTOR3 direction;

};