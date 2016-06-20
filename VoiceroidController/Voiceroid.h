#pragma once

#include <atlconv.h>
#include <string>
#include <tchar.h>
#include <windows.h>

// �e��E�B���h�E�������Ɏg�p����\����
struct SearchHwndParam {
	HWND hwnd;
};

// �e�� VOICEROID �̊��N���X
class Voiceroid
{
public:
	Voiceroid();
	virtual ~Voiceroid();

	// �e�L�X�g��ǂݏグ��
	virtual void echo(std::string sentence, BOOL isSync) = 0;

	// �ǂݏグ�������t�@�C���ɕۑ�����
	virtual void save(std::string sentence, std::string outFilePath, BOOL isSync) = 0;

protected:
	// �e��{�^������������̑҂�����
	static const int WAIT_TIME;

	// �Đ��E�ۑ��I���m�F�����̃C���^�[�o��
	static const int END_PLAY_CHECK_INTERVAL;

	// �w�肵���n���h���� text �𑗐M����
	void sendText(HWND hwnd, std::string text);
};

