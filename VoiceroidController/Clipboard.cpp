#include "Clipboard.h"


BOOL Clipboard::setClipboardText(std::string text)
{
	// 文字列が空の場合はなにもしない
	if (text.length() == 0)
	{
		return FALSE;
	}

	// クリップボードのオープン
	if (!OpenClipboard(NULL))
	{
		return FALSE;
	}

	size_t num = 0;
	wchar_t *wcs = new wchar_t[text.length() + 1];
	mbstowcs_s(&num, wcs, text.length() * MB_CUR_MAX + 1, text.c_str(), _TRUNCATE);

	// ヒープ上にメモリ領域を確保し、文字列をコピー
	size_t iStrLen = _tcslen(wcs) + 1U;
	HGLOBAL hMem = GlobalAlloc(GMEM_SHARE, (iStrLen + 1) * sizeof(TCHAR));
	TCHAR* pMem = (TCHAR*)hMem;
	lstrcpy(pMem, wcs);

	// クリップボードへ文字列をコピーし、クローズ
	EmptyClipboard();	// クリップボードの中身を空にする
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();

	delete wcs;

	return TRUE;
}
