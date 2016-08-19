#include "Akane.h"


Akane::Akane() : VoiceroidEx(SearchMainWindow)
{
}

Akane::~Akane()
{
}


const _TCHAR* Akane::TARGET_WIN_NAME1 = _T("VOICEROID�{ �՗t��");
const _TCHAR* Akane::TARGET_WIN_NAME2 = _T("VOICEROID�{ �՗t��*");

HWND Akane::SearchMainWindow() {
	// "VOICEROID�{ �՗t��" ��T��
	SearchHwndParam syep;
	EnumWindows(Akane::SearchAkane, (LPARAM)&syep);

	if (syep.hwnd == NULL) {
		throw _T("VOICEROID�{ �՗t�� ��������܂���ł����B");
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
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}
