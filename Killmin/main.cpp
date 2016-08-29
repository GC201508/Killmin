#include "System.h"
#include "Camera.h"
//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

const int				LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

Camera camera;//�J����


/*!-----------------------------------------------------------------------------
*@brief	���C�g���X�V�B
-----------------------------------------------------------------------------*/
void UpdateLight()
{
	g_diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	g_diffuseLightDirection[1] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	g_diffuseLightDirection[2] = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 1.0f);
	g_diffuseLightDirection[3] = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f);

	//�f�B�t���[�Y���C�g�B
	g_diffuseLightColor[0] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[1] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[2] = D3DXVECTOR4(0.5f, 0.4f, 0.6f, 1.0f);
	g_diffuseLightColor[3] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//�����B
	g_ambientLight = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	//���C�g���������B
	ZeroMemory(g_diffuseLightDirection, sizeof(g_diffuseLightDirection));
	ZeroMemory(g_diffuseLightColor, sizeof(g_diffuseLightColor));
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
	//���C�g�̍X�V�B
	UpdateLight();
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{

}
