#include <atlconv.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <boost/program_options.hpp>

using namespace boost::program_options;

#define TARGET_WIN_NAME1 _T("VOICEROID�{ �����䂩�� EX")
#define TARGET_WIN_NAME2 _T("VOICEROID�{ �����䂩�� EX*")
#define SAVE_DIALOG_NAME _T("�����t�@�C���̕ۑ�")
#define SAVING_DIALOG_NAME _T("�����ۑ�")
#define PLAY_BUTTON_NAME _T(" �Đ�")

#define WAIT_TIME 1500
#define END_PLAY_CHECK_INTERVAL 1000

HWND yukari;
HWND saveDialog;
HWND savingDialog;
HWND confirmationOverwriteDialog;

// �e�L�X�g��ǂݏグ��
void echo(BOOL is_sync_mode);

// �ǂݏグ�������t�@�C���ɕۑ�����
void save(std::string output_file);

// "VOICEROID�{ �����䂩�� EX" �E�B���h�E��T��
BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp);

// "�����t�@�C���̕ۑ�" �_�C�A���O��T��
BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp);

// "�����ۑ�" �_�C�A���O��T��
BOOL CALLBACK SearchSavingDialog(HWND hwnd, LPARAM lp);

// �㏑���m�F�_�C�A���O
BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp);

// "�e�L�X�g�G���A" ��T��
HWND CALLBACK SearchTextArea(HWND mainWindow);

// "�Đ��{�^��" ��T��
HWND CALLBACK SearchPlayButton(HWND mainWindow);

// "��~�{�^��" ��T��
HWND CALLBACK SearchStopButton(HWND mainWindow);

// "�ۑ��{�^��" ��T��
HWND CALLBACK SearchSaveButton(HWND mainWindow);

// "�t�@�C����:" ��T��
HWND CALLBACK SearchSaveFilePath(HWND savaDialog);

// "�ۑ�(S)" ��T��
HWND CALLBACK SearchSaveButtonInSaveDialog(HWND savaDialog);

// �㏑���m�F�_�C�A���O���́u�͂��v�{�^����T��
HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog);

// �w�肵���n���h���� text �𑗐M����
void sendText(HWND hwnd, TCHAR* text);


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

	// "VOICEROID�{ �����䂩�� EX" ��T��
	EnumWindows(SearchYukari, 0x0);

	if (yukari == NULL) {
		fwprintf(stderr, _T("VOICEROID ��������܂���B\n"));
		fwprintf(stderr, _T("VOICEROID ���N�����Ă��������B\n"));

		exit(1);
	}

	HWND textArea = SearchTextArea(yukari);

	// �Ȃ�������Ńe�L�X�g�G���A��������...
	SendMessage(textArea, WM_SETTEXT, (WPARAM)_T(""), NULL);

	sendText(textArea, W2T((LPWSTR)echo_text.c_str()));

	// �ǂݏグ���邩�t�@�C���ɕۑ����邩����
	if (output_file.length() == 0) {
		// �ǂݏグ
		echo(is_sync_mode);
	} else {
		// �t�@�C���ɕۑ�
		save(output_file);
	}

	exit(0);
}

// �ǂݏグ�������t�@�C���ɕۑ�����
void save(std::string output_file) {
	// "�����ۑ��{�^��" ��T���ĉ���
	HWND save_button = SearchSaveButton(yukari);
	PostMessage(save_button, BM_CLICK, 0, 0);
	Sleep(WAIT_TIME);

	// "�����t�@�C���̕ۑ�" �E�B���h�E��T��
	EnumWindows(SearchSaveDialog, 0x0);

	// �t�@�C�����e�L�X�g�G���A��T��
	HWND saveFilePath = SearchSaveFilePath(saveDialog);

	// �t�@�C��������
	// �t�@�C������ wstring �ɕϊ�
	wchar_t *wcs = new wchar_t[output_file.length() + 1];
	mbstowcs(wcs, output_file.c_str(), output_file.length() + 1);

	// �t�@�C�������e�L�X�g�G���A�ɑ}��
	sendText(saveFilePath, W2T((LPWSTR)wcs));
	delete [] wcs;

	// "�ۑ�(S)" �{�^������
	HWND save_button_in_save_dialog = SearchSaveButtonInSaveDialog(saveDialog);
	PostMessage(save_button_in_save_dialog, BM_CLICK, 0, 0);

	Sleep(WAIT_TIME);

	// "VOICEROID�{ �����䂩�� EX" ��T��
	EnumWindows(SearchConfirmationOverwriteDialog, 0x0);
	if (confirmationOverwriteDialog != NULL) {
		HWND yes_button_in_confirmation_overwrite_dialog = SearchYesButtonInConfirmationOverwriteDialog(confirmationOverwriteDialog);
		PostMessage(yes_button_in_confirmation_overwrite_dialog, BM_CLICK, 0, 0);
	}

	// �����ۑ����_�C�A���O��������܂őҋ@
	// TODO: �^�C���A�E�g����邩��������
	while (1) {
		Sleep(END_PLAY_CHECK_INTERVAL);

		savingDialog = NULL;
		EnumWindows(SearchSavingDialog, 0x0);

		if (savingDialog == NULL) {
			break;
		}
	}
}

// �e�L�X�g��ǂݏグ��
void echo(BOOL is_sync_mode) {
	// "�Đ�" �{�^����T��
	HWND play_button = SearchPlayButton(yukari);

	if (is_sync_mode) {
		// �Đ��I���܂ő҂�
		// TODO: �^�C���A�E�g����邩��������
		SendMessage(play_button, BM_CLICK, 0, 0);

		while (1) {
			Sleep(END_PLAY_CHECK_INTERVAL);
			TCHAR strWindowText[1024];

			GetWindowText(play_button, strWindowText, 1024);

			if (_tcscmp(PLAY_BUTTON_NAME, strWindowText) == 0) {
				break;
			}
		}
	} else {
		// �Đ��I���܂ő҂��Ȃ�
		PostMessage(play_button, BM_CLICK, 0, 0);
	}
}

// "VOICEROID�{ �����䂩�� EX" �E�B���h�E��T��
BOOL CALLBACK SearchYukari(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		    || _tcscmp(TARGET_WIN_NAME2, strWindowText) == 0)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		yukari = hwnd;
		return false;
	}

	return true;
}

// "�����t�@�C���̕ۑ�" �_�C�A���O��T��
BOOL CALLBACK SearchSaveDialog(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		saveDialog = hwnd;
		return false;
	}

	return true;
}

// "�����ۑ�" �_�C�A���O��T��
BOOL CALLBACK SearchSavingDialog(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVING_DIALOG_NAME, strWindowText) == 0)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		savingDialog = hwnd;
		return false;
	}

	return true;
}

// �㏑���m�F�_�C�A���O��T��
BOOL CALLBACK SearchConfirmationOverwriteDialog(HWND hwnd, LPARAM lp) {
	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(SAVE_DIALOG_NAME, strWindowText) == 0
			&& saveDialog != hwnd)
	{
		_tprintf(_T("%s ��������܂����B\n"), strWindowText);
		confirmationOverwriteDialog = hwnd;
		return false;
	}

	return true;
}

// "�e�L�X�g�G���A" ��T��
HWND CALLBACK SearchTextArea(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

// "�Đ��{�^��" ��T��
HWND CALLBACK SearchPlayButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

// "��~�{�^��" ��T��
HWND CALLBACK SearchStopButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

// "�����ۑ��{�^��" ��T��
HWND CALLBACK SearchSaveButton(HWND mainWindow) {
	HWND tmp = GetWindow(mainWindow, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

// "�t�@�C����:" ��T��
HWND CALLBACK SearchSaveFilePath(HWND savaDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);
	tmp = GetWindow(tmp, GW_CHILD);

	return tmp;
}

// "�ۑ�(S)�{�^��" ��T��
HWND CALLBACK SearchSaveButtonInSaveDialog(HWND savaDialog) {
	HWND tmp = GetWindow(saveDialog, GW_CHILD);
	tmp = GetWindow(tmp, GW_HWNDNEXT);
	tmp = GetWindow(tmp, GW_HWNDNEXT);

	return tmp;
}

// �㏑���m�F�_�C�A���O���́u�͂��v�{�^����T��
HWND CALLBACK SearchYesButtonInConfirmationOverwriteDialog(HWND confirmationOverwriteDialog) {
	return GetWindow(confirmationOverwriteDialog, GW_CHILD);
}

// �w�肵���n���h���� text �𑗐M����
void sendText(HWND hwnd, TCHAR* text) {
	for (int i = 0; i < _tcslen(text); i++) {
		SendMessage(hwnd, WM_CHAR, (WPARAM)text[i], 0);
	}
}

