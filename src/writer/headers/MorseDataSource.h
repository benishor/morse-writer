#pragma once
#include <string>
#include <queue>
#include <MorseElement.h>
#include <MorseDictionary.h>

class MorseDataSource {
public:
    MorseDataSource() = default;
    MorseDataSource(const std::string& text, const MorseDictionary& dictionary);
    MorseDataSource(const MorseDataSource&);

    MorseElement get();
    bool finished() const;

private:
    void crackIntoElements();
    void addSpaceBetweenWords();
    void addSpaceBetweenCharacters();
    void addElementsForCharacter(char character);

    std::string content;
    MorseDictionary dictionary;
    std::queue<MorseElement> elementQueue;
};
