#pragma once
#include "MorseElement.h"
#include <string>

enum class MorseEventType {
    ElementToRender,
    NewContent,
    MetaData,
    StartChar,
    EndChar,
    Eof
};

struct MorseEvent {
    MorseEventType type = MorseEventType::ElementToRender;
    MorseElement element = MorseElement::None;
    char letter = '\0';
    std::string content;
};

class MorseEventListener {
public:
	virtual void onMorseEvent(const MorseEvent& event) {};
};