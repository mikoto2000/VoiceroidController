#pragma once

#include "Voiceroid.h"
#include "Yukari.h"
#include "YukariEx.h"

enum class VoiceroidType {
	YUKARI,
	YUKARI_EX
};

class VoiceroidFactory
{
public:
	static Voiceroid* create(VoiceroidType type);
};

