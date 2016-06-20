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

// �t�@�C�����e���擾����
std::string getContents(std::string filepath);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	std::ios::sync_with_stdio(true);

	options_description opt("�I�v�V����");
	// �R�}���h���C��������`
	opt.add_options()
		("help,h", "�w���v��\��")
		("output-file,o", value<std::string>()->default_value(""), "�o�̓t�@�C���p�X")
		("input-file,i", value<std::string>()->default_value(""), "���̓t�@�C���p�X")
		("sync,s", "�������[�h(�Đ��E�ۑ�����������܂őҋ@���܂�)");

	// �R�}���h���C���������
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// ��͌��ʎ擾
	std::string output_file = argmap["output-file"].as<std::string>();
	std::string input_file = argmap["input-file"].as<std::string>();
	bool is_sync_mode = !argmap["sync"].empty();
	std::wstring echo_text = _T("");

	// �I�v�V�����ȊO�̃R�}���h���C�������擾
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		echo_text.append(_T(" "));
		echo_text.append(str);
	}

	// �w���v�\���w�肪���邩�A
	// echo_text, input_file �������Ƃ���̏ꍇ�A�w���v�\�����ďI��
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

	// �w�肳�ꂽ�����ɉ�����
	// �R�}���h���C���������t�@�C������ǂݏグ��������擾����B
	std::string contents;
	if (input_file.compare("") == 0) {
		// �t�@�C���w�肪�Ȃ���΃R�}���h���C���������擾
		char *mbs = new char[echo_text.length() * MB_CUR_MAX + 1];
		wcstombs(mbs, echo_text.c_str(), echo_text.length() * MB_CUR_MAX + 1);
		contents = mbs;
		delete mbs;
	} else {
		// �t�@�C���w�肪����΃t�@�C�����e���擾
		contents = getContents(input_file);
	}

	Voiceroid* voiceroid = new YukariEx();

	// �ǂݏグ���邩�t�@�C���ɕۑ����邩����
	if (output_file.length() == 0) {
		// �ǂݏグ
		voiceroid->echo(contents, is_sync_mode);
	} else {
		// �t�@�C���ɕۑ�
		voiceroid->save(contents, output_file, is_sync_mode);
	}

	delete voiceroid;

	exit(0);
}

// �t�@�C�����e���擾����
std::string getContents(std::string filepath) {
	std::ifstream ifs (filepath);

	// �t�@�C���I�[�v�����۔���
	if (ifs.fail())
	{
		std::cerr << "�t�@�C���I�[�v���Ɏ��s���܂����B" << std::endl;
		return NULL;
	}

	// TODO: ���ꉽ����Ă��ł���[���ˁH
	std::istreambuf_iterator<char> it(ifs);
	std::istreambuf_iterator<char> last;
	std::string str (it, last);

	return str;
}

