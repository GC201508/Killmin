#include "DebugFont.h"
#define LINE_SPACING 30

void DebugFont::Init(LPDIRECT3DDEVICE9 pd3d)
{
	D3DXCreateFont(
		pd3d,				// Direct3Dデバイス
		24,						// 高さ
		10,						// 幅
		FW_REGULAR,				// フォントの太さ 普通
		NULL,					// 下線
		FALSE,					// 斜体
		SHIFTJIS_CHARSET,		// 文字セット
		OUT_DEFAULT_PRECIS,		// 出力制度は普通
		PROOF_QUALITY,			// 文字品質を重視
		FIXED_PITCH | FF_SCRIPT,	// ピッチとファミリ
		TEXT("GN-キルゴUかなNA"),	// フォント名
		&pFont);					// ID3DXFontポインタ
}

const int lineSpace = 30;
const int rcBottom = 24;
void DebugFont::Render()
{
	RECT rc = {
		0,		// 左上のx座標 left
		0,		// 左上のy座標 top
		640,	// 右下のx座標 right
		24		// 右下のy座標 bottom
	};
	for (short i = 0; i < textInfo.vText.size(); i++)
	{
		if (textInfo.vText[i] == nullptr) 
			continue;

		rc.top     =		    lineSpace * i;
		rc.bottom  = rcBottom + lineSpace * i;

			// 描画
			pFont->DrawText(
			NULL,							// NULL
			textInfo.vText[i],				// 描画テキスト
			-1,								// 全て表示
			&rc,							// 表示範囲
			DT_LEFT,						// 左寄せ
			D3DCOLOR_XRGB(255, 255, 0)	// 黄色
			);
	}
}