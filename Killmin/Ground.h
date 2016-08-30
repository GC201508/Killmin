#pragma once
#include "Model.h"

class Ground : public Model
{
public:
	Ground(){}
	~Ground(){}

	LPCTSTR FileName()override { return "ground.x"; }
};