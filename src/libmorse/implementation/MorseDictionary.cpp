#include <MorseDictionary.h>
#include <iostream>
#include <algorithm>

MorseDictionary::MorseDictionary() {
    // std::cout << "Creating morse dictionary instance" << std::endl;
}

MorseDictionary MorseDictionary::defaultDictionary() {
    MorseDictionary result;

    result
    .add('a', ".-").add('b', "-...").add('c', "-.-.").add('d', "-..")
    .add('e', ".").add('f', "..-.").add('g', "--.").add('h', "....")
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

    result
    .add('.', ".-.-.-")
    .add(',', "--..--")
    .add('?', "..--..")
    .add('=', "-...-")
    .add('-', "-....-")
    .add('/', "-..-.")
    .add('"', ".----.")
    .add('(', "-.--.")
    .add(')', "-.--.-");

    return result;
}

MorseDictionary& MorseDictionary::add(char character, const std::string& morsePattern) {
    dictionary.insert(std::pair<char, const std::string&>(std::toupper(character), morsePattern));
    dictionary.insert(std::pair<char, const std::string&>(std::tolower(character), morsePattern));
    return *this;
}

const static std::string EMPTY_PATTERN {""};

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

std::string& MorseDictionary::filter(std::string& content) const {
    // Unfortunatelly gcc 4.8.2 does not yet have support for regex

    // Transform new lines into spaces so that we won't have
    // sentences on subsequent lines starting one after another
    for_each(content.begin(), content.end(), [](char& val) {
        val = val == '\n' ? ' ' : val;
    });

    // Remove invalid characters from the string
    content.erase(remove_if(content.begin(), content.end(), [this](char val) {
        return val != ' ' && !this->contains(val);
    }), content.end());

    return content;
}