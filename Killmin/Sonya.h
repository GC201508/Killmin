#pragma once
#include "Model.h"

class Sonya : public Model
{
public:
	Sonya(){}
	~Sonya(){}

	LPCTSTR FileName()override { return "sn/ƒ\[ƒjƒƒ.x"; }
};