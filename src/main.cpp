#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <MorseWriter.h>


void showUsage(const std::string& programName) {
    std::cout << std::endl;
    std::cout << "Usage: " << programName << " -i fileToConvert -o outputWavFile" << std::endl;
    std::cout << std::endl;
    std::cout << "\tOptional arguments:" << std::endl;
    std::cout << std::endl;
    std::cout << "\t\t-s speedInWpm. Integer, defaults to 20" << std::endl;
    std::cout << "\t\t-sf Farsnworth feel-like speed. Integer, defaults to configured speed" << std::endl;
    std::cout << "\t\t-f frequencyInHz, Integer, defaults to 600" << std::endl;
    std::cout << "\t\t-p punchiness. Float, defaults to 1" << std::endl;
    std::cout << "\t\t-sr samplerate. Integer, defaults to 44100" << std::endl;
    std::cout << "\t\t-c channels. Integer, defaults to 1" << std::endl;
    std::cout << std::endl;
}


int main(int argc, char** argv) {
    try {
        std::vector<std::string> args(argv + 1, argv + argc);
        MorseWriterConfiguration&& config = readConfiguration(args);
        validateConfiguration(config);

        MorseWriter writer(config);
        writer.write();
    } catch (const ConfigurationException& e) {
        std::cerr << std::endl << "Error: " << e.what() << std::endl;
        showUsage(std::string(argv[0]));
        return 1;
    } catch (const MorseWriterException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}