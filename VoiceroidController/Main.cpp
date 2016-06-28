#include <atlconv.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <boost/program_options.hpp>

#include "VoiceroidFactory.h"
#include "Yukari.h"
#include "YukariEx.h"

using namespace boost::program_options;

// ファイル内容を取得する
std::string getContents(std::string filepath);

// UTF8 文字列を SJIS 文字列にして返す
std::string utf8toSjis(std::string srcUTF8);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	std::ios::sync_with_stdio(true);

	options_description opt("オプション");
	// コマンドライン引数定義
	opt.add_options()
		("help,h", "ヘルプを表示")
		("voiceroid", value<std::string>()->default_value("YukariEx"), "読み上げ VOICEROID(Yukari, YukariEx)")
		("output-file,o", value<std::string>()->default_value(""), "出力ファイルパス")
		("input-file,i", value<std::string>()->default_value(""), "入力ファイルパス")
		("utf8,u", "入力ファイル文字コードを UTF8 として処理")
		("sync,s", "同期モード(再生・保存が完了するまで待機します)");

	// コマンドライン引数解析
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// 解析結果取得
	std::string target_voiceroid_str = argmap["voiceroid"].as<std::string>();
	std::string output_file = argmap["output-file"].as<std::string>();
	std::string input_file = argmap["input-file"].as<std::string>();
	bool is_utf8 = argmap.count("utf8");
	bool is_sync_mode = !argmap["sync"].empty();
	std::wstring echo_text = _T("");

	// オプション以外のコマンドライン引数取得
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		echo_text.append(_T(" "));
		echo_text.append(str);
	}

	// ヘルプ表示指定があるか、
	// echo_text, input_file が両方とも空の場合、ヘルプ表示して終了
	if (argmap.count("help") ||
		(echo_text.compare(_T("")) == 0
			&& input_file.compare("") == 0)) {
		_TCHAR drive[_MAX_DRIVE];
		_TCHAR dir[_MAX_DIR];
		_TCHAR filename[_MAX_FNAME];
		_TCHAR ext[_MAX_EXT];
		_tsplitpath(argv[0], drive, dir, filename, ext);

		wprintf(L"Usage: %s%s [options] [text]\n", filename, ext);

		std::stringstream helpStream;
		helpStream << opt << std::endl;
		printf(helpStream.str().c_str());

		exit(1);
	}

	// 指定された引数に応じて
	// コマンドライン引数かファイルから読み上げ文字列を取得する。
	std::string contents;
	if (input_file.compare("") == 0) {
		// ファイル指定がなければコマンドライン引数を取得
		char *mbs = new char[echo_text.length() * MB_CUR_MAX + 1];
		wcstombs(mbs, echo_text.c_str(), echo_text.length() * MB_CUR_MAX + 1);
		contents = mbs;
		delete mbs;
	} else {
		// ファイル指定があればファイル内容を取得
		contents = getContents(input_file);

		// ファイル文字コードに応じて文字列変換
		if (is_utf8) {
			// utf8
			contents = utf8toSjis(contents);
		}
	}

	// 引数に応じて誰を使用するかを決定する
	VoiceroidType voiceroidType;
	if (target_voiceroid_str.compare("Yukari") == 0) {
		voiceroidType = VoiceroidType::YUKARI;
	}
	else if (target_voiceroid_str.compare("YukariEx") == 0) {
		voiceroidType = VoiceroidType::YUKARI_EX;
	}

	// VOICEROID 作成
	Voiceroid* voiceroid = VoiceroidFactory::create(voiceroidType);

	// 読み上げするかファイルに保存するか判定
	if (output_file.length() == 0) {
		// 読み上げ
		voiceroid->echo(contents, is_sync_mode);
	} else {
		// ファイルに保存
		voiceroid->save(contents, output_file, is_sync_mode);
	}

	delete voiceroid;

	exit(0);
}

std::string utf8toSjis(std::string srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJISへ変換後の文字列長を得る
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//必要な分だけShiftJIS文字列のバッファを確保
	char* bufShiftJis = new char[lengthSJis];

	//UnicodeからShiftJISへ変換
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
}

// ファイル内容を取得する
std::string getContents(std::string filepath) {
	std::ifstream ifs (filepath);

	// ファイルオープン成否判定
	if (ifs.fail())
	{
		std::cerr << "ファイルオープンに失敗しました。" << std::endl;
		return NULL;
	}

	// TODO: これ何やってるんでしょーかね？
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str (it, last);

	return str;
}

