#include "Yukari.h"



Yukari::Yukari()
{
	// "VOICEROID＋ 結月ゆかり" を探す
	SearchHwndParam syep;
	EnumWindows(Yukari::SearchYukari, (LPARAM)&syep);
	yukari = syep.hwnd;

	if (yukari == NULL) {
		throw _T("VOICEROID＋ 結月ゆかり が見つかりませんでした。");
	}
}


Yukari::~Yukari()
{
}

const _TCHAR* Yukari::TARGET_WIN_NAME = _T("VOICEROID＋ 結月ゆかり");
const _TCHAR* Yukari::SAVE_DIALOG_NAME = _T("音声ファイルの保存");
const _TCHAR* Yukari::SAVING_DIALOG_NAME = _T("音声の保存中");
const _TCHAR* Yukari::PLAY_BUTTON_NAME = _T(" 再生");
const _TCHAR* Yukari::TITLE_SAVE_CONFIRM = _T("名前を付けて保存の確認");

void Yukari::echo(std::string sentence, BOOL isSync)
{
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_SELECT_ALL, NULL);
	SendMessage(yukari, WM_CHAR, _T(' '), NULL);

	// クリップボードに読み上げさせたい文章を設定
	Clipboard::setClipboardText(sentence);

	// クリップボードの内容を貼り付け
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_PASTE, 0);


	// "再生" ボタンを探す
	HWND play_button = SearchPlayButton(yukari);

	if (isSync) {
		// 再生終了まで待つ
		SendMessage(play_button, NULL, 0, 0);
	}
	else {
		// 再生終了まで待たない
		PostMessage(play_button, NULL, 0, 0);
	}

}

void Yukari::save(std::string sentence, std::string outFilePath, BOOL isSync)
{
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_SELECT_ALL, NULL);
	SendMessage(yukari, WM_CHAR, _T(' '), NULL);

	// クリップボードに読み上げさせたい文章を設定
	Clipboard::setClipboardText(sentence);

	// クリップボードの内容を貼り付け
	SendMessage(yukari, WM_ACTIVATE, WA_ACTIVE, NULL);
	SendMessage(yukari, WM_COMMAND, COMMAND_PASTE, 0);

	// 保存ボタンを探して押す
	HWND saveButton = SearchSaveButton(yukari);
	PostMessage(saveButton, NULL, 0, 0);

	Sleep(WAIT_TIME);

	// "音声ファイルの保存" ウィンドウを探す
	SearchHwndParam ssdp;
	EnumWindows(Yukari::SearchSaveDialog, (LPARAM)&ssdp);
	HWND saveDialog = ssdp.hwnd;

	// ファイル名テキストエリアを探す
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// ファイル名をテキストエリアに挿入
	sendText(saveFilePath, outFilePath);

	// "保存(S)" ボタン押下
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "音声ファイルの保存" 上書き確認ダイアログを探す
	SearchHwndParam ssdp2;
	EnumWindows(Yukari::SearchConfirmationOverwriteDialog, (LPARAM)&ssdp2);
	HWND confirmationOverwriteDialog = ssdp2.hwnd;

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
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
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

