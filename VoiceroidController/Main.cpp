#include <stdio.h>
#include <windows.h>
#include <tchar.h>

#define TARGET_WIN_NAME1 _T("VOICEROID＋ 結月ゆかり EX")
#define TARGET_WIN_NAME2 _T("VOICEROID＋ 結月ゆかり EX*")
#define SAVE_DIALOG_NAME _T("音声ファイルの保存")
#define WAIT_TIME 1500

HWND yukari;
HWND saveDialog;
HWND confirmationOverwriteDialog;

// "VOICEROID＋ 結月ゆかり EX" ウィンドウを探す
BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp);

// "音声ファイルの保存" ダイアログを探す
BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp);

// 上書き確認ダイアログ
BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp);

// "テキストエリア" を探す
HWND CALLBACK SearchTextArea(HWND mainWindow);

// "再生ボタン" を探す
HWND CALLBACK SearchPlayButton(HWND mainWindow);

// "停止ボタン" を探す
HWND CALLBACK SearchStopButton(HWND mainWindow);

// "保存ボタン" を探す
HWND CALLBACK SearchSaveButton(HWND mainWindow);

// "ファイル名:" を探す
HWND CALLBACK SearchSaveFilePath(HWND savaDialog);

// "保存(S)" を探す
HWND CALLBACK SearchSaveButtonInSaveDialog(HWND savaDialog);

// 上書き確認ダイアログ内の「はい」ボタンを探す
HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog);

// 指定したハンドラに text を送信する
void sendText(HWND hwnd, TCHAR* text);


int _tmain(int argc, _TCHAR* argv[])
{
	// "VOICEROID＋ 結月ゆかり EX" を探す
	EnumWindows(SearchYukari, 0x0);

	HWND textArea = SearchTextArea(yukari);

	// なぜかこれでテキストエリアが消える...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);
	
	sendText(textArea, _T("只今マイクのテスト中。"));

	// "音声保存ボタン" を探して押下
	HWND save_button = SearchSaveButton(yukari);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "音声ファイルの保存" ウィンドウを探す
	EnumWindows(SearchSaveDialog, 0x0);

	// ファイル名テキストエリアを探す
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// ファイル名入力
	sendText(saveFilePath, _T("C:\\Users\\mikoto\\Music\\TEST.wav"));

	// "保存(S)" ボタン押下
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "VOICEROID＋ 結月ゆかり EX" を探す
	EnumWindows(SearchConfirmationOverwriteDialog, 0x0);
	if (confirmationOverwriteDialog != NULL) {
		HWND yes_button_in_confirmation_overwrite_dialog = SearchYesButtonInConfirmationOverwriteDialog(confirmationOverwriteDialog);
		PostMessage(yes_button_in_confirmation_overwrite_dialog, BM_CLICK, 0, 0);
	}

	exit(0);
}

// "VOICEROID＋ 結月ゆかり EX" ウィンドウを探す
BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		    || _tcscmp(TARGET_WIN_NAME2, strWindowText) == 0)
	{
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
		yukari = hwnd;
		return false;
	}

	return true;
}

// "音声ファイルの保存" ダイアログを探す
BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0)
	{
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
		saveDialog = hwnd;
		return false;
	}

	return true;
}

// 上書き確認ダイアログを探す
BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0
			&& saveDialog != hwnd)
	{
		_tprintf(_T("%s が見つかりました。\n"), strWindowText);
		confirmationOverwriteDialog = hwnd;
		return false;
	}

	return true;
}

// "テキストエリア" を探す
HWND CALLBACK SearchTextArea(HWND mainWindow) {
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

// "再生ボタン" を探す
HWND CALLBACK SearchPlayButton(HWND mainWindow) {
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

// "停止ボタン" を探す
HWND CALLBACK SearchStopButton(HWND mainWindow) {
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

// "音声保存ボタン" を探す
HWND CALLBACK SearchSaveButton(HWND mainWindow) {
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

// "ファイル名:" を探す
HWND CALLBACK SearchSaveFilePath(HWND savaDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

// "保存(S)ボタン" を探す
HWND CALLBACK SearchSaveButtonInSaveDialog(HWND savaDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

// 上書き確認ダイアログ内の「はい」ボタンを探す
HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog) {
	return GetWindow(confirmationOverwriteDialog, GW_CHILD);
}

// 指定したハンドラに text を送信する
void sendText(HWND hwnd, TCHAR* text) {
	for (int i = 0; i < _tcslen(text); i++) {
		SendMessage(hwnd, WM_CHAR, (WPARAM)text[i], 0);
	}
}

