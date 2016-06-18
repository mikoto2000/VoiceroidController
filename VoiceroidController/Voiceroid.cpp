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
	wchar_t *wcs = new wchar_t[text.length() + 1];
	mbstowcs(wcs, text.c_str(), text.length() + 1);

	for (int i = 0; i < wcslen(wcs); i++) {
		SendMessage(hwnd, WM_CHAR, (WPARAM)wcs[i], 0);
	}
	delete wcs;
}
