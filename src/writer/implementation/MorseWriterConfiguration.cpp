#include <MorseWriterConfiguration.h>
#include <Utils.h>

MorseWriterConfiguration readConfiguration(const std::vector<std::string> args) throw (ConfigurationException) {
    MorseWriterConfiguration config;

    for (int index = 0; index < args.size(); index++) {
        const std::string& i = args[index++];

        try {
            if (i == "-i") {
                if (index < args.size()) {
                    config.inputFilename = args[index];
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-o") {
                if (index < args.size()) {
                    config.outputFilename = args[index];
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-s") {
                if (index < args.size()) {
                    config.speedInWpm = std::stoi(args[index]);
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-f") {
                if (index < args.size()) {
                    config.frequency = std::stoi(args[index]);
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-p") {
                if (index < args.size()) {
                    config.punchiness = std::stof(args[index]);
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else {
                throw InvalidArgumentException(i);
            }
        } catch (std::invalid_argument& e) {
            throw InvalidArgumentValueException(i);
        }
    }

    return config;
}

void validateConfiguration(const MorseWriterConfiguration& config) throw (ConfigurationException) {
    if (trim(config.inputFilename) == "") {
        throw BadConfigurationException("Input filename must be specified");
    }

    if (trim(config.outputFilename) == "") {
        throw BadConfigurationException("Output filename must be specified");
    }

    if (config.speedInWpm < 5 || config.speedInWpm > 1000) {
        throw BadConfigurationException("Speed out of range [5...1000] WPM");
    }

    if (config.frequency < 10 || config.speedInWpm > 4000) {
        throw BadConfigurationException("Frequency out of range [10...4000] Hz");
    }

    if (config.punchiness < 0 || config.punchiness > 20) {
        throw BadConfigurationException("Punchiness out of range [0...20]");
    }
}
