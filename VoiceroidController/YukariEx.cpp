#include "YukariEx.h"


YukariEx::YukariEx() : VoiceroidEx(SearchMainWindow)
{
}

YukariEx::~YukariEx()
{
}


const _TCHAR* YukariEx::TARGET_WIN_NAME1 = _T("VOICEROID＋ 結月ゆかり EX");
const _TCHAR* YukariEx::TARGET_WIN_NAME2 = _T("VOICEROID＋ 結月ゆかり EX*");


HWND YukariEx::SearchMainWindow() {
	// "VOICEROID＋ 結月ゆかり EX" を探す
	SearchHwndParam syep;
	EnumWindows(YukariEx::SearchYukariEx, (LPARAM)&syep);

	if (syep.hwnd == NULL) {
		char* voiceroid_exe = std::getenv("VOICEROID_YUKARI_EX_EXE");
		if (!voiceroid_exe) {
			voiceroid_exe = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\YukariEX\\VOICEROID.exe";
		}

		struct stat st;
		int ret = stat(voiceroid_exe, &st);
		if (ret != 0) {
			throw "VOICEROID 実行ファイルが見つかりませんでした。";
		}

		// voiceroid 起動コマンド組立
		std::stringstream command;
		command << "start cmd /c " << voiceroid_exe;
		system(command.str().c_str());
		
		// 起動待ち
		while (TRUE) {
			EnumWindows(YukariEx::SearchYukariEx, (LPARAM)&syep);
			if (syep.hwnd) {
				break;
			}
		}
	}

	return syep.hwnd;
}

BOOL CALLBACK YukariEx::SearchYukariEx(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(YukariEx::TARGET_WIN_NAME2, strWindowText) == 0)
	{
		/*
		 * メインウィンドウ判定
		 * 子要素にメニューがあればメインウィンドウが開いたと判断する。
		 */
		HWND tmp = GetWindow(hwnd, GW_CHILD);
		tmp = GetWindow(tmp, GW_HWNDNEXT);
		tmp = GetWindow(tmp, GW_HWNDNEXT);

		GetWindowText(tmp, strWindowText, 1024);

		if (_tcscmp(_T("menuStrip1"), strWindowText) == 0) {
			syep->hwnd = hwnd;
			return false;
		}
	}

	syep->hwnd = NULL;
	return true;
}

