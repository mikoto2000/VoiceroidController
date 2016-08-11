#include "YukariEx.h"


YukariEx::YukariEx()
{
	// "VOICEROID�{ �����䂩�� EX" ��T��
	SearchHwndParam syep;
	EnumWindows(YukariEx::SearchYukariEx, (LPARAM)&syep);
	mainWindow = syep.hwnd;

	if (mainWindow == NULL) {
		throw _T("VOICEROID�{ �����䂩�� EX ��������܂���ł����B");
	}
}

YukariEx::~YukariEx()
{
}


const _TCHAR* YukariEx::TARGET_WIN_NAME1 = _T("VOICEROID�{ �����䂩�� EX");
const _TCHAR* YukariEx::TARGET_WIN_NAME2 = _T("VOICEROID�{ �����䂩�� EX*");


BOOL CALLBACK YukariEx::SearchYukariEx(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(YukariEx::TARGET_WIN_NAME2, strWindowText) == 0)
	{
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}

