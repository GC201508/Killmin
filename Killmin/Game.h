#include "System.h"

//���ɕ��ނ����炢����
#include "Camera.h"
#include "Light.h"
#include "InputKey.h"
#include "XInputMode.h"

//Physics
#include "Physics.h"
#include "RigidBody.h"

//model
#include "Model.h"

//�I�t�X�N���[��
#include "Sprite.h"
#include "RenderTarget.h"

//�V���h�}�b�v
#include "ShadowMap.h"

//����
#include "Player.h"
#include "Pikumin.h"

//�X�L�����f��
#include "SkinModel.h"
#include "ModelData.h"
#include "Animation.h"

class Game
{
	//lib
	Camera camera;	//�J����
	Light  light ;  //���C�g

	//Physics
	PhysicsWorld physicsWorld;	//�q���C�W�b�N�X���[���h

	//model
	class Tiger       : public Model{ LPCTSTR FileName()override { return "Assets/model/tiger.X"         ; } }tora		;      //�Ƃ炿���
	class Ground      : public Model{ LPCTSTR FileName()override { return "Assets/model/ground.X"        ; } }ground	;     //�n��
	class Sonya       : public Model{ LPCTSTR FileName()override { return "Assets/model/sn/�\�[�j��.X"   ; } }sonya		;    //�\�[�j�������
	class TestPikumin : public Model{ LPCTSTR FileName()override { return "Assets/model/TyokurituTiger.X"; } }pikuTiger	;	//�s�N�~���^�C�K�[�����
	class TestStage : public Model{ LPCTSTR FileName()override{ return"Assets/model/TestKusaStage.X"; } }testStage;		   //�e�X�g�X�e�[�W
	//TODO:(�����܂�,)�f�G�ȃ��C�g�m�F�p,
	class ball        : public Model{ LPCTSTR FileName()override { return "Assets/model/sphere.X"        ; } }testball	;	//�e�X�g�{�[��


	//�I�t�X�N���[��
	Sprite			sprite		;
	RenderTarget	renderTarget;

	//�V���h�}�b�v,
	ShadowMap shadowMap;	//�V���h�}�b�v,

	//�Ȃ�Ƃ��Ȃ�Ƃ�
	Player		player;		//�v���C���[
	Pikumin		pikumin;	//�҂��݂�

	//�X�L�����f��
	SkinModel skinmodel;
	ModelData modeldata;
	Animation animation;

public:
	void GameInit();
	void GameUpdate();
	void GameRender();
	void GameTerminate();

//get
	//�������[���h�̎擾�B
	PhysicsWorld* GetPhysicsWorld()
	{
		return &physicsWorld;
	}
};

extern Game* game;