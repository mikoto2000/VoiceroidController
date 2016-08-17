#pragma once

#include "Voiceroid.h"
#include "Yukari.h"
#include "YukariEx.h"
#include "Aoi.h"
#include "Akane.h"

enum class VoiceroidType {
	YUKARI,
	YUKARI_EX,
	AOI,
	AKANE
};

class VoiceroidFactory
{
public:
	static Voiceroid* create(VoiceroidType type);
};

