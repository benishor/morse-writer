#include <MorseWriterConfiguration.h>
#include <Utils.h>

MorseWriterConfiguration readConfiguration(const std::vector<std::string> args) throw (ConfigurationException) {
    MorseWriterConfiguration config;

    bool speedSpecified = false;
    bool farnsworthSpecified = false;

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
                    speedSpecified = true;
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-sf") {
                if (index < args.size()) {
                    config.farnsworthSpeedInWpm = std::stoi(args[index]);
                    farnsworthSpecified = true;
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-f") {
                if (index < args.size()) {
                    config.frequency = std::stoi(args[index]);
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-sr") {
                if (index < args.size()) {
                    config.sampleRate = std::stoi(args[index]);
                } else {
                    throw MissingArgumentValueException(i);
                }
            } else if (i == "-c") {
                if (index < args.size()) {
                    config.channels = std::stoi(args[index]);
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

    if (speedSpecified && !farnsworthSpecified) {
        config.farnsworthSpeedInWpm = config.speedInWpm;
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

    if (config.farnsworthSpeedInWpm < 5 || config.farnsworthSpeedInWpm > 1000) {
        throw BadConfigurationException("Farnsworth speed out of range [5...1000] WPM");
    }

    if (config.farnsworthSpeedInWpm > config.speedInWpm) {
        throw BadConfigurationException("Farnsworth speed must be less or equal to configured speed");
    }

    if (config.frequency < 10 || config.speedInWpm > 4000) {
        throw BadConfigurationException("Frequency out of range [10...4000] Hz");
    }

    if (config.punchiness < 1 || config.punchiness > 20) {
        throw BadConfigurationException("Punchiness out of range [1...20]");
    }

    if (config.channels < 1 || config.channels > 2) {
        throw BadConfigurationException("Channels out of range [1...2]");
    }
}
