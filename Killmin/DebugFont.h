/*
	��ʏ�ɕ�����\������i�f�o�b�O�p�j

	�� ������ȍ�Ƃ��Ă��ĂƂĂ��x���i�`��X�s�[�h���Q�[�������łȂ��j
	�� �����܂ŊJ���i�K�̃f�o�b�O�p�Ƃ��ė��߂܂��傤
*/
#include "System.h"
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }

class DebugFont
{
	LPD3DXFONT	pFont;	// �t�H���g�I�u�W�F�N�g
public:
	void Init(LPDIRECT3DDEVICE9 pd3d);
	void Render(double d);
};

class CStopwatch {
public:
	/*!
	*@brief	�R���X�g���N�^�B
	*/
	CStopwatch()
	{
		freq         = 0;
		end          = 0;
		begin        = 0;
		elapsed      = 0.0;
		elapsedMill  = 0.0;
		elapsedMicro = 0.0;
	}
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~CStopwatch()
	{
	}
	/*!
	*@brief	�v���J�n�B
	*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
	*@brief	�v���I��
	*/
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed      = double(end - begin) / freq;
		elapsedMill  = elapsedMicro * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�b)
	*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�~���b)
	*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�}�C�N���b)
	*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)

};