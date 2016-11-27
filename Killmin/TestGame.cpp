#include "TestGame.h"

void TestGame::Init()
{
	light.Init();

	sonya.Init(g_pd3dDevice);
	stage.Init(g_pd3dDevice);
	
	sonya.SetPosition(Vec3(0.0f, 1.04f, 0.0f));
	stage.SetPosition(Vec3(0.0f, 0.0f, 0.0f));
	
	stage.setShadowMap(&shadowMap);
	stage.setIsShadowReceiver(true);

	player.setInit(&sonya, &camera, &shadowMap);

	shadowMap.Init(g_pd3dDevice, &camera, &light, player.getLpPlayerModel());

	camera.Init();

	playerLifeBer.Initialize();

#if debuGmodE
	//�t�H���g������
	Dfont->Init(g_pd3dDevice);
#endif //__debuGmodE
}

void TestGame::Render()
{
#if debuGmodE
	//�f�o�b�N�p�ϐ�
	double time = sw->GetElapsed(); //FPS
	float leftStickX = XInput->getLeftStickX();
	float leftStickY = XInput->getLeftStickY();
#endif //_debuGmodE

	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

#if debuGmodE
	//�f�o�b�O�e�L�X�g�\��,
	char timetext[256];	//FPS
	char testText2[256];    //�e�X�g1
	sprintf(timetext, "fps = %lf\n", 1.0f / time);
	sprintf(testText2, "LStick : X = %f Y = %f", leftStickX, leftStickY);
	Dfont->AddText(timetext);
	Dfont->AddText(testText2);
	Dfont->Render();
#endif //_debuGmodE

	shadowMap.Draw(g_pd3dDevice);

	sonya.Render(g_pd3dDevice, camera, light);
	stage.Render(g_pd3dDevice, camera, light);

/*2DTex Dorw*/
	playerLifeBer.DrawLifeBer();

	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

#if debuGmodE
	//FPS�̌v��,
	sw->Stop();
#endif //_debuGmodE
}

void TestGame::Update()
{
#if debuGmodE
	//FPS�̌v��,
	sw->Start();
#endif //_debuGmodE

	light.Update();
	camera.Update();

	sonya.Update();
	stage.Update();

	player.Update();

	shadowMap.Update();

	playerLifeBer.Update();
	if (onKey('T'))
	{
		playerLifeBer.AddValue(-1);
	}
	else if (onKey('Y'))
	{
		playerLifeBer.AddValue(1);
	}
}

void TestGame::Terminate()
{
	sonya.Release();
	stage.Release();
}