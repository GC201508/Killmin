#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>

/*	�� D3DX���Ă̂��܂ǂ�������� ��	*/
#define Vec3 D3DXVECTOR3  //�ł̂߂�ǂ�
#define Vec4 D3DXVECTOR4  //���̂߂�ǂ�
#define Matrix D3DXMATRIX //�F�Â̂߂񂱂�
#define onKey GetAsyncKeyState  //�ǂ�����
#define or ||					//�͂���
/*	-	-	�Ȃ񂽂�f�o�C�X	-	-	*/
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice;

/*	-	���낢��Ȃ��Ċy����������	-	*/
#include "D3dxFormula.h" //�v�Z��

/*	-	-	�f�o�b�NUI�̕\��	-	-	*/
const int debuGmodE = 0; //[ON=1][OFF=0]