#include "Akane.h"


Akane::Akane() : VoiceroidEx(SearchMainWindow)
{
}

Akane::~Akane()
{
}


const _TCHAR* Akane::TARGET_WIN_NAME1 = _T("VOICEROID＋ 琴葉茜");
const _TCHAR* Akane::TARGET_WIN_NAME2 = _T("VOICEROID＋ 琴葉茜*");

HWND Akane::SearchMainWindow() {
	// "VOICEROID＋ 琴葉茜" を探す
	SearchHwndParam syep;
	EnumWindows(Akane::SearchAkane, (LPARAM)&syep);

	if (syep.hwnd == NULL) {
		char* voiceroid_exe = std::getenv("VOICEROID_AKANE_EXE");
		if (!voiceroid_exe) {
			voiceroid_exe = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\Akane\\VOICEROID.exe";
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
			EnumWindows(Akane::SearchAkane, (LPARAM)&syep);
			if (syep.hwnd) {
				break;
			}
		}
	}
	
	return syep.hwnd;
}

BOOL CALLBACK Akane::SearchAkane(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(TARGET_WIN_NAME2, strWindowText) == 0)
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
