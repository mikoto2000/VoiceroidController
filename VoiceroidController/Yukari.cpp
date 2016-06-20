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
const _TCHAR* Yukari::SAVING_DIALOG_NAME = _T("音声保存");
const _TCHAR* Yukari::PLAY_BUTTON_NAME = _T(" 再生");

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
