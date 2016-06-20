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

	wchar_t *wcs = new wchar_t[text.length() + 1];
	mbstowcs(wcs, text.c_str(), text.length() + 1);

	// ヒープ上にメモリ領域を確保し、文字列をコピー
	int iStrLen = _tcslen(wcs) + 1;
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
