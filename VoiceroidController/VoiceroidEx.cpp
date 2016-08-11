#include "VoiceroidEx.h"

VoiceroidEx::~VoiceroidEx()
{
}

const _TCHAR* VoiceroidEx::SAVE_DIALOG_NAME = _T("�����t�@�C���̕ۑ�");
const _TCHAR* VoiceroidEx::SAVING_DIALOG_NAME = _T("�����ۑ�");
const _TCHAR* VoiceroidEx::PLAY_BUTTON_NAME = _T(" �Đ�");

void VoiceroidEx::echo(std::string sentence, BOOL isSync)
{
	HWND textArea = VoiceroidEx::SearchTextArea(mainWindow);

	// �Ȃ�������Ńe�L�X�g�G���A��������...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "�Đ�" �{�^����T��
	HWND play_button = SearchPlayButton(mainWindow);

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

void VoiceroidEx::save(std::string sentence, std::string outFilePath, BOOL isSync)
{
	HWND textArea = VoiceroidEx::SearchTextArea(mainWindow);

	// �Ȃ�������Ńe�L�X�g�G���A��������...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "�����ۑ��{�^��" ��T���ĉ���
	HWND save_button = SearchSaveButton(mainWindow);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "�����t�@�C���̕ۑ�" �E�B���h�E��T��
	SearchHwndParam ssdp;
	EnumWindows(VoiceroidEx::SearchSaveDialog, (LPARAM)&ssdp);
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

BOOL CALLBACK VoiceroidEx::SearchSaveDialog(HWND hwnd, LPARAM lp) {
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

BOOL CALLBACK VoiceroidEx::SearchSavingDialog(HWND hwnd, LPARAM lp) {
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

BOOL CALLBACK VoiceroidEx::SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp) {
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

HWND CALLBACK VoiceroidEx::SearchTextArea(HWND mainWindow)
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

HWND CALLBACK VoiceroidEx::SearchPlayButton(HWND mainWindow) {
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

HWND CALLBACK VoiceroidEx::SearchStopButton(HWND mainWindow) {
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

HWND CALLBACK VoiceroidEx::SearchSaveButton(HWND mainWindow) {
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

HWND CALLBACK VoiceroidEx::SearchSaveFilePath(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

HWND CALLBACK VoiceroidEx::SearchSaveButtonInSaveDialog(HWND saveDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

HWND CALLBACK VoiceroidEx::SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog) {
	return GetWindow(confirmationOverwriteDialog, GW_CHILD);
}

