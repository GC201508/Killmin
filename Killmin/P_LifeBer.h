#pragma once
/*
	�v���C���[�̗̑͂��Ǘ�����
*/
#include "Tex2d.h"

#define MAX_NUM 9	//�̗͍ő�l(9�ŌŒ�)

class PLifeBer : public Tex2d
{
public:
	PLifeBer();
	~PLifeBer();
	void Initialize()override;
	void Update()override;

	void DrawLifeBer();

	void SetValue(const int value)	//���l�̑��
	{
		this->value = value;
	}

	void AddValue(const int value)	//�����Ŏ󂯎�����l�����Z
	{
		//�l���O�ȉ�����������
		if (this->value + value < 0)
		{
			this->value = 0;
		}
		else if (this->value + value > MAX_NUM - 1)
		{
			this->value = MAX_NUM - 1;
		}
		else
		{
			this->value += value;
		}
	}

	int GetValue();	//���݂�value

	HRESULT CreateSprite();	//�X�v���C�g�����֐�

private:
	LPD3DXSPRITE pSprite;	//�X�v���C�g
	int value; //���l
};