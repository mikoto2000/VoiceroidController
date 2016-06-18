#include "YukariEx.h"



YukariEx::YukariEx()
{
	// "VOICEROID＋ 結月ゆかり EX" を探す
	SearchHwndParam syep;
	EnumWindows(YukariEx::SearchYukariEx, (LPARAM)&syep);
	yukari = syep.hwnd;

	if (yukari == NULL) {
		throw _T("VOICEROID＋ 結月ゆかり EX が見つかりませんでした。");
	}
}

YukariEx::~YukariEx()
{
}

const _TCHAR* YukariEx::TARGET_WIN_NAME1 = _T("VOICEROID＋ 結月ゆかり EX");
const _TCHAR* YukariEx::TARGET_WIN_NAME2 = _T("VOICEROID＋ 結月ゆかり EX*");
const _TCHAR* YukariEx::SAVE_DIALOG_NAME = _T("音声ファイルの保存");
const _TCHAR* YukariEx::SAVING_DIALOG_NAME = _T("音声保存");
const _TCHAR* YukariEx::PLAY_BUTTON_NAME = _T(" 再生");

void YukariEx::echo(std::string sentence, BOOL isSync)
{
	HWND textArea = YukariEx::SearchTextArea(yukari);

	// なぜかこれでテキストエリアが消える...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "再生" ボタンを探す
	HWND play_button = SearchPlayButton(yukari);

	if (isSync) {
		// 再生終了まで待つ
		// TODO: タイムアウト入れるか検討する
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
		// 再生終了まで待たない
		PostMessage(play_button, BM_CLICK, 0, 0);
	}

}

void YukariEx::save(std::string sentence, std::string outFilePath, BOOL isSync)
{
	HWND textArea = YukariEx::SearchTextArea(yukari);

	// なぜかこれでテキストエリアが消える...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, sentence);

	// "音声保存ボタン" を探して押下
	HWND save_button = SearchSaveButton(yukari);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "音声ファイルの保存" ウィンドウを探す
	SearchHwndParam ssdp;
	EnumWindows(YukariEx::SearchSaveDialog, (LPARAM)&ssdp);
	HWND saveDialog = ssdp.hwnd;

	// ファイル名テキストエリアを探す
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// ファイル名をテキストエリアに挿入
	sendText(saveFilePath, outFilePath);

	// "保存(S)" ボタン押下
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "VOICEROID＋ 結月ゆかり EX" を探す
	SearchConfirmationOverwriteDialogParam scodp;
	scodp.parentHwnd = saveDialog;
	EnumWindows(SearchConfirmationOverwriteDialog, (LPARAM)&scodp);
	HWND confirmationOverwriteDialog = scodp.targetHwnd;

	if (confirmationOverwriteDialog != NULL) {
		HWND yes_button_in_confirmation_overwrite_dialog = SearchYesButtonInConfirmationOverwriteDialog(confirmationOverwriteDialog);
		PostMessage(yes_button_in_confirmation_overwrite_dialog, BM_CLICK, 0, 0);
	}

	// 音声保存中ダイアログが消えるまで待機
	// TODO: タイムアウト入れるか検討する
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
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
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
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
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

