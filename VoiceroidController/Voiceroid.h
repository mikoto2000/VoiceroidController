#pragma once

#include <atlconv.h>
#include <string>
#include <tchar.h>
#include <windows.h>

// 各種ウィンドウ検索時に使用する構造体
struct SearchHwndParam {
	HWND hwnd;
};

// 各種 VOICEROID の基底クラス
class Voiceroid
{
public:
	Voiceroid();
	virtual ~Voiceroid();

	// テキストを読み上げる
	virtual void echo(std::string sentence, BOOL isSync) = 0;

	// 読み上げ音声をファイルに保存する
	virtual void save(std::string sentence, std::string outFilePath, BOOL isSync) = 0;

protected:
	// 各種ボタンを押した後の待ち時間
	static const int WAIT_TIME;

	// 再生・保存終了確認処理のインターバル
	static const int END_PLAY_CHECK_INTERVAL;

	// 指定したハンドラに text を送信する
	void sendText(HWND hwnd, std::string text);
};

