#include <MorseWriter.h>
#include <UnitTest++.h>
#include <iostream>

using namespace std;

TEST(Configuration_defaultValues) {
    MorseWriterConfiguration config;

    CHECK_EQUAL(config.inputFilename, "");
    CHECK_EQUAL(config.outputFilename, "");
    CHECK_EQUAL(config.punchiness, 1.f);
    CHECK_EQUAL(config.frequency, 600);
    CHECK_EQUAL(config.speedInWpm, 20);
}

TEST(ReadConfiguration_missingInputArguments) {
    vector<string> argumentsToCheckForMissingValues {"-i", "-o", "-s", "-f", "-p", "-sr", "-c", "-sf"};
    for (auto argument : argumentsToCheckForMissingValues) {

        vector<string> commandLineArguments { argument };
        CHECK_THROW(
            MorseWriterConfiguration && config = readConfiguration(commandLineArguments),
            MissingArgumentValueException
        );
    }
}

TEST(ReadConfiguration_succes) {
    vector<string> commandLineArguments {
        "-i", "input.txt",
        "-o", "output.wav",
        "-s", "33",
        "-f", "1234",
        "-p", "1.234",
        "-sr", "22050",
        "-c", "2",
        "-sf", "30"
    };

    MorseWriterConfiguration&&  config = readConfiguration(commandLineArguments);

    CHECK_EQUAL(config.inputFilename, "input.txt");
    CHECK_EQUAL(config.outputFilename, "output.wav");
    CHECK_EQUAL(config.punchiness, 1.234f);
    CHECK_EQUAL(config.frequency, 1234);
    CHECK_EQUAL(config.speedInWpm, 33);
    CHECK_EQUAL(config.sampleRate, 22050);
    CHECK_EQUAL(config.channels, 2);
    CHECK_EQUAL(config.farnsworthSpeedInWpm, 30);
}


TEST(ReadConfiguration_farnsworthSetAsSpeedIfNotSpecified) {
    vector<string> commandLineArguments {
        "-s", "33"
    };

    MorseWriterConfiguration&&  config = readConfiguration(commandLineArguments);
    CHECK_EQUAL(config.speedInWpm, 33);
    CHECK_EQUAL(config.farnsworthSpeedInWpm, 33);
}

TEST(ValidateConfiguration_farnsworthMustBeLessThanSpeed) {
    vector<string> commandLineArguments {
        "-s", "33",
        "-sf", "34"
    };

    MorseWriterConfiguration&& config = readConfiguration(commandLineArguments);
    CHECK_THROW(
        validateConfiguration(config),
        BadConfigurationException
    );
}
