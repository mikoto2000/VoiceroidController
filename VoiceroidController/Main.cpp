#include <atlconv.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <boost/program_options.hpp>

#include "Yukari.h"
#include "YukariEx.h"

using namespace boost::program_options;

// ファイル内容を取得する
std::string getContents(std::string filepath);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	std::ios::sync_with_stdio(true);

	options_description opt("オプション");
	// コマンドライン引数定義
	opt.add_options()
		("help,h", "ヘルプを表示")
		("output-file,o", value<std::string>()->default_value(""), "出力ファイルパス")
		("input-file,i", value<std::string>()->default_value(""), "入力ファイルパス")
		("sync,s", "同期モード(再生・保存が完了するまで待機します)");

	// コマンドライン引数解析
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// 解析結果取得
	std::string output_file = argmap["output-file"].as<std::string>();
	std::string input_file = argmap["input-file"].as<std::string>();
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
	}

	Voiceroid* voiceroid = new YukariEx();

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

