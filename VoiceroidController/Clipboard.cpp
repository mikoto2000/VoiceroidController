#include "Clipboard.h"


BOOL Clipboard::setClipboardText(std::string text)
{
	// �����񂪋�̏ꍇ�͂Ȃɂ����Ȃ�
	if (text.length() == 0)
	{
		return FALSE;
	}

	// �N���b�v�{�[�h�̃I�[�v��
	if (!OpenClipboard(NULL))
	{
		return FALSE;
	}

	size_t num = 0;
	wchar_t *wcs = new wchar_t[text.length() + 1];
	mbstowcs_s(&num, wcs, text.length() * MB_CUR_MAX + 1, text.c_str(), _TRUNCATE);

	// �q�[�v��Ƀ������̈���m�ۂ��A��������R�s�[
	size_t iStrLen = _tcslen(wcs) + 1U;
	HGLOBAL hMem = GlobalAlloc(GMEM_SHARE, (iStrLen + 1) * sizeof(TCHAR));
	TCHAR* pMem = (TCHAR*)hMem;
	lstrcpy(pMem, wcs);

	// �N���b�v�{�[�h�֕�������R�s�[���A�N���[�Y
	EmptyClipboard();	// �N���b�v�{�[�h�̒��g����ɂ���
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();

	delete wcs;

	return TRUE;
}
