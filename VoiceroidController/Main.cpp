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

#define INSERT_DELIMITER _T("�B")
#define DELIMITERS _T(".�B")

#define EMPTY_STR _T("")

using namespace boost::program_options;

// �I�v�V������\���\����
struct Options {
	// �ǂݏグ VOICEROID(Yukari, YukariEx)
	std::string target_voiceroid_str;
	// �o�̓t�@�C���p�X
	std::wstring output_file;
	// ���̓t�@�C���p�X
	std::wstring input_file;
	// ���̓t�@�C�������R�[�h�� UTF8 �Ƃ��ď���
	bool is_utf8;
	// �������[�h(�Đ��E�ۑ�����������܂őҋ@���܂�)
	bool is_sync_mode;
	// �ǂݏグ������(�����Ƃ��ēn���ꂽ�ꍇ)
	std::wstring echo_text;
	// �ǂݏグ������𕪊�����ڈ��̃T�C�Y
	size_t split_size;
} typedef Options;

// �t�@�C�����e���擾����
std::wstring getContents(std::wstring filepath, bool is_utf8);

// �����񂪃f���~�^���ǂ������肷��
bool is_delimiter(std::wstring str);

// wstring �� string �ɕϊ�����
std::string wstring2string(const std::wstring &src);

// string �� wstring �ɕϊ�����
std::wstring string2wstring(const std::string &src);

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
	std::wstring wcontents;
	if (options.input_file.compare(EMPTY_STR) == 0) {
		// �t�@�C���w�肪�Ȃ���΃R�}���h���C���������擾
		wcontents = options.echo_text;
	} else {
		// �t�@�C���w�肪����΃t�@�C�����e���擾
		wcontents = getContents(options.input_file, options.is_utf8);
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


	// �w�蕶�����𒴂��Ȃ��悤�ɕ������Ȃ���ǂݏグ�����邽�߂̏����B

	std::list<std::wstring> list_string;
	boost::split(list_string, wcontents, boost::is_any_of(DELIMITERS));

	size_t size = 0;
	std::wstringstream ss;

	// �ǂݏグ���邩�t�@�C���ɕۑ����邩����
	if (options.output_file.length() == 0) {

		BOOST_FOREACH(std::wstring s, list_string) {

			// �w�蕶�����𒴂���ꍇ�A
			// ���܂ł��ߍ���ł������̂�ǂݏグ��B
			// �������A���񂩂璴���Ă����ꍇ�͂�����߂�B
			if (size != 0 && size + s.length() > options.split_size) {

				// �ǂݏグ
				voiceroid->echo(wstring2string(ss.str()), options.is_sync_mode);

				// ss ���Z�b�g
				ss.str(EMPTY_STR);
				ss.clear();
				size = 0;
			}

			// �T�C�Y�𑝂₵�ĕ����񌋍�
			size += s.length();
			ss << s << INSERT_DELIMITER;
		}

		// �Ō�̓ǂݏグ
		voiceroid->echo(wstring2string(ss.str()), options.is_sync_mode);

	} else {

		size_t fileno = 0;

		BOOST_FOREACH(std::wstring s, list_string) {

			// �w�蕶�����𒴂���ꍇ�A
			// ���܂ł��ߍ���ł������̂�ǂݏグ��B
			// �������A���񂩂璴���Ă����ꍇ�͂�����߂�B
			if (size != 0 && size + s.length() > options.split_size) {

				// �t�@�C���ɕۑ�
		
				// �t�@�C�����g�ݗ���
				_TCHAR drive[_MAX_DRIVE];
				_TCHAR dir[_MAX_DIR];
				_TCHAR filename[_MAX_FNAME];
				_TCHAR ext[_MAX_EXT];
				_tsplitpath_s(options.output_file.c_str(), drive, dir, filename, ext);

				std::wstringstream dest;
				dest << drive << dir << filename << _T("_") << std::setfill(L'0') << std::setw(3) << std::right << fileno << ext;
				voiceroid->save(wstring2string(ss.str()), wstring2string(dest.str()), options.is_sync_mode);

				// ss ���Z�b�g
				ss.str(EMPTY_STR);
				ss.clear();
				size = 0;

				fileno++;
			}

			// �T�C�Y�𑝂₵�ĕ����񌋍�
			size += s.length();
			ss << s << INSERT_DELIMITER;
		}

		// �Ō�̕ۑ�
		
		// �t�@�C�����g�ݗ���
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
	options_description opt("�I�v�V����");
	// �R�}���h���C��������`
	opt.add_options()
		("help,h", "�w���v��\��")
		("voiceroid", value<std::string>()->default_value("YukariEx"), "�ǂݏグ VOICEROID(Yukari, YukariEx)")
		("output-file,o", wvalue<std::wstring>(), "�o�̓t�@�C���p�X")
		("input-file,i", wvalue<std::wstring>(), "���̓t�@�C���p�X")
		("utf8,u", "���̓t�@�C�������R�[�h�� UTF8 �Ƃ��ď���")
		("sync,s", "�������[�h(�Đ��E�ۑ�����������܂őҋ@���܂�)")
		("split-size", value<size_t>()->default_value(20000), "�ǂݏグ������𕪊�����ڈ��̃T�C�Y");

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
		options.output_file = argmap["output-file"].as<std::wstring>();
	} else {
		options.output_file = EMPTY_STR;
	}

	// ���̓t�@�C���p�X�擾
	// (�g�p���Ă���o�[�W������ boost_program_options �ł́A
	//  std::wstring �ɑ΂��� default_value ���w�肷��ƃR���p�C���G���[�ɂȂ邽�߁A
	//  �I�v�V�����̗L���`�F�b�N�Ƒ�������O�Ŏ�������)
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

	// �I�v�V�����ȊO�̃R�}���h���C�������擾
	for (auto const& str : collect_unrecognized(pr.options, include_positional)) {
		options.echo_text.append(_T(" "));
		options.echo_text.append(str);
	}

	// �w���v�\���w�肪���邩�A
	// echo_text, input_file �������Ƃ���̏ꍇ�A�w���v�\�����ďI��
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
	//Unicode�֕ϊ���̕����񒷂𓾂�
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, NULL, 0);

	//�K�v�ȕ�����Unicode������̃o�b�t�@���m��
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

	//UTF8����Unicode�֕ϊ�
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), (int)(srcUTF8.size()) + 1, bufUnicode, lenghtUnicode);

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
std::wstring getContents(std::wstring filepath, bool is_utf8) {
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

	// �t�@�C�������R�[�h�ɉ����ĕ�����ϊ�
	if (is_utf8) {
		// utf8
		str = utf8toSjis(str);
	}

	return string2wstring(str);
}

