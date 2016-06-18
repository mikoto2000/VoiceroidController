#include "YukariEx.h"



YukariEx::YukariEx()
{
	// "VOICEROID�{ �����䂩�� EX" ��T��
	SearchHwndParam syep;
	EnumWindows(YukariEx::SearchYukariEx, (LPARAM)&syep);
	yukari = syep.hwnd;

	if (yukari == NULL) {
		throw _T("VOICEROID�{ �����䂩�� EX ��������܂���ł����B");
	}
}

YukariEx::~YukariEx()
{
}

const _TCHAR* YukariEx::TARGET_WIN_NAME1 = _T("VOICEROID�{ �����䂩�� EX");
const _TCHAR* YukariEx::TARGET_WIN_NAME2 = _T("VOICEROID�{ �����䂩�� EX*");
const _TCHAR* YukariEx::SAVE_DIALOG_NAME = _T("�����t�@�C���̕ۑ�");
const _TCHAR* YukariEx::SAVING_DIALOG_NAME = _T("�����ۑ�");
const _TCHAR* YukariEx::PLAY_BUTTON_NAME = _T(" �Đ�");

void YukariEx::echo(std::string sentence, BOOL isSync)
{
	HWND textArea = YukariEx::SearchTextArea(yukari);

	// �Ȃ�������Ńe�L�X�g�G���A��������...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "�Đ�" �{�^����T��
	HWND play_button = SearchPlayButton(yukari);

	if (isSync) {
		// �Đ��I���܂ő҂�
		// TODO: �^�C���A�E�g����邩��������
		SendMessage(play_button, BM_CLICK, 0, 0);

		while (1) {
			Sleep(END_PLAY_CHECK_INTERVAL);
			TCHAR strWindowText[1024];

			GetWindowText(play_button, strWindowText, 1024);

			if (_tcscmp(PLAY_BUTTON_NAME, strWindowText) == 0) {
				break;
			}
		}
	}
	else {
		// �Đ��I���܂ő҂��Ȃ�
		PostMessage(play_button, BM_CLICK, 0, 0);
	}

}

void YukariEx::save(std::string sentence, std::string outFilePath, BOOL isSync)
{
	HWND textArea = YukariEx::SearchTextArea(yukari);

	// �Ȃ�������Ńe�L�X�g�G���A��������...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "�����ۑ��{�^��" ��T���ĉ���
	HWND save_button = SearchSaveButton(yukari);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "�����t�@�C���̕ۑ�" �E�B���h�E��T��
	SearchHwndParam ssdp;
	EnumWindows(YukariEx::SearchSaveDialog, (LPARAM)&ssdp);
	HWND saveDialog = ssdp.hwnd;

	// �t�@�C�����e�L�X�g�G���A��T��
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// �t�@�C�������e�L�X�g�G���A�ɑ}��
	sendText(saveFilePath, outFilePath);

	// "�ۑ�(S)" �{�^������
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "VOICEROID�{ �����䂩�� EX" ��T��
	SearchConfirmationOverwriteDialogParam scodp;
	scodp.parentHwnd = saveDialog;
	EnumWindows(SearchConfirmationOverwriteDialog, (LPARAM)&scodp);
	HWND confirmationOverwriteDialog = scodp.targetHwnd;

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

BOOL CALLBACK YukariEx::SearchYukariEx(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(YukariEx::TARGET_WIN_NAME2, strWindowText) == 0)
	{
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}

BOOL CALLBACK YukariEx::SearchSaveDialog(HWND hwnd, LPARAM lp) {
	SearchHwndParam* ssdp = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		ssdp->hwnd = hwnd;
		return false;
	}

	ssdp->hwnd = NULL;
	return true;
}

BOOL CALLBACK YukariEx::SearchSavingDialog(HWND hwnd, LPARAM lp) {
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

BOOL CALLBACK YukariEx::SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp) {
	SearchConfirmationOverwriteDialogParam* ssdp = (SearchConfirmationOverwriteDialogParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0
		&& ssdp->parentHwnd != hwnd)
	{
		ssdp->targetHwnd = hwnd;
		return false;
	}

	ssdp->targetHwnd = NULL;
	return true;
}

HWND CALLBACK YukariEx::SearchTextArea(HWND mainWindow)
{
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

HWND CALLBACK YukariEx::SearchPlayButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

HWND CALLBACK YukariEx::SearchStopButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK YukariEx::SearchSaveButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK YukariEx::SearchSaveFilePath(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

HWND CALLBACK YukariEx::SearchSaveButtonInSaveDialog(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK YukariEx::SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog) {
	return GetWindow(confirmationOverwriteDialog, GW_CHILD);
}

