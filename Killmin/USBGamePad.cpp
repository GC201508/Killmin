#include "USBGamePad.h"

namespace Input {//������C���X�^���X������Ӗ������܂�Ȃ��̂ƁA�O���[�o���֐��͌�����̂�Input���O��ԂŊ���܂��B

	// Input::joypadKeyDown�֐��ł́A�w�肵��ID�̃W���C�p�b�h�̎w�肵���{�^����������Ă����true��Ԃ��֐��ł��BjoyGetDevCaps�֐��ŁA�W���C�p�b�h�̐��\�𒲂ׁAJOYCAPS�\���̂�dwSize�����o�ɃT�C�Y��dwFlags�����o�Ƀ{�^�����͂𓾂�t���O�𗧂Ă���joyGetPosEx���g���Č��݂̓��͏�Ԃ𒲂ׂ܂��B
	// �Ԃ�l��JOYERR_NOERROR�ȊO��������G���[�Ȃ̂�false��Ԃ��Ƃ��܂��B
	// �{�^�����͂́AJOYINFOEX�\���̂�dwButtons�����o�ɂ͊e�L�[�̏�Ԃ������Ă��܂��B
	// �{�^����������Ă���΂��̃{�^���ԍ��̃r�b�g�������Ă���̂ŁA�{�^���ԍ���&�������ʂŒ��ׂ܂��B
	// �\���L�[�̕��͌X������c�����A���������ꂼ��dwXpos�AdwYpos�����o�ɓ����Ă��܂��B
	// �Ȃ񂩕ςȌv�Z�����Ă���̂́A�V�т̕�������͂ƂƂ�Ȃ��悤�ɂ���H�v�I�ȉ����ł��B�@���A�i���O�X�e�B�b�N�͂Ȃ��̂ō폜

	//�\���L�[�@dwXpos,dwYpos �����l�i32511�j 
	//������@dwYpos = 0;
	//�������@dwYpos = 65535;
	//�������@dwXPos = 0;
	//�E�����@dwXpos = 65535;

	bool joypadKeyDown(JoypadCode joypadcode, JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNBUTTONS;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			if (joypadcode <= PAD_INPUT_10) {
				return (joypad_info.dwButtons & joypadcode) != 0;
			}
			else if (joypadcode == PAD_INPUT_UP) {
				return (joypad_info.dwYpos == 0);
			}
			else if (joypadcode == PAD_INPUT_DOWN) {
				return (joypad_info.dwYpos == 65535);
			}
			else if (joypadcode == PAD_INPUT_LEFT) {
				return (joypad_info.dwXpos == 0);
			}
			else if (joypadcode == PAD_INPUT_RIGHT) {
				return (joypad_info.dwXpos == 65535);
			}
			else {
				return false;
			}
		}
		return false;
	}

	// joypadAnalogX�֐���joypadAnalogY�֐��͂��ꂼ��A�i���O�X�e�B�b�N��X���AY���̌X�����Ԃ��܂��B
	// JOYCAPS�\���̂�dwFlags�����o�ɂ́A�A�i���O�X�e�B�b�N�̎��̌X�����
	//�擾����t���OJOY_RETURNX�AJOY_RETURNY�����Ă���joyGetPosEx���g���ČX����𒲂ׂ܂��B
	// �ςȌv�Z�����Ă���̂͂܂���ɂ���ėV�ѕ������X���Ƃ��Ď��Ȃ��悤�ɂ���H�v�I�ȉ����ł��B
	// �����ȌX���͖������Ă����������������K�ɑ���ł��܂��B

	float joypadAnalogX(JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNX;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			const float center((static_cast<float>(joypad_caps.wXmin + joypad_caps.wXmax)) / 2.f);
			const float diff(static_cast<float>(joypad_caps.wXmax - joypad_caps.wXmin));
			const float val((static_cast<float>(joypad_info.dwXpos) - center) * 2.f / diff);
			return fabsf(val) < 0.1f ? 0.f : val;
		}
		return false;
	}

	float joypadAnalogY(JoypadID id) {
		JOYINFOEX joypad_info;
		JOYCAPS joypad_caps;
		joyGetDevCaps(id, &joypad_caps, sizeof(joypad_caps));
		joypad_info.dwSize = sizeof(joypad_info);
		joypad_info.dwFlags = JOY_RETURNY;
		if (joyGetPosEx(id, &joypad_info) == JOYERR_NOERROR) {
			const float center((static_cast<float>(joypad_caps.wYmin + joypad_caps.wYmax)) / 2.f);
			const float diff(static_cast<float>(joypad_caps.wYmax - joypad_caps.wYmin));
			const float val((static_cast<float>(joypad_info.dwYpos) - center) * 2.f / diff);
			return fabsf(val) < 0.1f ? 0.f : val;
		}
		return false;
	}
}