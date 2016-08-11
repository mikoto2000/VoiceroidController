#pragma once

#include "Voiceroid.h"
#include "Yukari.h"
#include "YukariEx.h"
#include "Aoi.h"

enum class VoiceroidType {
	YUKARI,
	YUKARI_EX,
	AOI
};

class VoiceroidFactory
{
public:
	static Voiceroid* create(VoiceroidType type);
};

