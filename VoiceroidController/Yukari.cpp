#include "Yukari.h"



Yukari::Yukari()
{
	// "VOICEROID�{ �����䂩��" ��T��
	SearchHwndParam syep;
	EnumWindows(Yukari::SearchYukari, (LPARAM)&syep);
	yukari = syep.hwnd;

	if (yukari == NULL) {
		throw _T("VOICEROID�{ �����䂩�� ��������܂���ł����B");
	}
}


Yukari::~Yukari()
{
}

const _TCHAR* Yukari::TARGET_WIN_NAME = _T("VOICEROID�{ �����䂩��");
const _TCHAR* Yukari::SAVE_DIALOG_NAME = _T("�����t�@�C���̕ۑ�");
const _TCHAR* Yukari::SAVING_DIALOG_NAME = _T("�����ۑ�");
const _TCHAR* Yukari::PLAY_BUTTON_NAME = _T(" �Đ�");

void Yukari::echo(std::string sentence, BOOL isSync)
{
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_SELECT_ALL, NULL);
	SendMessage(yukari, WM_CHAR, _T(' '), NULL);

	// �N���b�v�{�[�h�ɓǂݏグ�����������͂�ݒ�
	Clipboard::setClipboardText(sentence);

	// �N���b�v�{�[�h�̓��e��\��t��
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_PASTE, 0);


	// "�Đ�" �{�^����T��
	HWND play_button = SearchPlayButton(yukari);

	if (isSync) {
		// �Đ��I���܂ő҂�
		SendMessage(play_button, NULL, 0, 0);
	}
	else {
		// �Đ��I���܂ő҂��Ȃ�
		PostMessage(play_button, NULL, 0, 0);
	}

}

void Yukari::save(std::string sentence, std::string outFilePath, BOOL isSync)
{

}

BOOL CALLBACK Yukari::SearchYukari(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME, strWindowText) == 0)
	{
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}

HWND CALLBACK Yukari::SearchTextArea(HWND mainWindow)
{
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK Yukari::SearchPlayButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}
