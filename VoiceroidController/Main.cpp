#include <atlconv.h>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

#include "VoiceroidFactory.h"
#include "Yukari.h"
#include "YukariEx.h"

#define INSERT_DELIMITER _T("。")
#define DELIMITERS _T(".。")

#define EMPTY_STR _T("")

using namespace boost::program_options;

// オプションを表す構造体
struct Options {
	// 読み上げ VOICEROID(Yukari, YukariEx)
	std::string target_voiceroid_str;
	// 出力ファイルパス
	std::wstring output_file;
	// 入力ファイルパス
	std::wstring input_file;
	// 入力ファイル文字コードを UTF8 として処理
	bool is_utf8;
	// 同期モード(再生・保存が完了するまで待機します)
	bool is_sync_mode;
	// 読み上げ文字列(引数として渡された場合)
	std::wstring echo_text;
	// 読み上げ文字列を分割する目安のサイズ
	size_t split_size;
} typedef Options;

// ファイル内容を取得する
std::wstring getContents(std::wstring filepath, bool is_utf8);

// 文字列がデリミタかどうか判定する
bool is_delimiter(std::wstring str);

// wstring を string に変換する
std::string wstring2string(const std::wstring &src);

// string を wstring に変換する
std::wstring string2wstring(const std::string &src);

// UTF8 文字列を SJIS 文字列にして返す
std::string utf8toSjis(std::string srcUTF8);

// コマンドライン引数解析
Options parseArgs(int argc, _TCHAR* argv[]);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	std::ios::sync_with_stdio(true);

	// コマンドライン引数解析
	Options options = parseArgs(argc, argv);

	// 指定された引数に応じて
	// コマンドライン引数かファイルから読み上げ文字列を取得する。
	std::wstring wcontents;
	if (options.input_file.compare(EMPTY_STR) == 0) {
		// ファイル指定がなければコマンドライン引数を取得
		wcontents = options.echo_text;
	} else {
		// ファイル指定があればファイル内容を取得
		wcontents = getContents(options.input_file, options.is_utf8);
	}

	// 引数に応じて誰を使用するかを決定する
	VoiceroidType voiceroidType;
	if (options.target_voiceroid_str.compare("Yukari") == 0) {
		voiceroidType = VoiceroidType::YUKARI;
	}
	else if (options.target_voiceroid_str.compare("YukariEx") == 0) {
		voiceroidType = VoiceroidType::YUKARI_EX;
	}

	// VOICEROID 作成
	Voiceroid* voiceroid = VoiceroidFactory::create(voiceroidType);


	// 指定文字数を超えないように分割しながら読み上げさせるための準備。

	std::list<std::wstring> list_string;
	boost::split(list_string, wcontents, boost::is_any_of(DELIMITERS));

	size_t size = 0;
	std::wstringstream ss;

	// 読み上げするかファイルに保存するか判定
	if (options.output_file.length() == 0) {

		BOOST_FOREACH(std::wstring s, list_string) {

			// 指定文字数を超える場合、
			// 今までため込んでいたものを読み上げる。
			// ただし、初回から超えていた場合はあきらめる。
			if (size != 0 && size + s.length() > options.split_size) {

				// 読み上げ
				voiceroid->echo(wstring2string(ss.str()), options.is_sync_mode);

				// ss リセット
				ss.str(EMPTY_STR);
				ss.clear();
				size = 0;
			}

			// サイズを増やして文字列結合
			size += s.length();
			ss << s << INSERT_DELIMITER;
		}

		// 最後の読み上げ
		voiceroid->echo(wstring2string(ss.str()), options.is_sync_mode);

	} else {

		size_t fileno = 0;

		BOOST_FOREACH(std::wstring s, list_string) {

			// 指定文字数を超える場合、
			// 今までため込んでいたものを読み上げる。
			// ただし、初回から超えていた場合はあきらめる。
			if (size != 0 && size + s.length() > options.split_size) {

				// ファイルに保存
		
				// ファイル名組み立て
				_TCHAR drive[_MAX_DRIVE];
				_TCHAR dir[_MAX_DIR];
				_TCHAR filename[_MAX_FNAME];
				_TCHAR ext[_MAX_EXT];
				_tsplitpath_s(options.output_file.c_str(), drive, dir, filename, ext);

				std::wstringstream dest;
				dest << drive << dir << filename << _T("_") << std::setfill(L'0') << std::setw(3) << std::right << fileno << ext;
				voiceroid->save(wstring2string(ss.str()), wstring2string(dest.str()), options.is_sync_mode);

				// ss リセット
				ss.str(EMPTY_STR);
				ss.clear();
				size = 0;

				fileno++;
			}

			// サイズを増やして文字列結合
			size += s.length();
			ss << s << INSERT_DELIMITER;
		}

		// 最後の保存
		
		// ファイル名組み立て
		_TCHAR drive[_MAX_DRIVE];
		_TCHAR dir[_MAX_DIR];
		_TCHAR filename[_MAX_FNAME];
		_TCHAR ext[_MAX_EXT];
		_tsplitpath_s(options.output_file.c_str(), drive, dir, filename, ext);

		std::wstringstream dest;
		dest << drive << dir << filename << _T("_") << std::setfill(L'0') << std::setw(3) << std::right << fileno << ext;
		voiceroid->save(wstring2string(ss.str()), wstring2string(dest.str()), options.is_sync_mode);

	}

	delete voiceroid;

	exit(0);
}

Options parseArgs(int argc, _TCHAR* argv[]) {
	options_description opt("オプション");
	// コマンドライン引数定義
	opt.add_options()
		("help,h", "ヘルプを表示")
		("voiceroid", value<std::string>()->default_value("YukariEx"), "読み上げ VOICEROID(Yukari, YukariEx)")
		("output-file,o", wvalue<std::wstring>(), "出力ファイルパス")
		("input-file,i", wvalue<std::wstring>(), "入力ファイルパス")
		("utf8,u", "入力ファイル文字コードを UTF8 として処理")
		("sync,s", "同期モード(再生・保存が完了するまで待機します)")
		("split-size", value<size_t>()->default_value(20000), "読み上げ文字列を分割する目安のサイズ");

	// コマンドライン引数解析
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// 解析結果取得
	Options options;
	options.target_voiceroid_str = argmap["voiceroid"].as<std::string>();

	// 出力ファイルパス取得
	// (使用しているバージョンの boost_program_options では、
	//  std::wstring に対して default_value を指定するとコンパイルエラーになるため、
	//  オプションの有無チェックと代入を自前で実装した)
	std::wstring woutput_file;
	if (argmap.count("output-file")) {
		options.output_file = argmap["output-file"].as<std::wstring>();
	} else {
		options.output_file = EMPTY_STR;
	}

	// 入力ファイルパス取得
	// (使用しているバージョンの boost_program_options では、
	//  std::wstring に対して default_value を指定するとコンパイルエラーになるため、
	//  オプションの有無チェックと代入を自前で実装した)
	std::wstring winput_file;
	if (argmap.count("input-file")) {
		options.input_file = argmap["input-file"].as<std::wstring>();
	} else {
		options.input_file = EMPTY_STR;
	}

	options.is_utf8 = argmap.count("utf8") > 0U;
	options.is_sync_mode = !argmap["sync"].empty();
	options.echo_text = EMPTY_STR;
	options.split_size = argmap["split-size"].as<size_t>();

	// オプション以外のコマンドライン引数取得
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		options.echo_text.append(_T(" "));
		options.echo_text.append(str);
	}

	// ヘルプ表示指定があるか、
	// echo_text, input_file が両方とも空の場合、ヘルプ表示して終了
	if (argmap.count("help") ||
		(options.echo_text.compare(EMPTY_STR) == 0
			&& options.input_file.compare(EMPTY_STR) == 0)) {
		_TCHAR drive[_MAX_DRIVE];
		_TCHAR dir[_MAX_DIR];
		_TCHAR filename[_MAX_FNAME];
		_TCHAR ext[_MAX_EXT];
		_tsplitpath_s(argv[0], drive, dir, filename, ext);

		wprintf(_T("Usage: %s%s [options] [text]\n"), filename, ext);

		std::stringstream helpStream;
		helpStream << opt << std::endl;
		printf(helpStream.str().c_str());

		exit(1);
	}

	return options;
}

std::string wstring2string(const std::wstring &src) {
	size_t num = 0;
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs_s(&num, mbs, src.length() * MB_CUR_MAX + 1, src.c_str(), src.length() * MB_CUR_MAX + 1);

	std::string dest = mbs;
	delete[] mbs;

	return dest;
}

std::wstring string2wstring(const std::string &src) {
	size_t num;
	wchar_t *wcs = new wchar_t[src.length() + 1];
	mbstowcs_s(&num, wcs, src.length() * MB_CUR_MAX + 1, src.c_str(), _TRUNCATE);
	std::wstring dest = wcs;
	delete[] wcs;

	return dest;
}

std::string utf8toSjis(std::string srcUTF8) {
	//Unicodeへ変換後の文字列長を得る
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, NULL, 0);

	//必要な分だけUnicode文字列のバッファを確保
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8からUnicodeへ変換
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, bufUnicode, lenghtUnicode);

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
std::wstring getContents(std::wstring filepath, bool is_utf8) {
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

	// ファイル文字コードに応じて文字列変換
	if (is_utf8) {
		// utf8
		str = utf8toSjis(str);
	}

	return string2wstring(str);
}

