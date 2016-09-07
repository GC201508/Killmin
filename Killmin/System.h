#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>

/*	☆ D3DXってのがまどろっこしい ☆	*/
#define Vec3 D3DXVECTOR3  //打つのめんどい
#define Vec4 D3DXVECTOR4  //うつのめんどい
#define Matrix D3DXMATRIX //宇津のめんこい
#define onKey GetAsyncKeyState  //どっこい
#define or ||					//はつこい
/*	-	-	なんたらデバイス	-	-	*/
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice;

/*	-	いろいろ省いて楽したいがな	-	*/
#include "D3dxFormula.h" //計算式

/*	-	-	デバックUIの表示	-	-	*/
const int debuGmodE = 0; //[ON=1][OFF=0]