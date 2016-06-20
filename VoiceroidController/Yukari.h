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

	// テキストを読み上げる
	void echo(std::string sentence, BOOL isSync);

	// 読み上げ音声をファイルに保存する
	void save(std::string sentence, std::string outFilePath, BOOL isSync);

private:
	// メインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME;

	// 音声ファイル保存ダイアログのダイアログ名
	static const _TCHAR* SAVE_DIALOG_NAME;

	// 音声保存中ダイアログのダイアログ名
	static const _TCHAR* SAVING_DIALOG_NAME;

	// 再生ボタン名
	static const _TCHAR* PLAY_BUTTON_NAME;

	// メインウィンドウのウィンドハンドル
	HWND yukari;

	// "VOICEROID＋ 結月ゆかり" ウィンドウを探す
	static BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp);

	// "テキストエリア" を探す
	static HWND CALLBACK SearchTextArea(HWND mainWindow);

	// "再生ボタン" を探す
	static HWND CALLBACK SearchPlayButton(HWND mainWindow);
};

