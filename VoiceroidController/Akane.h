#pragma once
#include <atlconv.h>
#include <windows.h>

#include "VoiceroidEx.h"

// 「VOICEROID＋ 琴葉茜」を表すクラス
class Akane :
	public VoiceroidEx
{
public:
	Akane();
	virtual ~Akane();

private:
	// テキスト編集前のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME1;

	// テキスト編集後のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME2;

	// "VOICEROID＋ 琴葉茜" ウィンドウを探す
	static HWND SearchMainWindow();
	static BOOL CALLBACK SearchAkane(HWND hwnd, LPARAM lp);
};
