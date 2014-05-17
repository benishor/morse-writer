#include <MorseWriter.h>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string getFileContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw MorseWriterException("Failed to open file " + filename);
    }

    string result;

    file.seekg(0, std::ios::end);
    result.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    result.assign(std::istreambuf_iterator<char>(file),
                  std::istreambuf_iterator<char>());

    file.close();
    return result;
}

inline bool isValidCharacter(char val) {
    return isalpha(val) ||
           isdigit(val) ||
           val == ' ' ||
           val == '.' ||
           val == '?' ||
           val == ',' ||
           val == '!';
}

string filterContent(string& content) {
    // Unfortunatelly gcc 4.8.2 does not yet have support for regex

    // Transform new lines into spaces so that we won't have
    // sentences on subsequent lines starting one after another
    for_each(content.begin(), content.end(), [](char& val) {
        val = val == '\n' ? ' ' : val;
    });

    // Remove invalid characters from the string
    content.erase(remove_if(content.begin(), content.end(), [](char val) {
        return !isValidCharacter(val);
    }), content.end());

    return content;
}

MorseWriter::MorseWriter(const MorseWriterConfiguration& config)
    : configuration(config) {
}

void MorseWriter::write() {

    cout << "Rendering " << configuration.inputFilename
         << " into " << configuration.outputFilename
         << " ..." << endl;

    string&& content = getFileContent(configuration.inputFilename);
    content = filterContent(content);
    cout << "Read content is: [" << content << "]" << endl;
}

