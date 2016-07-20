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

// �I�v�V������\���\����
struct Options {
	// �ǂݏグ VOICEROID(Yukari, YukariEx)
	std::string target_voiceroid_str;
	// �o�̓t�@�C���p�X
	std::string output_file;
	// ���̓t�@�C���p�X
	std::string input_file;
	// ���̓t�@�C�������R�[�h�� UTF8 �Ƃ��ď���
	bool is_utf8;
	// �������[�h(�Đ��E�ۑ�����������܂őҋ@���܂�)
	bool is_sync_mode;
	// �ǂݏグ������(�����Ƃ��ēn���ꂽ�ꍇ)
	std::wstring echo_text;
} typedef Options;

// �t�@�C�����e���擾����
std::string getContents(std::string filepath);

// wstring �� string �ɕϊ�����
std::string wstring2string(const std::wstring &src);

// UTF8 ������� SJIS ������ɂ��ĕԂ�
std::string utf8toSjis(std::string srcUTF8);

// �R�}���h���C���������
Options parseArgs(int argc, _TCHAR* argv[]);

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "");
	std::ios::sync_with_stdio(true);

	// �R�}���h���C���������
	Options options = parseArgs(argc, argv);

	// �w�肳�ꂽ�����ɉ�����
	// �R�}���h���C���������t�@�C������ǂݏグ��������擾����B
	std::string contents;
	if (options.input_file.compare("") == 0) {
		// �t�@�C���w�肪�Ȃ���΃R�}���h���C���������擾
		contents = wstring2string(options.echo_text);
	} else {
		// �t�@�C���w�肪����΃t�@�C�����e���擾
		contents = getContents(options.input_file);

		// �t�@�C�������R�[�h�ɉ����ĕ�����ϊ�
		if (options.is_utf8) {
			// utf8
			contents = utf8toSjis(contents);
		}
	}

	// �����ɉ����ĒN���g�p���邩�����肷��
	VoiceroidType voiceroidType;
	if (options.target_voiceroid_str.compare("Yukari") == 0) {
		voiceroidType = VoiceroidType::YUKARI;
	}
	else if (options.target_voiceroid_str.compare("YukariEx") == 0) {
		voiceroidType = VoiceroidType::YUKARI_EX;
	}

	// VOICEROID �쐬
	Voiceroid* voiceroid = VoiceroidFactory::create(voiceroidType);

	// �ǂݏグ���邩�t�@�C���ɕۑ����邩����
	if (options.output_file.length() == 0) {
		// �ǂݏグ
		voiceroid->echo(contents, options.is_sync_mode);
	} else {
		// �t�@�C���ɕۑ�
		voiceroid->save(contents, options.output_file, options.is_sync_mode);
	}

	delete voiceroid;

	exit(0);
}

Options parseArgs(int argc, _TCHAR* argv[]) {
	options_description opt("�I�v�V����");
	// �R�}���h���C��������`
	opt.add_options()
		("help,h", "�w���v��\��")
		("voiceroid", value<std::string>()->default_value("YukariEx"), "�ǂݏグ VOICEROID(Yukari, YukariEx)")
		("output-file,o", wvalue<std::wstring>(), "�o�̓t�@�C���p�X")
		("input-file,i", wvalue<std::wstring>(), "���̓t�@�C���p�X")
		("utf8,u", "���̓t�@�C�������R�[�h�� UTF8 �Ƃ��ď���")
		("sync,s", "�������[�h(�Đ��E�ۑ�����������܂őҋ@���܂�)");

	// �R�}���h���C���������
	variables_map argmap;
	auto pr = parse_command_line(argc, argv, opt);
	store(pr, argmap);
	notify(argmap);

	// ��͌��ʎ擾
	Options options;
	options.target_voiceroid_str = argmap["voiceroid"].as<std::string>();

	// �o�̓t�@�C���p�X�擾
	// (�g�p���Ă���o�[�W������ boost_program_options �ł́A
	//  std::wstring �ɑ΂��� default_value ���w�肷��ƃR���p�C���G���[�ɂȂ邽�߁A
	//  �I�v�V�����̗L���`�F�b�N�Ƒ�������O�Ŏ�������)
	std::wstring woutput_file;
	if (argmap.count("output-file")) {
		woutput_file = argmap["output-file"].as<std::wstring>();
	} else {
		woutput_file = L"";
	}

	// ���̓t�@�C���p�X�擾
	// (�g�p���Ă���o�[�W������ boost_program_options �ł́A
	//  std::wstring �ɑ΂��� default_value ���w�肷��ƃR���p�C���G���[�ɂȂ邽�߁A
	//  �I�v�V�����̗L���`�F�b�N�Ƒ�������O�Ŏ�������)
	std::wstring winput_file;
	if (argmap.count("input-file")) {
		winput_file = argmap["input-file"].as<std::wstring>();
	} else {
		winput_file = L"";
	}

	options.is_utf8 = argmap.count("utf8");
	options.is_sync_mode = !argmap["sync"].empty();
	options.echo_text = _T("");

	// wstring �Ŏ󂯎����������� string �ɕϊ�
	options.output_file = wstring2string(woutput_file);
	options.input_file = wstring2string(winput_file);

	// �I�v�V�����ȊO�̃R�}���h���C�������擾
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		options.echo_text.append(_T(" "));
		options.echo_text.append(str);
	}

	// �w���v�\���w�肪���邩�A
	// echo_text, input_file �������Ƃ���̏ꍇ�A�w���v�\�����ďI��
	if (argmap.count("help") ||
		(options.echo_text.compare(_T("")) == 0
			&& options.input_file.compare("") == 0)) {
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

	return options;
}

std::string wstring2string(const std::wstring &src) {
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);

	std::string dest = mbs;
	delete[] mbs;

	return dest;
}

std::string utf8toSjis(std::string srcUTF8) {
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

	//ShiftJIS�֕ϊ���̕����񒷂𓾂�
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	//�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
	char* bufShiftJis = new char[lengthSJis];

	//Unicode����ShiftJIS�֕ϊ�
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	delete bufUnicode;
	delete bufShiftJis;

	return strSJis;
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

