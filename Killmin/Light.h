#pragma once
#include "System.h"

//キャラクターのらいとうらう

#define tLIGHT_NUM 4
class Light
{
public:
	Light();
	~Light();
	void Init();
	void Update();
//get
	int getLNum(){ return lightNum; }
	D3DXVECTOR4* getDiffuseLDir(){ return diffuseLightDirection; }
	D3DXVECTOR4* getDiffuseLColor(){ return diffuseLightColor; }
	D3DXVECTOR4 getAmbientL(){ return ambientLight; }
private:
	int				lightNum = tLIGHT_NUM;
	D3DXVECTOR4 			diffuseLightDirection[tLIGHT_NUM];	//ライトの方向。
	D3DXVECTOR4				diffuseLightColor[tLIGHT_NUM];		//ライトの色。
	D3DXVECTOR4				ambientLight;						//環境光
};