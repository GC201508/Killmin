#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>
#pragma comment(lib, "winmm.lib")

/*	☆ D3DXってのがまどろっこしい ☆	*/
#define Vec2 D3DXVECTOR2  //入力がめんどい
#define Vec3 D3DXVECTOR3  //打つのめんどい
#define Vec4 D3DXVECTOR4  //うつのめんどい
#define Matrix D3DXMATRIX //宇津のめんこい
#define onKey GetAsyncKeyState  //どっこい
#define onJoyKey Input::joypadKeyDown //い
#define or ||					//ひよこい
#define and &&					//ひなこい
#define constatic const static	//ひまこい
#define vUp Vec3(0.0f, 1.0f, 0.0f)	//こい
/*	-	-	なんたらデバイス	-	-	*/
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice;
/*	-	-	えふぇくとまねじゃー	-	*/
#include "Effect.h"
class Effect;
extern Effect*			g_effect;

/*	-	いろいろ省いて楽したいがな	-	*/
#include "D3dxFormula.h" //計算式

/*	-	-	デバックUIの表示	-	-	*/
const int debuGmodE = 0; //[ON=1][OFF=0]