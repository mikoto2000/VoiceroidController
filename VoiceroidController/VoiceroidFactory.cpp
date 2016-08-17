#include "VoiceroidFactory.h"


Voiceroid* VoiceroidFactory::create(VoiceroidType type)
{
	Voiceroid* voiceroid = NULL;

	switch (type) {
	case(VoiceroidType::YUKARI) :
		voiceroid = new Yukari();
		break;
	case(VoiceroidType::YUKARI_EX) :
		voiceroid = new YukariEx();
		break;
	case(VoiceroidType::AOI) :
		voiceroid = new Aoi();
		break;
	case(VoiceroidType::AKANE) :
		voiceroid = new Akane();
		break;
	}

	return voiceroid;
}
