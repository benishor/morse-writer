#pragma once
#include <string>
#include <map>


class MorseDictionary {
public:
    /**
     * @brief Default constructor
     */
    explicit MorseDictionary();

    /**
     * @brief Static factory method which returns the default ITU dictionary.
     * @details A Morse dictionary provides the means to map between ASCII characters
     *          and their corresponding morse character pattern. A morse character pattern
     *          is a string comprised of dots and dashes, e.g: ".--"
     *
     * @return the default ITU compatible dictionary
     */
    static MorseDictionary defaultDictionary();

    /**
     * @brief Adds a character mapping to the current dictionary
     * @details [long description]
     *
     * @param character ASCII character, the key in the dictionary
     * @param morsePattern A morse pattern containing dash and dots the provided character, e.g: ".--"
     *
     * @return self instance, useful for chaining
     */
    MorseDictionary& add(char character, const std::string& morsePattern);

    /**
     * @brief Retrieves the pattern corresponding to the provided character (case insensitive).
     *
     * @param character the ASCII character whose mapping is to be retrieved
     * @return the morse pattern corresponding to the provided character, or empty string
     *         if such mapping does not exist in the dictionary
     */
    const std::string& characterTemplate(char character) const;

    /**
     * @brief Determines whether a mapping exists for the provided ASCII character or not.
     * @details [long description]
     *
     * @param character [description]
     * @return [description]
     */
    bool contains(char character) const;

    /**
     * @brief Filters out unknown characters in the provided string.
     * @details Modifies the provided content by removing characters the dictionary
     *          does not have a translation for. Spaces are kept and new lines are
     *          converted to spaces. The content is being modified and also returned.
     *          This method is const because it does not modify the state of the dictionary.
     *
     * @param content the content to be filtered. This will be modified.
     * @return the filtered out content, free from unknown characters
     */
    std::string& filter(std::string& content) const;

private:
    std::map<char, std::string> dictionary;
};
