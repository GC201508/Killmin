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
//	void setPlayer(Player* lp){ *lpPlayerPos = lp->getPlayerPos(); }
private:
	Model* lpModel;
	Vec3 addPos;
	Vec3* lpPlayerPos;
	D3DXMATRIX Rot;            // �̉�]�s��
	D3DXMATRIX Offset;         // �̃I�t�Z�b�g�s��
};