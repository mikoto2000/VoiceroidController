#include "Akane.h"


Akane::Akane()
{
	// "VOICEROID＋ 琴葉茜" を探す
	SearchHwndParam syep;
	EnumWindows(Akane::SearchAkane, (LPARAM)&syep);
	mainWindow = syep.hwnd;

	if (mainWindow == NULL) {
		throw _T("VOICEROID＋ 琴葉茜 が見つかりませんでした。");
	}
}

Akane::~Akane()
{
}


const _TCHAR* Akane::TARGET_WIN_NAME1 = _T("VOICEROID＋ 琴葉茜");
const _TCHAR* Akane::TARGET_WIN_NAME2 = _T("VOICEROID＋ 琴葉茜*");


BOOL CALLBACK Akane::SearchAkane(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(TARGET_WIN_NAME2, strWindowText) == 0)
	{
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}
