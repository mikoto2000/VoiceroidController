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
const _TCHAR* Yukari::SAVING_DIALOG_NAME = _T("�����̕ۑ���");
const _TCHAR* Yukari::PLAY_BUTTON_NAME = _T(" �Đ�");
const _TCHAR* Yukari::TITLE_SAVE_CONFIRM = _T("���O��t���ĕۑ��̊m�F");

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
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_SELECT_ALL, NULL);
	SendMessage(yukari, WM_CHAR, _T(' '), NULL);

	// �N���b�v�{�[�h�ɓǂݏグ�����������͂�ݒ�
	Clipboard::setClipboardText(sentence);

	// �N���b�v�{�[�h�̓��e��\��t��
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_PASTE, 0);

	// �ۑ��{�^����T���ĉ���
	HWND saveButton = SearchSaveButton(yukari);
	PostMessage(saveButton, NULL, 0, 0);

	Sleep(WAIT_TIME);

	// "�����t�@�C���̕ۑ�" �E�B���h�E��T��
	SearchHwndParam ssdp;
	EnumWindows(Yukari::SearchSaveDialog, (LPARAM)&ssdp);
	HWND saveDialog = ssdp.hwnd;

	// �t�@�C�����e�L�X�g�G���A��T��
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// �t�@�C�������e�L�X�g�G���A�ɑ}��
	sendText(saveFilePath, outFilePath);

	// "�ۑ�(S)" �{�^������
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "�����t�@�C���̕ۑ�" �㏑���m�F�_�C�A���O��T��
	SearchHwndParam ssdp2;
	EnumWindows(Yukari::SearchConfirmationOverwriteDialog, (LPARAM)&ssdp2);
	HWND confirmationOverwriteDialog = ssdp2.hwnd;

	if (confirmationOverwriteDialog != NULL) {
		HWND yes_button_in_confirmation_overwrite_dialog = SearchYesButtonInConfirmationOverwriteDialog(confirmationOverwriteDialog);
		PostMessage(yes_button_in_confirmation_overwrite_dialog, BM_CLICK, 0, 0);
	}

	// �����ۑ����_�C�A���O��������܂őҋ@
	// TODO: �^�C���A�E�g����邩��������
	while (1) {
		Sleep(END_PLAY_CHECK_INTERVAL);

		SearchHwndParam ssdp;
		ssdp.hwnd = NULL;
		EnumWindows(SearchSavingDialog, (LPARAM)&ssdp);

		if (ssdp.hwnd == NULL) {
			break;
		}
	}
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

BOOL CALLBACK Yukari::SearchSaveDialog(HWND hwnd, LPARAM lp) {
	SearchHwndParam* ssdp = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0)
	{
		ssdp->hwnd = hwnd;
		return false;
	}

	ssdp->hwnd = NULL;
	return true;
}

BOOL CALLBACK Yukari::SearchSavingDialog(HWND hwnd, LPARAM lp) {
	SearchHwndParam* ssdp = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVING_DIALOG_NAME, strWindowText) == 0)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		ssdp->hwnd = hwnd;
		return false;
	}

	ssdp->hwnd = NULL;
	return true;
}

BOOL CALLBACK Yukari::SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp) {
	SearchHwndParam* ssdp = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TITLE_SAVE_CONFIRM, strWindowText) == 0)
	{
		ssdp->hwnd = hwnd;
		return false;
	}

	ssdp->hwnd = NULL;
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

HWND CALLBACK Yukari::SearchSaveButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK Yukari::SearchSaveFilePath(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

HWND CALLBACK Yukari::SearchSaveButtonInSaveDialog(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK Yukari::SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog) {
	HWND tmp = GetWindow(confirmationOverwriteDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

