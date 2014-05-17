#include <MorseWriter.h>
#include <MorseDataSource.h>
#include <MorseDictionary.h>
#include <MorseRenderer.h>
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

string filterContent(string& content, const MorseDictionary& dictionary) {
    // Unfortunatelly gcc 4.8.2 does not yet have support for regex

    // Transform new lines into spaces so that we won't have
    // sentences on subsequent lines starting one after another
    for_each(content.begin(), content.end(), [](char& val) {
        val = val == '\n' ? ' ' : val;
    });

    // Remove invalid characters from the string
    content.erase(remove_if(content.begin(), content.end(), [dictionary](char val) {
        return val != ' ' && !dictionary.contains(val);
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

    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    string&& content = getFileContent(configuration.inputFilename);
    content = filterContent(content, dictionary);

    MorseDataSource dataSource = MorseDataSource(content, dictionary);
    MorseRenderer renderer = MorseRenderer(dataSource);

    const int SAMPLES = 1024;
    short buffer[SAMPLES];

    ofstream out(configuration.outputFilename, ios::binary);
    while (!renderer.finished()) {
        int renderedSamples = renderer.render(buffer, SAMPLES);
        out.write((char*)buffer, sizeof(short)*SAMPLES);
    }
    out.close();
}

