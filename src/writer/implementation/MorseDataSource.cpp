#include <MorseDataSource.h>
#include <MorseEvents.h>
#include <iostream>

const char SPACE_CHARACTER = ' ';
const char DOT_CHARACTER = '.';

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


MorseEvent MorseDataSource::get() {
    MorseEvent result = MorseEvents::element(MorseElement::None);
    if (!eventQueue.empty()) {
        result = eventQueue.front();
        eventQueue.pop();
    }
    return result;
}

bool MorseDataSource::finished() const {
    return eventQueue.empty();
}

std::string MorseDataSource::getContent() {
    return content;
}

void MorseDataSource::crackIntoElements() {
    // Clear the queue in an efficient manner by using swap
    std::queue<MorseEvent> emptyQueue;
    std::swap(eventQueue, emptyQueue);

    bool inWord = false;
    for (auto c : content) {
        if (c == SPACE_CHARACTER) {
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
    eventQueue.push(MorseEvents::eof());
}

void MorseDataSource::addSpaceBetweenWords() {
    eventQueue.push(MorseEvents::startChar(SPACE_CHARACTER));
    eventQueue.push(MorseEvents::element(MorseElement::SpaceBetweenWords));
    eventQueue.push(MorseEvents::endChar(SPACE_CHARACTER));
}

void MorseDataSource::addSpaceBetweenCharacters() {
    eventQueue.push(MorseEvents::element(MorseElement::SpaceBetweenChars));
}

void MorseDataSource::addElementsForCharacter(char character) {
    eventQueue.push(MorseEvents::startChar(character));
    int i = 0;
    for (auto c : dictionary.characterTemplate(character)) {
        if (i++ > 0) {
            eventQueue.push(MorseEvents::element(MorseElement::SilentDot));
        }

        eventQueue.push(MorseEvents::element(c == DOT_CHARACTER ?
                                             MorseElement::Dot :
                                             MorseElement::Dash));
    }
    eventQueue.push(MorseEvents::endChar(character));
}