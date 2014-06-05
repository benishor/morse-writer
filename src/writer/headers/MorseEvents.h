#pragma once
#include "MorseEvent.h"

class MorseEvents {
public:

    static MorseEvent startChar(char letter) {
        MorseEvent result;
        result.type = MorseEventType::StartChar;
        result.letter = letter;
        return result;
    }

    static MorseEvent endChar(char letter) {
        MorseEvent result;
        result.type = MorseEventType::EndChar;
        result.letter = letter;
        return result;
    }

    static MorseEvent element(MorseElement element) {
        MorseEvent result;
        result.type = MorseEventType::ElementToRender;
        result.element = element;
        return result;
    }

    static MorseEvent eof() {
        MorseEvent result;
        result.type = MorseEventType::Eof;
        return result;
    }
};
