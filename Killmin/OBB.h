#pragma once

#include "System.h"

// [�Q�l�ɂ����T�C�g] ttp://distancevector.web.fc2.com/collision.html
struct OBB
{
		D3DXVECTOR3 c;     //���S�_�̍��W
		D3DXVECTOR3 u[3];  //XYZ �̊e���W���̌X����\�������x�N�g��
		D3DXVECTOR3 e;     //OBB �̊e���W���ɉ����������̔����i���S�_����ʂ܂ł̒����j
};

struct VERTEX //���b�V���̒��_�����i�[���邽�߂̍\����
{
		D3DXVECTOR3 position, normal;
		float tu, tv;
};

class Model;

class CollisionOBB
{
public:
	CollisionOBB();
	~CollisionOBB();

	HRESULT CreateOBB(OBB *obb);

	bool CollisionOBBOBB(OBB *a, OBB *b);

//set 
	void setModel(Model *model){ lpModel = model; }
private:
	Model* lpModel;
};