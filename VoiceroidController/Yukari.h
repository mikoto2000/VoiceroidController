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

	// 名前を付けて保存の確認ダイアログ名
	static const _TCHAR* TITLE_SAVE_CONFIRM;

	// メインウィンドウのウィンドハンドル
	HWND yukari;

	// "VOICEROID＋ 結月ゆかり" ウィンドウを探す
	static BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp);

	// "音声ファイルの保存" ダイアログを探す
	static BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp);

	// "音声保存" ダイアログを探す
	static BOOL CALLBACK SearchSavingDialog(HWND hwnd, LPARAM lp);

	// 名前を付けて保存の上書き確認ダイアログを探す
	static BOOL CALLBACK Yukari::SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp);

	// "テキストエリア" を探す
	static HWND CALLBACK SearchTextArea(HWND mainWindow);

	// "再生ボタン" を探す
	static HWND CALLBACK SearchPlayButton(HWND mainWindow);

	// "保存ボタン" を探す
	HWND CALLBACK Yukari::SearchSaveButton(HWND mainWindow);

	// "ファイル名:" を探す
	static HWND CALLBACK SearchSaveFilePath(HWND saveDialog);

	// "保存(S)" を探す
	static HWND CALLBACK SearchSaveButtonInSaveDialog(HWND saveDialog);

	// 上書き確認ダイアログ内の「はい」ボタンを探す
	static HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog);
};

