#include "DebugFont.h"

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

void DebugFont::Render(double d)
{

	char text[256];
	sprintf(text, "fps = %lf\n", 1.0f / d);

	RECT rc = {
		0,		// 左上のx座標
		0,		// 左上のy座標
		640,	// 右下のx座標
		24		// 右下のy座標
	};
	// 描画
	pFont->DrawText(
		NULL,							// NULL
		text,							// 描画テキスト
		-1,								// 全て表示
		&rc,							// 表示範囲
		DT_LEFT,						// 左寄せ
		D3DCOLOR_XRGB(255, 255, 0)	// 黄色
		);
}