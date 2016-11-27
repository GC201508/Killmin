#pragma once

#include "System.h"

// [参考にしたサイト] ttp://distancevector.web.fc2.com/collision.html
struct OBB
{
		D3DXVECTOR3 c;     //中心点の座標
		D3DXVECTOR3 u[3];  //XYZ の各座標軸の傾きを表す方向ベクトル
		D3DXVECTOR3 e;     //OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）
};

struct VERTEX //メッシュの頂点情報を格納するための構造体
{
		D3DXVECTOR3 position, normal;
		float tu, tv;
};

class Model;

class CollisionOBB
{
public:
	CollisionOBB();
	~CollisionOBB();

	HRESULT CreateOBB(OBB *obb);

	bool CollisionOBBOBB(OBB *a, OBB *b);

//set 
	void setModel(Model *model){ lpModel = model; }
private:
	Model* lpModel;
};