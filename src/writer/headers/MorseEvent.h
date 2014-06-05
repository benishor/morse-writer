#pragma once
#include "MorseElement.h"

enum class MorseEventType {
    ElementToRender,
    MetaData,
    StartChar,
    EndChar,
    Eof
};

struct MorseEvent {
    MorseEventType type = MorseEventType::ElementToRender;
    MorseElement element = MorseElement::None;
    char letter = '\0';
};

class MorseEventListener {
public:
	virtual void onMorseEvent(const MorseEvent& event) {};
};