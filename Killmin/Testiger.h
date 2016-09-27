#pragma once

#include "Model.h"

//虎クラス。
class Tiger : public Model {
public:
	Tiger(){} //コンスト
	~Tiger(){} //デスト

	LPCTSTR FileName()override { return "tiger.x"; }
};