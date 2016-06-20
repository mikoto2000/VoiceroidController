#pragma once

#include "Clipboard.h"
#include "Voiceroid.h"

#define COMMAND_SELECT_ALL 60
#define COMMAND_PASTE 56

class Yukari :
	public Voiceroid
{
public:
	Yukari();
	virtual ~Yukari();

	// �e�L�X�g��ǂݏグ��
	void echo(std::string sentence, BOOL isSync);

	// �ǂݏグ�������t�@�C���ɕۑ�����
	void save(std::string sentence, std::string outFilePath, BOOL isSync);

private:
	// ���C���E�B���h�E��
	static const _TCHAR* TARGET_WIN_NAME;

	// �����t�@�C���ۑ��_�C�A���O�̃_�C�A���O��
	static const _TCHAR* SAVE_DIALOG_NAME;

	// �����ۑ����_�C�A���O�̃_�C�A���O��
	static const _TCHAR* SAVING_DIALOG_NAME;

	// �Đ��{�^����
	static const _TCHAR* PLAY_BUTTON_NAME;

	// ���C���E�B���h�E�̃E�B���h�n���h��
	HWND yukari;

	// "VOICEROID�{ �����䂩��" �E�B���h�E��T��
	static BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp);

	// "�e�L�X�g�G���A" ��T��
	static HWND CALLBACK SearchTextArea(HWND mainWindow);

	// "�Đ��{�^��" ��T��
	static HWND CALLBACK SearchPlayButton(HWND mainWindow);
};

