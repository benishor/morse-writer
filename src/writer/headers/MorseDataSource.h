#pragma once
#include <string>
#include <queue>
#include <MorseElement.h>
#include <MorseDictionary.h>

class MorseDataSource {
public:
    explicit MorseDataSource(const std::string& text, const MorseDictionary& dictionary);

    MorseElement get();

    bool finished() const;

private:
    void crackIntoElements();

    void addSpaceBetweenWords();
    void addSpaceBetweenCharacters();
    void addElementsForCharacter(char character);


    const std::string& content;
    const MorseDictionary& dictionary;

    std::queue<MorseElement> elementQueue;
};
