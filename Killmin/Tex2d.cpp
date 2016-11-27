#include "Tex2d.h"

Tex2d::Tex2d(){}

Tex2d::~Tex2d(){}

void Tex2d::Initialize()
{
	/*D3DXIMAGE_INFO*/ //imgInfo;	//�摜���i�[�p�\����

	D3DXCreateTextureFromFileEx(
		g_pd3dDevice     ,				 //  0Direct3D�f�o�C�X
		this->texFileName,				//  �摜�t�@�C����
		0                ,			   //  ���i�摜�T�C�Y�ł��̂܂܎g�p����ꍇ�͂O�ŗǂ��B
		0                ,			  //  �����i�摜�T�C�Y���̂܂܎g�p����ꍇ�͂O�ŗǂ��B
		0                ,			 //  �v��Mipmap���i�O�w��Ŋ��S�ȃ~�b�v�}�b�v�`�F�[��"�S�Ẳ𑜓x��"�쐬
		0                ,			//  �O�ŗǂ�
		D3DFMT_UNKNOWN   ,		   //  ���̋L�q�Ńt�@�C������t�H�[�}�b�g�擾
		D3DPOOL_DEFAULT  ,		  //  �e�N�X�`���̔z�u�惁�����N���X�B���̋L�q�ŗǂ�
		D3DX_FILTER_NONE ,	     //  �C���[�W�t�B���^�����O���@�B���̋L�q�ŗǂ�
		D3DX_DEFAULT     ,		//  ����ł悢
		0                ,	   //  �����ƂȂ�D3DCOLOR�̒l�B�@�����ɂ���ꍇ�͂O�w��
		&imgInfo         ,	  //  �摜�t�@�C�����i�[��
		NULL             ,	 //  NULL�ŗǂ�
		&this->pTexture);	//  �e�N�X�`���o�͐�

	//�e�N�X�`���̒��_�Z�b�g
	this->texCenter = D3DXVECTOR2(//2D��Ԃ̃x�N�g�����L�q����
		(float)imgInfo.Width / 2,	//
		(float)imgInfo.Height / 2);	//

	RECT rec = { 0, 0, imgInfo.Width, imgInfo.Height };//�`��̈�
	memcpy(&this->rect, &rec, sizeof(RECT));//�`��̈�Z�b�g
}

void Tex2d::Draw(LPD3DXSPRITE pSprite)
{
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);//�X�v���C�g�`��J�n

	pSprite->SetTransform(&this->transformMatrix);//�ϊ��s��Z�b�g

	pSprite->Draw(		//�X�v���C�g�Ƀe�N�X�`���\�t��
		this->pTexture,	//�X�v���C�g�Ɏg���\�[�X �C���[�W��\�� IDirect3DTexture9 �C���^�[�t�F�C�X�ւ̃|�C���^
		&this->rect,		//�\�[�X �e�N�X�`���̂ǂ̕������X�v���C�g�Ɏg���������� RECT �\���̂ւ̃|�C���^
		&D3DXVECTOR3(this->texCenter.x, this->texCenter.y, 0.0f),	//�X�P�[�����O �x�N�g�����i�[���� D3DXVECTOR2 �\���̂ւ̃|�C���^
		NULL,	//�X�N���[�� �s�N�Z���ł̕��s�ړ��l���i�[���� D3DXVECTOR2 �\���̂ւ̃|�C���^(�Ȃ̂�...�H) ���̈����� NULL �̏ꍇ�́A�|�C���g(0, 0) ���g��
		this->backColor);/*D3DCOLOR �^�B���̒l�ɂ��J���[ �`�����l���ƃA���t�@ �`�����l������Z�����B
						   0xFFFFFFFF �Ƃ����l���g���ƁA���̃\�[�X �J���[����уA���t�@ �f�[�^���ێ������B*/

	pSprite->End();//�X�v���C�g�`��I��
}

//�A�j���[�V����Draw
void Tex2d::Draw(LPD3DXSPRITE pSprite, D3DXVECTOR3& position, RECT& rect, D3DXVECTOR2& center)
{
	D3DXMatrixIdentity(&this->transformMatrix);
	D3DXMatrixTransformation2D(&this->transformMatrix, NULL, 0.0f, &D3DXVECTOR2(this->scale.x, this->scale.y),
		NULL, D3DXToRadian(this->angle), &D3DXVECTOR2(position.x, position.y));

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);//�X�v���C�g�`��J�n
	pSprite->SetTransform(&this->transformMatrix);//�ϊ��s��Z�b�g

	pSprite->Draw(
		this->pTexture,
		&rect,
		&D3DXVECTOR3(center.x, center.y, 0.0f),
		NULL,
		this->backColor);

	pSprite->End();
}


void Tex2d::SetupMatrices()
{
	D3DXMatrixIdentity(&this->transformMatrix);//�ϊ��s�񏉊���

	D3DXMatrixTransformation2D(//�ϊ��s��̎Z�o
		&this->transformMatrix,	//�o�͐�A�h���X
		NULL,						//NULL�ł���
		0.0f,						//�O�ł���
		&this->scale,				//�X�P�[�����O�l�A�h���X
		NULL,						//NULL�ł���
		D3DXToRadian(this->angle),//��]�A���O���i���W�A���j	DirectX�̃}�N���֐��ł���D3DXToRadian���Ăяo���A�p�x�����W�A���ɕϊ�����B
		&this->position);			//�ʒu�A�h���X
}