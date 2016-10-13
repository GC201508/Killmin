#include "DebugFont.h"

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

void DebugFont::Render(double d)
{

	char text[256];
	sprintf(text, "fps = %lf\n", 1.0f / d);

	RECT rc = {
		0,		// �����x���W
		0,		// �����y���W
		640,	// �E����x���W
		24		// �E����y���W
	};
	// �`��
	pFont->DrawText(
		NULL,							// NULL
		text,							// �`��e�L�X�g
		-1,								// �S�ĕ\��
		&rc,							// �\���͈�
		DT_LEFT,						// ����
		D3DCOLOR_XRGB(255, 255, 0)	// ���F
		);
}