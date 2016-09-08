#include "System.h"
#include "Camera.h"
#include "Light.h"
//�g�p���f��
#include "Testiger.h"
#include "Ground.h"
#include "Sonya.h"
//����
#include "Player.h"
#include "Pikumin.h"
//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

//lib
Camera camera;//�J����
Light light;//���C�g

//model
Tiger tora;//�Ƃ炿���
Ground ground;//�n��
Sonya sonya;//�\�[�j�������

//�Ȃ�Ƃ��Ȃ�Ƃ�
Player player;//�v���C���[
Pikumin pikumin;

//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	//���C�g���������B
	light.Init();

	//���ł����傫��
	tora.Init(g_pd3dDevice);
	tora.SetPosition(D3DXVECTOR3(0.5f, 0.77f, 0.0f));
	ground.Init(g_pd3dDevice);
	ground.SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	sonya.Init(g_pd3dDevice);
	sonya.SetPosition(D3DXVECTOR3(-3.0f, 1.04f, 0.0f));
	
	//�Ղꂢ����ǉ�
	player.setModel(&tora);

	//�҂��݂���ǉ�
	//pikumin.setModel(&tora);

	//�����炵�傫��
	camera.Init();
}
//-----------------------------------------------------------------------------
// Name: �`�揈���B
//-----------------------------------------------------------------------------
VOID Render()
{
	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();

	/*	-	-	-	-	-	-	-	-	*/
	tora.Render( g_pd3dDevice,camera,light); //�Ƃ炿���
	ground.Render(g_pd3dDevice, camera, light);//�n��
	sonya.Render(g_pd3dDevice, camera, light);//�\�[�j�������
	/*	-	-	-	-	-	-	-	-	*/
	
	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
/*!-----------------------------------------------------------------------------
*@brief	�X�V�����B
-----------------------------------------------------------------------------*/
void Update()
{
	light.Update(); //�炢��
	camera.Update();//���߂�
	tora.Update();//�Ƃ炿���
	sonya.Update();//���ɂႿ���
	
	player.Update();//�Ղꂢ��
	pikumin.Update();//�҂��݂�
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
	tora.Release();
	ground.Release();
	sonya.Update();
}
