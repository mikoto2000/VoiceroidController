#include "Aoi.h"


Aoi::Aoi()
{
	// "VOICEROID�{ �՗t��" ��T��
	SearchHwndParam syep;
	EnumWindows(Aoi::SearchAoi, (LPARAM)&syep);
	mainWindow = syep.hwnd;

	if (mainWindow == NULL) {
		throw _T("VOICEROID�{ �՗t�� ��������܂���ł����B");
	}
}

Aoi::~Aoi()
{
}


const _TCHAR* Aoi::TARGET_WIN_NAME1 = _T("VOICEROID�{ �՗t��");
const _TCHAR* Aoi::TARGET_WIN_NAME2 = _T("VOICEROID�{ �՗t��*");


BOOL CALLBACK Aoi::SearchAoi(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(Aoi::TARGET_WIN_NAME2, strWindowText) == 0)
	{
		syep->hwnd = hwnd;
		return false;
	}

	syep->hwnd = NULL;
	return true;
}
