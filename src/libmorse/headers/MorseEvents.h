#pragma once
#include "MorseEvent.h"

class MorseEvents {
public:

    static MorseEvent newContent(const std::string& content) {
        MorseEvent result;
        result.type = MorseEventType::NewContent;
        result.content = content;
        return result;
    }

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
