#include "Pikumin.h"
#include "Model.h"
#include "Player.h"

Pikumin::Pikumin()
{//���������s��
	lpModel = nullptr;
	lpPlayerPos = nullptr;
	addPos = nullptr;
}

Pikumin::~Pikumin()
{

}

const float fPI = 3.14159265358979323846264338327950288; //�o�^�[�X�R�b�`�V�i�����߲�߲�߰����߲�"�����������������޵�
void Pikumin::Init()
{//�������s��
	if (lpModel == nullptr)
	{
		printf("���ł�Ȃ���");
	}
	else
	{
		addPos = lpModel->lpPos();
		AngleY = lpModel->lpAngle();
		D3DXVec3Subtract(&direction, lpPlayerPos, addPos);
		dxFor::Vec3Normalize(&direction);
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;
	}
}

void Pikumin::Update()
{//�X�V�s��
	StaringPlayers();
	StalkingPlayers();
}

void Pikumin::StaringPlayers()
{//�v���C���[�����߂�

	if (*lpPlayerPos != LocalPlayerPos)	//Player���ړ�������
	{
		D3DXVec3Subtract(&direction, lpPlayerPos, addPos);//Player�ړ���̈ʒu - �����̈ʒu�ŕ��������߂�
		dxFor::Vec3Normalize(&direction); //���K����
		float angle = 2 * fPI - atan2f(direction.z, direction.x);
		*AngleY = angle - 0.5f * fPI;

		LocalPlayerPos = *lpPlayerPos; //Player�ړ��O�ʒu���X�V��
	}
}

void Pikumin::StalkingPlayers()
{
	Vec3 toPos; //Player��Pikumin�̋���
	Vec3 localToPos;
	static bool isStalking = false;
	static bool isMoveTarget = false;
	float searchLen = 5.f;

	//�@
	D3DXVec3Subtract(&toPos, lpPlayerPos, addPos);
	localToPos = toPos;
	dxFor::Vec3Normalize(&toPos);
	//toPos.y = 0.f;

	//�A�X�g�[�J�[�s�׊J�n�͈͂ɓ���
	if (!isStalking and searchLen >= D3DXVec3Length(&localToPos))
	{
		isMoveTarget = true;
	}

	//�BPlayer�̈ʒu�߂����Ĉړ����J�n
	if (isMoveTarget)
	{
		float ksk = 1.f;
		if (10.f <= D3DXVec3Length(&localToPos))
		{
			ksk = 1.9f * ( D3DXVec3Length(&localToPos) - 9.f);
		}

		*addPos += toPos * 0.03f * ksk;

		if (1.f > D3DXVec3Length(&localToPos))
		{
			isStalking = true;
			isMoveTarget = false;
		}
	}

	//�C���܂�����
	if (isStalking and 1.f < D3DXVec3Length(&localToPos))
	{
		*addPos = *lpPlayerPos - toPos;
	}

}