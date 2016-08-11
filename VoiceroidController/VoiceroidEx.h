#pragma once
#include <atlconv.h>
#include <windows.h>
	
#include "Voiceroid.h"

// �uVOICEROID�{ EX�v��\���N���X
class VoiceroidEx :
	public Voiceroid
{
public:
	virtual ~VoiceroidEx();

	// �e�L�X�g��ǂݏグ��
	void echo(std::string sentence, BOOL isSync);

	// �ǂݏグ�������t�@�C���ɕۑ�����
	void save(std::string sentence, std::string outFilePath, BOOL isSync);

protected:
	// ���C���E�B���h�E�̃E�B���h�n���h��
	HWND mainWindow;

private:
	// �����t�@�C���ۑ��_�C�A���O�̃_�C�A���O��
	static const _TCHAR* SAVE_DIALOG_NAME;

	// �����ۑ����_�C�A���O�̃_�C�A���O��
	static const _TCHAR* SAVING_DIALOG_NAME;

	// �Đ��{�^����
	static const _TCHAR* PLAY_BUTTON_NAME;

	// "�����t�@�C���̕ۑ�" �_�C�A���O��T��
	static BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp);

	// "�����ۑ�" �_�C�A���O��T��
	static BOOL CALLBACK SearchSavingDialog(HWND hwnd, LPARAM lp);

	// �㏑���m�F�_�C�A���O��T��
	static BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp);

	// "�e�L�X�g�G���A" ��T��
	static HWND CALLBACK SearchTextArea(HWND mainWindow);

	// "�Đ��{�^��" ��T��
	static HWND CALLBACK SearchPlayButton(HWND mainWindow);

	// "��~�{�^��" ��T��
	static HWND CALLBACK SearchStopButton(HWND mainWindow);

	// "�ۑ��{�^��" ��T��
	static HWND CALLBACK SearchSaveButton(HWND mainWindow);

	// "�t�@�C����:" ��T��
	static HWND CALLBACK SearchSaveFilePath(HWND saveDialog);

	// "�ۑ�(S)" ��T��
	static HWND CALLBACK SearchSaveButtonInSaveDialog(HWND saveDialog);

	// �㏑���m�F�_�C�A���O���́u�͂��v�{�^����T��
	static HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog);
};
