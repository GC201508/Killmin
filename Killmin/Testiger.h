#pragma once

#include "Model.h"

//�ՃN���X�B
class Tiger : public Model {
public:
	Tiger(){} //�R���X�g
	~Tiger(){} //�f�X�g

	LPCTSTR FileName()override { return "tiger.x"; }
};