#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "InputKey.h"
//model
#include "Model.h"

//�I�t�X�N���[��
#include "Sprite.h"
#include "RenderTarget.h"
//����
#include "Player.h"
#include "Pikumin.h"
//�X�L�����f��
#include "SkinModel.h"
#include "ModelData.h"
#include "Animation.h"

#if debuGmodE
//�f�o�b�O
#include "DebugFont.h"
DebugFont Dfont;
#endif //_debuGmodE

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

//lib
Camera camera;//�J����
Light light;//���C�g

//model
class Tiger : public Model{ LPCTSTR FileName()override { return "Assets/model/tiger.X"; } }tora;//�Ƃ炿���
class Ground : public Model{ LPCTSTR FileName()override { return "Assets/model/ground.X"; } }ground;//�n��
class Sonya : public Model{ LPCTSTR FileName()override { return "Assets/model/sn/�\�[�j��.X"; } }sonya;//�\�[�j�������
class TestPikumin : public Model{ LPCTSTR FileName()override { return "Assets/model/TyokurituTiger.X"; } }pikuTiger;//�s�N�~���^�C�K�[�����

//TODO:(�����܂�,)�f�G�ȃ��C�g�m�F�p,
class ball : public Model{ LPCTSTR FileName()override { return "Assets/model/sphere.X"; } }testball;//�e�X�g�{�[��


//�I�t�X�N���[��
Sprite sprite;
RenderTarget renderTarget;

//�Ȃ�Ƃ��Ȃ�Ƃ�
Player player;//�v���C���[
Pikumin pikumin;//�҂��݂�

//�X�L�����f��
SkinModel skinmodel;
ModelData modeldata;
Animation animation;

//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	//���C�g���������B
	light.Init();

	//���ł����傫��,
	tora.Init(g_pd3dDevice);
	tora.SetPosition(D3DXVECTOR3(0.5f, 0.77f, 0.0f));
	ground.Init(g_pd3dDevice);
	ground.SetPosition(D3DXVECTOR3(0.0f, -3.5f, 0.0f));
	sonya.Init(g_pd3dDevice);
	sonya.SetPosition(D3DXVECTOR3(-3.0f, 1.04f, 0.0f));
	pikuTiger.Init(g_pd3dDevice);
	pikuTiger.SetPosition(Vec3(0.f, 0.f, 10.f));
	
	testball.Init(g_pd3dDevice);
	testball.SetPosition(Vec3(10.f, 20.f, 200.f));

	//�I�t�X�N���[��
	sprite.Init(g_pd3dDevice);
	renderTarget.Create(g_pd3dDevice,
		400,//����,
		400,//����,
		1,	//�����_�����O�^�[�Q�b�g�ɂ̓~�b�v�}�b�v�͕s�v�Ȃ̂ňꖇ�̂݁B
		D3DFMT_A8R8G8B8,	//�J���[�o�b�t�@�̃t�H�[�}�b�g��ARGB��32bit
		D3DFMT_D16,	//�w���̂o�b��24bit�̐[�x�o�b�t�@���쐬�ł��Ȃ������̂ŁA16�r�b�g�Ő[�x�o�b�t�@���쐬����B
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�͂Ȃ��B
		0	//�}���`�T���v�����O���Ȃ��̂łO���w��B
		);
	renderTarget.InitGeometry(g_pd3dDevice);

	//�Ղꂢ����ǉ�,
	player.setInit(&tora, &camera);

	//�҂��݂���ǉ�,
	pikumin.setModel(&pikuTiger);
	pikumin.setLPPlayerPos(player.getLpPlayerModel()->lpPos);
	pikumin.Init();

	//�����炵�傫��,
	camera.Init();

	//���f�����[�h,
	modeldata.LoadModelData("Assets/model/Unity.X", &animation);
	skinmodel.Init(&modeldata);
	skinmodel.SetLight(&light);
	animation.PlayAnimation(0);

#if debuGmodE
	//�t�H���g������
	Dfont.Init(g_pd3dDevice);
#endif //__debuGmodE

}
//TODO:����
CStopwatch sw;
//-----------------------------------------------------------------------------
// Name: �`�揈���B
//-----------------------------------------------------------------------------
VOID Render()
{
	double time = sw.GetElapsed();


	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();	

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
#if debuGmodE

	Dfont.Render(time);
#endif

	/*	-	-	-	-	-	-	-	-	*/
	tora.Render( g_pd3dDevice,camera,light); //�Ƃ炿���,
	ground.Render(g_pd3dDevice, camera, light);//�n��,
	sonya.Render(g_pd3dDevice, camera, light);//�\�[�j�������,
	pikuTiger.Render(g_pd3dDevice, camera, light);//�s�N�~���Ղ����,
	testball.Render(g_pd3dDevice, camera, light);//�e�X�g�{�[��,
	skinmodel.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
	/*	-	-	-	-	-	-	-	-	*/
	
	/*�I�t�X�N���[��*/

	renderTarget.Render(g_pd3dDevice,camera);

	static int renderCount = 0;
	renderCount++;
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, renderCount / 500.0f);
	sprite.Draw(matWorld,camera,renderTarget.GetTexture());

	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();


	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
#if debuGmodE
	//�f�o�b�O�e�L�X�g�\��
	sw.Stop();
#endif //_debuGmodE

}
/*!-----------------------------------------------------------------------------
*@brief	�X�V�����B
-----------------------------------------------------------------------------*/
void Update()
{
	sw.Start();
	animation.Update(1.0f / 60.0f);//anime

	light.Update(); //�炢��
	camera.Update();//���߂�


	tora.Update();//�Ƃ炿���
	sonya.Update();//���ɂႿ���
	ground.Update();//���߂�
	pikuTiger.Update();//�҂��҂��Ƃ炿���


	testball.Update();//�e�X�g�{�A�[��

	
	player.Update();//�Ղꂢ��
	pikumin.Update();//�҂��݂�

	if (InputKey::InputHoldCode(VK_SPACE))
	{
		animation.PlayAnimation(3);
	}

	skinmodel.UpdateWorldMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
	tora.Release();
	ground.Release();
	sonya.Release();
	pikuTiger.Release();

	testball.Release();
}
