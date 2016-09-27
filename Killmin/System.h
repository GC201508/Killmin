#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>
#pragma comment(lib, "winmm.lib")

/*	�� D3DX���Ă̂��܂ǂ�������� ��	*/
#define Vec2 D3DXVECTOR2  //���͂��߂�ǂ�
#define Vec3 D3DXVECTOR3  //�ł̂߂�ǂ�
#define Vec4 D3DXVECTOR4  //���̂߂�ǂ�
#define Matrix D3DXMATRIX //�F�Â̂߂񂱂�
#define onKey GetAsyncKeyState  //�ǂ�����
#define onJoyKey Input::joypadKeyDown //��
#define or ||					//�Ђ悱��
#define and &&					//�ЂȂ���
#define constatic const static	//�Ђ܂���
#define vUp Vec3(0.0f, 1.0f, 0.0f)	//����
/*	-	-	�Ȃ񂽂�f�o�C�X	-	-	*/
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice;
/*	-	-	���ӂ����Ƃ܂˂���[	-	*/
#include "Effect.h"
class Effect;
extern Effect*			g_effect;

/*	-	���낢��Ȃ��Ċy����������	-	*/
#include "D3dxFormula.h" //�v�Z��

/*	-	-	�f�o�b�NUI�̕\��	-	-	*/
const int debuGmodE = 0; //[ON=1][OFF=0]