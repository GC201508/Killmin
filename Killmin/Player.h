#pragma once
/*
	���f���Ƀv���C���[������^����
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
	Vec3 camToPos; //Eye(���o) - Lookat(�����_)�̃x�N�g�����i�[����
	float* AngleY;
	D3DXVECTOR3 direction;

};