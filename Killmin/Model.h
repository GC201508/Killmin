#pragma once
#include "System.h"
#include "OBB.h"
class Light;
class Camera;
class ShadowMap;


//もでううらう
class Model{
public:
	Model();
	~Model();
	void Init(LPDIRECT3DDEVICE9 pd3dDevicde);

	void Update();

	void Render(LPDIRECT3DDEVICE9 pd3dDevice,Camera camera,Light light);

	void SetUpTechnique(LPDIRECT3DDEVICE9 pd3dDevice, Camera camera, Light light,int loopCnt);

	void Release();

//set
	void SetPosition(D3DXVECTOR3 pos);
	void setWorld(Matrix wol){ mWorld = wol; }
	void setIsRenderShadowMap(bool b){ isRenderShadowMap = b; }

	void setAngleX(float x){ angleX = x; }
	void setAngleY(float y){ angleY = y; }
	void setAngleZ(float z){ angleZ = z; }

	void addAngleX(float x){ angleX += x; }
	void addAngleY(float y){ angleY += y; }
	void addAngleZ(float z){ angleZ += z; }

	// * @brief		ShadowMapクラス
	void setShadowMap(ShadowMap* sm){ lpShadowMap = sm; }
	
	void setIsShadowReceiver(bool b){ isShadowReceiver = b; }

	void setIsReversalModel(bool b){ isReversalModel = b; }

	void setIsToonShader(bool b){ isToonShader = b; }
//get
	D3DXVECTOR3 GetPosition();
	Vec3 getWorldOffset(){ Matrix w = mWorld; return Vec3(w._41,w._42,w._43); }
	Matrix getRotation(){ return mRotation; }
	LPD3DXMESH getMesh(){ return mesh; }
	OBB getOBB(){ return modelOBB; }

	float getAngleX(){ return angleX; }
	float getAngleY(){ return angleY; }
	float getAngleZ(){ return angleZ; }

	Vec3* lpPos()    { return &position; }	//posアドレス
	//float* lpAngle() { return &angleY; }		//angleアドレ

protected:
	//読み込むXファイル指定す
	virtual LPCTSTR FileName() = 0;

	D3DXVECTOR3 position;			//座標,
	LPD3DXMESH mesh;				//メッシュ,
	LPDIRECT3DTEXTURE9* textures;	//テクスチャ,
	DWORD numMaterial;				//マテリアル,
	ID3DXEffect*	effect;			//エフェクト,
	D3DXMATRIX	mWorld;				//ワールド行列,
	D3DXMATRIX	mRotation;			//回転行列,

	Matrix mTrans;

	float angleX;
	float angleY;
	float angleZ;

	/*ShadowMap*/
	ShadowMap* lpShadowMap;	//ShadowMapクラスのぽいんた,
	bool	isRenderShadowMap;		//シャドマップで描画する,
	bool	isShadowReceiver;		//シャドウレシーバー,
	bool	isReversalModel;		//輪郭（モデルの反転）,

	/*ToonShader*/
	bool	isToonShader;			//トゥーンシェーダ,
	LPDIRECT3DTEXTURE9 texToonMap;	//トゥーンマップ
	//シーンのカラー情報を格納するサーフェイス宣言
	LPDIRECT3DTEXTURE9 pColorTexture;  //
	LPDIRECT3DSURFACE9 pColorSurface;  //
	//シーンのZ値情報を格納するサーフェイス宣言
	LPDIRECT3DTEXTURE9 pZMapTexture;  //
	LPDIRECT3DSURFACE9 pZMapSurface;  //

	int		reversalModelCnt;		//重複防止,

	/* OBB */
	OBB modelOBB;
};