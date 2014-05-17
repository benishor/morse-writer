#pragma once
#include <string>
#include <map>

class MorseDictionary {
public:

	explicit MorseDictionary();
	
	MorseDictionary& add(char character, const std::string morsePattern);

	const std::string& characterTemplate(char character) const;

	bool contains(char character) const;

	static MorseDictionary defaultDictionary();

private:
	std::map<char, const std::string> dictionary;
};