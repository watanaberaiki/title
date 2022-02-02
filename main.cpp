#include "DxLib.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "LC1B_31_���^�i�x���C�L:�^�C�g��";

// �E�B���h�E����
const int WIN_WIDTH = 896;

// �E�B���h�E�c��
const int WIN_HEIGHT = 896;

float easeOutBounce(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int titlegh = LoadGraph("title2.png");
	int backgh = LoadGraph("back.png");
	int space[6];
	LoadDivGraph("space.png", 6, 6, 1, 896, 896, space);
	// �Q�[�����[�v�Ŏg���ϐ��̐錾

	//�^�C�g��
	int spaceMove = 0;
	int ismove = 1;
	float frame = 0.0f;
	float endframe = 250.0f;
	float start = -386.0f;
	float end = 100.0f;
	int titleX = 56;
	int titleY = 0;

	//�V�[��
	int scene = 0;
	//���ύX
	int changeMode = 0;
	int changeBack = 0;
	int changeCount = 0;
	int alpha = 200;
	int changeX = WIN_WIDTH / 2;
	int changeY = WIN_HEIGHT / 2;
	int changeR = 0;
	int changeCoolTime = 0;
	int isChange = 0;
	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = { 0 };

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = { 0 };

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		for (int i = 0; i < 256; i++) {
			oldkeys[i] = keys[i];
		}
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		switch (scene) {
		case 0:
			if (keys[KEY_INPUT_SPACE] == 1 && oldkeys[KEY_INPUT_SPACE] == 0) {
				if (changeCount == 0) {
					changeCount = 1;
				}
			}

			if (frame == endframe) {
				ismove = 0;
			}
			if (ismove == 1) {
				frame += 1;
			}
			if (spaceMove < 50) {
				spaceMove += 1;
			}
			if (spaceMove == 50) {
				spaceMove = 0;
			}
			//�C�[�W���O
			titleY = start + (end - start) * easeOutBounce(frame / endframe);

			//���񂾂�傫���Ȃ�
			if (changeCount == 1 && changeBack == 0) {
				changeR += 10;
			}
			
			//�߂�
			if (changeR >= 1000 && changeBack == 0) {
				//�����ŃV�[���؂�ւ�

				changeBack = 1;
				scene = 1;
				////////////////////
			}
			break;
		case 1:
			if (changeBack == 1) {
				changeR -= 10;
			}
			if (changeR <= 0 && changeBack == 1) {
			changeBack = 0;
			changeCount = 0;
			changeR = 0;
			isChange = 1;
		}

			break;
		}
		
		// �`�揈��
		switch (scene) {
		case 0:
			DrawGraph(0, 0, backgh, true);
			DrawGraph(titleX, titleY, titlegh, true);
			DrawGraph(0, 0, space[spaceMove / 10], true);
			if (changeCount == 1) {
				DrawCircle(changeX, changeY, changeR, GetColor(255, 255, 255), true);
			}
			break;
		case 1:
			DrawFormatString(500,500,GetColor(255,255,255),"�X�e�[�W�I��");
			if (changeCount == 1) {
				DrawCircle(changeX, changeY, changeR, GetColor(255, 255, 255), true);
			}
		}
		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}
