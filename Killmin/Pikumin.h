#pragma once
/*
	���f�����s�N�~����������
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
	D3DXMATRIX Rot;            // �̉�]�s��
	D3DXMATRIX Offset;         // �̃I�t�Z�b�g�s��
};