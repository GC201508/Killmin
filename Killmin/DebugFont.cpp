#include "DebugFont.h"
#define LINE_SPACING 30

void DebugFont::Init(LPDIRECT3DDEVICE9 pd3d)
{
	D3DXCreateFont(
		pd3d,				// Direct3D�f�o�C�X
		24,						// ����
		10,						// ��
		FW_REGULAR,				// �t�H���g�̑��� ����
		NULL,					// ����
		FALSE,					// �Α�
		SHIFTJIS_CHARSET,		// �����Z�b�g
		OUT_DEFAULT_PRECIS,		// �o�͐��x�͕���
		PROOF_QUALITY,			// �����i�����d��
		FIXED_PITCH | FF_SCRIPT,	// �s�b�`�ƃt�@�~��
		TEXT("GN-�L���SU����NA"),	// �t�H���g��
		&pFont);					// ID3DXFont�|�C���^
}

const int lineSpace = 30;
const int rcBottom = 24;
void DebugFont::Render()
{
	RECT rc = {
		0,		// �����x���W left
		0,		// �����y���W top
		640,	// �E����x���W right
		24		// �E����y���W bottom
	};
	for (short i = 0; i < textInfo.vText.size(); i++)
	{
		if (textInfo.vText[i] == nullptr) 
			continue;

		rc.top     =		    lineSpace * i;
		rc.bottom  = rcBottom + lineSpace * i;

			// �`��
			pFont->DrawText(
			NULL,							// NULL
			textInfo.vText[i],				// �`��e�L�X�g
			-1,								// �S�ĕ\��
			&rc,							// �\���͈�
			DT_LEFT,						// ����
			D3DCOLOR_XRGB(255, 255, 0)	// ���F
			);
	}
}