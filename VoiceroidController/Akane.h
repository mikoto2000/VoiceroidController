#pragma once
#include <atlconv.h>
#include <windows.h>

#include "VoiceroidEx.h"

// �uVOICEROID�{ �՗t���v��\���N���X
class Akane :
	public VoiceroidEx
{
public:
	Akane();
	virtual ~Akane();

private:
	// �e�L�X�g�ҏW�O�̃��C���E�B���h�E��
	static const _TCHAR* TARGET_WIN_NAME1;

	// �e�L�X�g�ҏW��̃��C���E�B���h�E��
	static const _TCHAR* TARGET_WIN_NAME2;

	// "VOICEROID�{ �՗t��" �E�B���h�E��T��
	static HWND SearchMainWindow();
	static BOOL CALLBACK SearchAkane(HWND hwnd, LPARAM lp);
};
