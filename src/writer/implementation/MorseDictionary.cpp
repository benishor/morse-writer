#include <MorseDictionary.h>
#include <iostream>

MorseDictionary::MorseDictionary() {
    // std::cout << "Creating morse dictionary instance" << std::endl;
}

MorseDictionary MorseDictionary::defaultDictionary() {
    MorseDictionary result;

    result
    .add('a', ".-").add('b', "-...").add('c', "-.-.").add('d', "-..")
    .add('e', ".").add('f', "..-.").add('g', "--.").add('h', "...")
    .add('i', "..").add('j', ".---").add('k', "-.-").add('l', ".-..")
    .add('m', "--").add('n', "-.").add('o', "---").add('p', ".--.")
    .add('q', "--.-").add('r', ".-.").add('s', "...").add('t', "-")
    .add('u', "..-").add('v', "...-").add('w', ".--").add('x', "-..-")
    .add('y', "-.--").add('z', "--..");

    result
    .add('0', "-----").add('1', ".----").add('2', "..---")
    .add('3', "...--").add('4', "....-").add('5', ".....")
    .add('6', "-....").add('7', "--...").add('8', "---..")
    .add('9', "----.");

    return result;
}

static std::string defaultTemplate = "";

MorseDictionary& MorseDictionary::add(char character, const std::string morsePattern) {
    dictionary.insert(std::pair<char, const std::string&>(std::toupper(character), morsePattern));
    dictionary.insert(std::pair<char, const std::string&>(std::tolower(character), morsePattern));
    return *this;
}

static std::string EMPTY_PATTERN;

bool MorseDictionary::contains(char character) const {
    auto it = dictionary.find(character);
    return it != dictionary.end();
}

const std::string& MorseDictionary::characterTemplate(char character) const {
    auto it = dictionary.find(character);
    if (it == dictionary.end()) {
        return EMPTY_PATTERN;
    } else {
        return it->second;
    }
}
