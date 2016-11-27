#pragma once
#include "System.h"

//�L�����N�^�[�̂炢�Ƃ��炤

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
	D3DXVECTOR4 			diffuseLightDirection[tLIGHT_NUM];	//���C�g�̕����B
	D3DXVECTOR4				diffuseLightColor[tLIGHT_NUM];		//���C�g�̐F�B
	D3DXVECTOR4				ambientLight;						//����
};