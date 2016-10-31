#include "Akane.h"


Akane::Akane() : VoiceroidEx(SearchMainWindow)
{
}

Akane::~Akane()
{
}


const _TCHAR* Akane::TARGET_WIN_NAME1 = _T("VOICEROID�{ �՗t��");
const _TCHAR* Akane::TARGET_WIN_NAME2 = _T("VOICEROID�{ �՗t��*");

HWND Akane::SearchMainWindow() {
	// "VOICEROID�{ �՗t��" ��T��
	SearchHwndParam syep;
	EnumWindows(Akane::SearchAkane, (LPARAM)&syep);

	if (syep.hwnd == NULL) {
		char* voiceroid_exe = std::getenv("VOICEROID_AKANE_EXE");
		if (!voiceroid_exe) {
			voiceroid_exe = "C:\\Program Files (x86)\\AHS\\VOICEROID+\\Akane\\VOICEROID.exe";
		}

		struct stat st;
		int ret = stat(voiceroid_exe, &st);
		if (ret != 0) {
			throw "VOICEROID ���s�t�@�C����������܂���ł����B";
		}

		// voiceroid �N���R�}���h�g��
		std::stringstream command;
		command << "start cmd /c " << voiceroid_exe;
		system(command.str().c_str());

		// �N���҂�
		while (TRUE) {
			EnumWindows(Akane::SearchAkane, (LPARAM)&syep);
			if (syep.hwnd) {
				break;
			}
		}
	}
	
	return syep.hwnd;
}

BOOL CALLBACK Akane::SearchAkane(HWND hwnd, LPARAM lp) {
	SearchHwndParam* syep = (SearchHwndParam*)lp;

	TCHAR strWindowText[1024];
	GetWindowText(hwnd, strWindowText, 1024);

	if (_tcscmp(TARGET_WIN_NAME1, strWindowText) == 0
		|| _tcscmp(TARGET_WIN_NAME2, strWindowText) == 0)
	{
		/*
		* ���C���E�B���h�E����
		* �q�v�f�Ƀ��j���[������΃��C���E�B���h�E���J�����Ɣ��f����B
		*/
		HWND tmp = GetWindow(hwnd, GW_CHILD);
		tmp = GetWindow(tmp, GW_HWNDNEXT);
		tmp = GetWindow(tmp, GW_HWNDNEXT);

		GetWindowText(tmp, strWindowText, 1024);

		if (_tcscmp(_T("menuStrip1"), strWindowText) == 0) {
			syep->hwnd = hwnd;
			return false;
		}
	}

	syep->hwnd = NULL;
	return true;
}
