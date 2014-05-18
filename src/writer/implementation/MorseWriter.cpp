#include <MorseWriter.h>
#include <MorseDataSource.h>
#include <MorseDictionary.h>
#include <MorseRenderer.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <WavWriter.h>

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

MorseWriter::MorseWriter(const MorseWriterConfiguration& config)
    : configuration(config) {
}

void MorseWriter::write() {
    cout << "Rendering " << configuration.inputFilename
         << " into " << configuration.outputFilename
         << " ..." << endl;

    MorseDictionary dictionary = MorseDictionary::defaultDictionary();
    string&& content = getFileContent(configuration.inputFilename);

    AudioSettings audioSettings;
    audioSettings.sampleRate = configuration.sampleRate;
    audioSettings.channels = configuration.channels;

    MorseCodeStyle style;
    MorseCodeSpeed speed = MorseCodeSpeed::fromParisWpmAndStyle(configuration.speedInWpm, style);

    MorseDataSource dataSource = MorseDataSource(content, dictionary);
    MorseRenderer renderer = MorseRenderer(dataSource, audioSettings, configuration.frequency, configuration.punchiness, speed);

    const int SAMPLES = 1024;
    short buffer[SAMPLES];

    WavWriter wavWriter(audioSettings, configuration.outputFilename);
    wavWriter.begin();
    while (!renderer.finished()) {
        renderer.render(buffer, SAMPLES);
        wavWriter.write(buffer, SAMPLES);
    }
    wavWriter.end();
}

