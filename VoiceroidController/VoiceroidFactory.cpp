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
	}

	return voiceroid;
}
