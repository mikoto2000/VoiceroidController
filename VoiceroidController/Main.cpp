#include <atlconv.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <boost/program_options.hpp>

using namespace boost::program_options;

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
	setlocale(LC_CTYPE, "");

	options_description opt("オプション");
	// コマンドライン引数定義
	opt.add_options()
		("help,h", "ヘルプを表示")
		("output-file,o", value<std::string>()->default_value(""), "出力ファイルパス")
		("input-file,i", value<std::string>()->default_value(""), "入力ファイルパス")
		("sync,s", "同期モード(再生・保存が完了するまで待機します)");

	// コマンドライン引数解析
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// 解析結果取得
	std::string output_file = argmap["output-file"].as<std::string>();
	std::string input_file = argmap["input-file"].as<std::string>();
	bool is_sync_mode = !argmap["sync"].empty();
	std::wstring echo_text = _T("");

	// オプション以外のコマンドライン引数取得
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		echo_text.append(_T(" "));
		echo_text.append(str);
	}

	// ヘルプ表示指定があるか、
	// echo_text, input_file が両方とも空の場合、ヘルプ表示して終了
	if (argmap.count("help") || 
		(echo_text.compare(_T("")) == 0
			&& input_file.compare("") == 0)) {
		_TCHAR drive[_MAX_DRIVE];
		_TCHAR dir[_MAX_DIR];
		_TCHAR filename[_MAX_FNAME];
		_TCHAR ext[_MAX_EXT];
		_tsplitpath(argv[0], drive, dir, filename, ext);

		std::wcout << "Usage: " << filename << ext << " [options] [TEXT]" << std::endl;
		std::cout << opt << std::endl;
		return 1;
	}

	// "VOICEROID＋ 結月ゆかり EX" を探す
	EnumWindows(SearchYukari, 0x0);

	HWND textArea = SearchTextArea(yukari);

	// なぜかこれでテキストエリアが消える...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);
	
	sendText(textArea, W2T((LPWSTR)echo_text.c_str()));

	// "音声保存ボタン" を探して押下
	HWND save_button = SearchSaveButton(yukari);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "音声ファイルの保存" ウィンドウを探す
	EnumWindows(SearchSaveDialog, 0x0);

	// ファイル名テキストエリアを探す
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// ファイル名入力
	// ファイル名を wstring に変換
	wchar_t *wcs = new wchar_t[output_file.length() + 1];
	mbstowcs(wcs, output_file.c_str(), output_file.length() + 1);

    // ファイル名をテキストエリアに挿入
	sendText(saveFilePath, W2T((LPWSTR)wcs));
	delete [] wcs;

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

