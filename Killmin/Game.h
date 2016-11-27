#include "System.h"

//何に分類したらいいの
#include "Camera.h"
#include "Light.h"
#include "InputKey.h"
#include "XInputMode.h"

//Physics
#include "Physics.h"
#include "RigidBody.h"

//model
#include "Model.h"

//オフスクリーン
#include "Sprite.h"
#include "RenderTarget.h"

//シャドマップ
#include "ShadowMap.h"

//役割
#include "Player.h"
#include "Pikumin.h"

//スキンモデル
#include "SkinModel.h"
#include "ModelData.h"
#include "Animation.h"

class Game
{
	//lib
	Camera camera;	//カメラ
	Light  light ;  //ライト

	//Physics
	PhysicsWorld physicsWorld;	//ヒュイジックスワールド

	//model
	class Tiger       : public Model{ LPCTSTR FileName()override { return "Assets/model/tiger.X"         ; } }tora		;      //とらちゃん
	class Ground      : public Model{ LPCTSTR FileName()override { return "Assets/model/ground.X"        ; } }ground	;     //地面
	class Sonya       : public Model{ LPCTSTR FileName()override { return "Assets/model/sn/ソーニャ.X"   ; } }sonya		;    //ソーニャちゃん
	class TestPikumin : public Model{ LPCTSTR FileName()override { return "Assets/model/TyokurituTiger.X"; } }pikuTiger	;	//ピクミンタイガーちゃん
	class TestStage : public Model{ LPCTSTR FileName()override{ return"Assets/model/TestKusaStage.X"; } }testStage;		   //テストステージ
	//TODO:(たけまさ,)素敵なライト確認用,
	class ball        : public Model{ LPCTSTR FileName()override { return "Assets/model/sphere.X"        ; } }testball	;	//テストボール


	//オフスクリーン
	Sprite			sprite		;
	RenderTarget	renderTarget;

	//シャドマップ,
	ShadowMap shadowMap;	//シャドマップ,

	//なんとかなんとか
	Player		player;		//プレイヤー
	Pikumin		pikumin;	//ぴくみん

	//スキンモデル
	SkinModel skinmodel;
	ModelData modeldata;
	Animation animation;

public:
	void GameInit();
	void GameUpdate();
	void GameRender();
	void GameTerminate();

//get
	//物理ワールドの取得。
	PhysicsWorld* GetPhysicsWorld()
	{
		return &physicsWorld;
	}
};

extern Game* game;