#pragma once
#include <atlconv.h>
#include <windows.h>

#include "VoiceroidEx.h"

// 「VOICEROID＋ 結月ゆかり EX」を表すクラス
class YukariEx :
	public VoiceroidEx
{
public:
	YukariEx();
	virtual ~YukariEx();

private:
	// テキスト編集前のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME1;

	// テキスト編集後のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME2;

	// "VOICEROID＋ 結月ゆかり" ウィンドウを探す
	static BOOL CALLBACK SearchYukariEx(HWND hwnd, LPARAM lp);
};
