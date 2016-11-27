#include "P_LifeBer.h"
#include <string>

PLifeBer::PLifeBer(){}

PLifeBer::~PLifeBer(){}

void PLifeBer::Initialize()
{
	this->CreateSprite();

	//îwåiâÊëú
	position = D3DXVECTOR2(WindowSizeX / 2.0f, 35);
	angle = 0;
	scale = D3DXVECTOR2(1.7f, 1.7f);
	backColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	texFileName = "Assets/tex2d/MarioGauge_H46_W49.png";
	Tex2d::Initialize();

	//êîílèâä˙âª
	value = 0;
}

void PLifeBer::Update()
{
	this->SetupMatrices();
}

void PLifeBer::DrawLifeBer()
{
	std::string value = std::to_string(this->value);
	for (int idx = value.length() - 1, offset = 0; idx >= 0; idx--, offset++){

		short width = imgInfo.Width / MAX_NUM;
		char now = value[idx];
		int num = atoi(&now);

		RECT rect = {
			num * width,			// ç∂
			0,						// è„
			(num + 1)*width,		// âE
			imgInfo.Height };		// â∫

		D3DXVECTOR2 center(float(width - width / 2),
			float(rect.bottom - rect.bottom / 2));

		D3DXVECTOR3 position(position.x - (width* offset), position.y, NULL);

		this->Draw(pSprite, position, rect, center);
	}
}

int PLifeBer::GetValue()
{
	return value;
}

HRESULT PLifeBer::CreateSprite()
{
	if (FAILED(D3DXCreateSprite(g_pd3dDevice, &pSprite)))
	{
		MessageBox(0, TEXT("ÉXÉvÉâÉCÉgçÏê¨é∏îs"), NULL, MB_OK);
		return E_FAIL;//é∏îsï‘ãp
	}
}