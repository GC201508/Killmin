/*
	11月18日用のゲームモード
*/

#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "XInputMode.h"
#include "Model.h"
#include "ShadowMap.h"
#include "Player.h"
#include "P_LifeBer.h"

class TestGame
{
	Camera camera;
	Light light;
	class Sonya : public Model{ LPCTSTR FileName()override { return "Assets/model/sn/Sonya.X"; } }sonya;
	//class Sonya : public Model{ LPCTSTR FileName()override { return "Assets/model/TyokurituTiger.X"; } }sonya;
	class stage : public Model{ LPCTSTR FileName()override{ return"Assets/model/TestKusaStage.X"; } }stage;
	ShadowMap shadowMap;
	Player player;
	PLifeBer playerLifeBer;
public:
	void Init();
	void Update();
	void Render();
	void Terminate();
};

extern TestGame* testgame;