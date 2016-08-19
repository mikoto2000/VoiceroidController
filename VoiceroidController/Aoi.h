#pragma once
#include <atlconv.h>
#include <windows.h>

#include "VoiceroidEx.h"

// 「VOICEROID＋ 琴葉葵」を表すクラス
class Aoi :
	public VoiceroidEx
{
public:
	Aoi();
	virtual ~Aoi();

private:
	// テキスト編集前のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME1;

	// テキスト編集後のメインウィンドウ名
	static const _TCHAR* TARGET_WIN_NAME2;

	// "VOICEROID＋ 琴葉葵" ウィンドウを探す
	static HWND SearchMainWindow();
	static BOOL CALLBACK SearchAoi(HWND hwnd, LPARAM lp);
};
