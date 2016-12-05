#include "OBB.h"
#include "Model.h"

CollisionOBB::CollisionOBB()
{
	lpModel = nullptr;
};

CollisionOBB::~CollisionOBB()
{

};

const float Inimax = -10000.0f;
const float Inimin = 10000.0f;
HRESULT CollisionOBB::CreateOBB(OBB *obb)
{
	if (lpModel == nullptr)
	{
		return E_INVALIDARG;
	}

	Matrix mRot;

	//最大値最小値の初期設定
	Vec3 max = Vec3(Inimax, Inimax, Inimax);
	Vec3 min = Vec3(Inimin, Inimin, Inimin);

	//メッシュの頂点データ取得
	VERTEX* vertexBuffer = nullptr;
	lpModel->getMesh()->LockVertexBuffer(0, (void**)&vertexBuffer);
	//最大値最小値の取得ループ
	for (int i = 0; i < lpModel->getMesh()->GetNumVertices(); i++)
	{
		Vec3 pos = vertexBuffer[i].position;
		if (pos.x < min.x)min.x = pos.x;
		if (pos.x > max.x)max.x = pos.x;
		if (pos.y < min.y)min.y = pos.y;
		if (pos.y > max.y)max.y = pos.y;
		if (pos.z < min.z)min.z = pos.z;
		if (pos.z > max.z)max.z = pos.z;
	}
	lpModel->getMesh()->UnlockVertexBuffer();

	//中心点取得				/*OBBのワールド座標を足すらしい*/
	obb->c = (min + max) * 0.5f + lpModel->getWorldOffset();

	//方向ベクトル取得
	/* D3DXMatrixRotationYawPitchRoll(&matRot, g_angleY, g_angleX, g_angleZ); */
	mRot = lpModel->getRotation();
	obb->u[0] = Vec3(mRot._11, mRot._12, mRot._13);
	obb->u[1] = Vec3(mRot._21, mRot._22, mRot._23);
	obb->u[2] = Vec3(mRot._31, mRot._32, mRot._33);

	//長さ取得
	obb->e.x = fabsf(max.x - min.x) * 0.5f;
	obb->e.y = fabsf(max.y - min.y) * 0.5f;
	obb->e.z = fabsf(max.z - min.z) * 0.5f;

	return S_OK;
}


bool CollisionOBB::CollisionOBBOBB(OBB *a, OBB *b)
{
	const float EPSILON = 1.175494e-37;

	float R[3][3], AbsR[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R[i][j] = D3DXVec3Dot(&a->u[i], &b->u[j]);
			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
		}
	}

	D3DXVECTOR3 t = b->c - a->c;
	t = D3DXVECTOR3(D3DXVec3Dot(&t, &a->u[0]), D3DXVec3Dot(&t, &a->u[1]), D3DXVec3Dot(&t, &a->u[2]));

	//軸L=A0, L=A1, L=A2判定
	float ra, rb;

	for (int i = 0; i < 3; i++)
	{
		ra = a->e[i];
		rb = b->e[0] * AbsR[i][0] + b->e[1] * AbsR[i][1] + b->e[2] * AbsR[i][2];
		if (fabsf(t[i]) > ra + rb)return false;
	}
	//軸L=B0, L=B1, L=B2判定
	for (int i = 0; i < 3; i++)
	{
		ra = a->e[0] * AbsR[0][i] + a->e[1] * AbsR[1][i] + a->e[2] * AbsR[2][i];
		rb = b->e[i];
		if (fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)return false;
	}

	//軸L=A0 X B0判定
	ra = a->e[1] * AbsR[2][0] + a->e[2] * AbsR[1][0];
	rb = b->e[1] * AbsR[0][2] + b->e[2] * AbsR[0][1];
	if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return false;

	//軸L=A0 X B1判定
	ra = a->e[1] * AbsR[2][1] + a->e[2] * AbsR[1][1];
	rb = b->e[0] * AbsR[0][2] + b->e[2] * AbsR[0][0];
	if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return false;

	//軸L=A0 X B2判定
	ra = a->e[1] * AbsR[2][2] + a->e[2] * AbsR[1][2];
	rb = b->e[0] * AbsR[0][1] + b->e[1] * AbsR[0][0];
	if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return false;

	//軸L=A1 X B0判定
	ra = a->e[0] * AbsR[2][0] + a->e[2] * AbsR[0][0];
	rb = b->e[1] * AbsR[1][2] + b->e[2] * AbsR[1][1];
	if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return false;

	//軸L=A1 X B1判定
	ra = a->e[0] * AbsR[2][1] + a->e[2] * AbsR[0][1];
	rb = b->e[0] * AbsR[1][2] + b->e[2] * AbsR[1][0];
	if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return false;

	//軸L=A1 X B2判定
	ra = a->e[0] * AbsR[2][2] + a->e[2] * AbsR[0][2];
	rb = b->e[0] * AbsR[1][1] + b->e[1] * AbsR[1][0];
	if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return false;

	//軸L=A2 X B0判定
	ra = a->e[0] * AbsR[1][0] + a->e[1] * AbsR[0][0];
	rb = b->e[1] * AbsR[2][2] + b->e[2] * AbsR[2][1];
	if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return false;

	//軸L=A2 X B1判定
	ra = a->e[0] * AbsR[1][1] + a->e[1] * AbsR[0][1];
	rb = b->e[0] * AbsR[2][2] + b->e[2] * AbsR[2][0];
	if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return false;

	//軸L=A2 X B2判定
	ra = a->e[0] * AbsR[1][2] + a->e[1] * AbsR[0][2];
	rb = b->e[0] * AbsR[2][1] + b->e[1] * AbsR[2][0];
	if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)return false;

	return true;
}
