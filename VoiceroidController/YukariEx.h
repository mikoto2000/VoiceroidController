#pragma once
#include <atlconv.h>
#include <windows.h>

#include "VoiceroidEx.h"

// �uVOICEROID�{ �����䂩�� EX�v��\���N���X
class YukariEx :
	public VoiceroidEx
{
public:
	YukariEx();
	virtual ~YukariEx();

private:
	// �e�L�X�g�ҏW�O�̃��C���E�B���h�E��
	static const _TCHAR* TARGET_WIN_NAME1;

	// �e�L�X�g�ҏW��̃��C���E�B���h�E��
	static const _TCHAR* TARGET_WIN_NAME2;

	// "VOICEROID�{ �����䂩��" �E�B���h�E��T��
	static BOOL CALLBACK SearchYukariEx(HWND hwnd, LPARAM lp);
};
