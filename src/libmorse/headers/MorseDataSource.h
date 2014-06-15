#pragma once
#include <string>
#include <queue>
#include <MorseEvent.h>
#include <MorseDictionary.h>

class MorseDataSource {
public:
    MorseDataSource() = default;
    MorseDataSource(const std::string& text, const MorseDictionary& dictionary);
    MorseDataSource(const MorseDataSource&);

    MorseEvent get();
    bool finished() const;
    std::string getContent();

private:
    void crackIntoElements();
    void addSpaceBetweenWords();
    void addSpaceBetweenCharacters();
    void addElementsForCharacter(char character);

    std::string content;
    MorseDictionary dictionary;
    std::queue<MorseEvent> eventQueue;
};
