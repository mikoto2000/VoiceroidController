#pragma once

#include <string>
#include <tchar.h>
#include <windows.h>

class Clipboard
{
public:
	static BOOL setClipboardText(std::string text);
};

