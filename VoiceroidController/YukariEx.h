#pragma once
#include <atlconv.h>
#include <windows.h>

#include "Voiceroid.h"

// 保存確認ダイアログ検索時に使用する構造体
struct SearchConfirmationOverwriteDialogParam {
	HWND parentHwnd;
	HWND targetHwnd;
};

// 「VOICEROID＋ 結月ゆかり EX」を表すクラス
class YukariEx :
	public Voiceroid
{
public:
	YukariEx();
	virtual ~YukariEx();

	// テキストを読み上げる
	void echo(std::string sentence, BOOL isSync);

	// 読み上げ音声をファイルに保存する
	void save(std::string sentence, std::string outFilePath, BOOL isSync);

private:
	// テキスト編集前のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME1;

	// テキスト編集後のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME2;

	// 音声ファイル保存ダイアログのダイアログ名
	static const _TCHAR* SAVE_DIALOG_NAME;

	// 音声保存中ダイアログのダイアログ名
	static const _TCHAR* SAVING_DIALOG_NAME;

	// 再生ボタン名
	static const _TCHAR* PLAY_BUTTON_NAME;

	// メインウィンドウのウィンドハンドル
	HWND yukari;

	// "VOICEROID＋ 結月ゆかり EX" ウィンドウを探す
	static BOOL CALLBACK SearchYukariEx(HWND hwnd, LPARAM lp);

	// "音声ファイルの保存" ダイアログを探す
	static BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp);

	// "音声保存" ダイアログを探す
	static BOOL CALLBACK SearchSavingDialog(HWND hwnd, LPARAM lp);

	// 上書き確認ダイアログを探す
	static BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp);

	// "テキストエリア" を探す
	static HWND CALLBACK SearchTextArea(HWND mainWindow);

	// "再生ボタン" を探す
	static HWND CALLBACK SearchPlayButton(HWND mainWindow);

	// "停止ボタン" を探す
	static HWND CALLBACK SearchStopButton(HWND mainWindow);

	// "保存ボタン" を探す
	static HWND CALLBACK SearchSaveButton(HWND mainWindow);

	// "ファイル名:" を探す
	static HWND CALLBACK SearchSaveFilePath(HWND saveDialog);

	// "保存(S)" を探す
	static HWND CALLBACK SearchSaveButtonInSaveDialog(HWND saveDialog);

	// 上書き確認ダイアログ内の「はい」ボタンを探す
	static HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog);
};
