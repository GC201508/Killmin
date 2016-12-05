#pragma once
/*
	モデルにプレイヤー権限を与える
*/
#include "System.h"
class Model;
class Camera;
class ShadowMap;
class Player
{
public:
	Player();
	~Player();
	
	void Init();
	void Update();
//set
	void setInit(Model* lpM = nullptr, Camera* lpC = nullptr, ShadowMap* lpS = nullptr){ lpModel = lpM; lpCam = lpC; lpShadowMap = lpS; Init(); }
	void setModel(Model* lp){ lpModel = lp; Init(); }
	void setCam(Camera* lp){ lpCam = lp; Init(); }
//get
	Model* getLpPlayerModel(){ return lpModel; }
	
private:

	void Move();
	void camToPosUpdate();
	
	Model*		lpModel;
	Camera*		lpCam;
	ShadowMap*	lpShadowMap;
	//Vec3* addPos;
	Vec3 camToPos; //Eye(視覚) - Lookat(注視点)のベクトルを格納する
	
	D3DXVECTOR3 direction;

};