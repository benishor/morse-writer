#include <MorseDataSource.h>
#include <iostream>

MorseDataSource::MorseDataSource(const std::string& text, const MorseDictionary& dict)
    : content(text),
      dictionary(dict) {
    dictionary.filter(content);
    crackIntoElements();
}


MorseDataSource::MorseDataSource(const MorseDataSource& other) 
    : content(other.content), 
      dictionary(other.dictionary) {
    crackIntoElements();
}


MorseElement MorseDataSource::get() {
    MorseElement result = MorseElement::None;
    if (!elementQueue.empty()) {
        result = elementQueue.front();
        elementQueue.pop();
    }
    return result;
}

bool MorseDataSource::finished() const {
    return elementQueue.empty();
}

void MorseDataSource::crackIntoElements() {
    // Clear the queue in an efficient manner by using swap
    std::queue<MorseElement> emptyQueue;
    std::swap(elementQueue, emptyQueue);

    bool inWord = false;
    for (auto c : content) {
        if (c == ' ') {
            addSpaceBetweenWords();
            inWord = false;
        } else {
            if (inWord) {
                addSpaceBetweenCharacters();
            }

            addElementsForCharacter(c);
            inWord = true;
        }
    }
}

void MorseDataSource::addSpaceBetweenWords() {
    elementQueue.push(MorseElement::SpaceBetweenWords);
}

void MorseDataSource::addSpaceBetweenCharacters() {
    elementQueue.push(MorseElement::SpaceBetweenChars);
}

void MorseDataSource::addElementsForCharacter(char character) {
    int i = 0;
    for (auto c : dictionary.characterTemplate(character)) {
        if (i++ > 0) {
            elementQueue.push(MorseElement::SilentDot);
        }

        elementQueue.push(c == '.' ?
                          MorseElement::Dot :
                          MorseElement::Dash);
    }
}