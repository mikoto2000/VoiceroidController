#include "Voiceroid.h"



Voiceroid::Voiceroid()
{
}


Voiceroid::~Voiceroid()
{
}

const int Voiceroid::WAIT_TIME = 1500;
const int Voiceroid::END_PLAY_CHECK_INTERVAL = 1000;

void Voiceroid::sendText(HWND hwnd, std::string text)
{
	size_t num = 0;
	wchar_t *wcs = new wchar_t[text.length() + 1];
	errno_t err = mbstowcs_s(&num, wcs, text.length() + 1, text.c_str(), _TRUNCATE);

	for (int i = 0; i < wcslen(wcs); i++) {
		SendMessage(hwnd, WM_CHAR, (WPARAM)wcs[i], 0);
	}
	delete wcs;
}
