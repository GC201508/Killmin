#pragma once
#include "System.h"
#include "OBB.h"
class Light;
class Camera;
class ShadowMap;


//���ł����炤
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

	// * @brief		ShadowMap�N���X
	void setShadowMap(ShadowMap* sm){ lpShadowMap = sm; }
	
	void setIsShadowReceiver(bool b){ isShadowReceiver = b; }

	void setIsReversalModel(bool b){ isReversalModel = b; }

	void setIsToonShader(bool b){ isToonShader = b; }
//get
	D3DXVECTOR3 GetPosition();
	Vec3 getWorldOffset(){ Matrix w = mWorld; return Vec3(w._41,w._42,w._43); }
	Matrix getRotation(){ return mRotation; }
	LPD3DXMESH getMesh(){ return mesh; }

	Vec3* lpPos()    { return &position; }	//pos�A�h���X
	float* lpAngle() { return &angle; }		//angle�A�h���X

protected:
	//�ǂݍ���X�t�@�C���w�肷
	virtual LPCTSTR FileName() = 0;

	D3DXVECTOR3 position;			//���W,
	LPD3DXMESH mesh;				//���b�V��,
	LPDIRECT3DTEXTURE9* textures;	//�e�N�X�`��,
	DWORD numMaterial;				//�}�e���A��,
	ID3DXEffect*	effect;			//�G�t�F�N�g,
	D3DXMATRIX	mWorld;				//���[���h�s��,
	D3DXMATRIX	mRotation;			//��]�s��,

	Matrix mTrans;
	float angle;

	/*ShadowMap*/
	ShadowMap* lpShadowMap;	//ShadowMap�N���X�̂ۂ���,
	bool	isRenderShadowMap;		//�V���h�}�b�v�ŕ`�悷��,
	bool	isShadowReceiver;		//�V���h�E���V�[�o�[,
	bool	isReversalModel;		//�֊s�i���f���̔��]�j,

	/*ToonShader*/
	bool	isToonShader;			//�g�D�[���V�F�[�_,
	LPDIRECT3DTEXTURE9 texToonMap;	//�g�D�[���}�b�v
	//�V�[���̃J���[�����i�[����T�[�t�F�C�X�錾
	LPDIRECT3DTEXTURE9 pColorTexture;  //
	LPDIRECT3DSURFACE9 pColorSurface;  //
	//�V�[����Z�l�����i�[����T�[�t�F�C�X�錾
	LPDIRECT3DTEXTURE9 pZMapTexture;  //
	LPDIRECT3DSURFACE9 pZMapSurface;  //

	int		reversalModelCnt;		//�d���h�~,

	/* OBB */
	OBB modelOBB;
};