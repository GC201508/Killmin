#pragma once
/*	-	-	-	-	-	-	-	-	-	-	-	*/
//ベクトルと行列とかの計算が面倒くさいんでまとめますよｫｫｩｰｰ!!!
/*	-	-	-	-	-	-	-	-	-	-	-	*/
#include "System.h"
class dxFor
{
public:
	//ベクトルと行列の乗算
	static void Vec3Transform(Vec3 *lpV, Matrix *lpM)
	{
		Vec3 vTmp = *lpV;
		lpV->x = vTmp.x * lpM->m[0][0] + vTmp.y * lpM->m[1][0] + vTmp.z * lpM->m[2][0];
		lpV->y = vTmp.x * lpM->m[0][1] + vTmp.y * lpM->m[1][1] + vTmp.z * lpM->m[2][1];
		lpV->z = vTmp.x * lpM->m[0][2] + vTmp.y * lpM->m[1][2] + vTmp.z * lpM->m[2][2];
	}

	//正規化の引数ひとつでええやん
	static void Vec3Normalize(Vec3 *lpV)
	{
		float len = sqrt(lpV->x * lpV->x + lpV->y * lpV->y + lpV->z * lpV->z);
		lpV->x /= len;
		lpV->y /= len;
		lpV->z /= len;
	}

	//X軸Z軸の向き
	typedef struct xzDir{ float x = 0; float z = 0; };
};