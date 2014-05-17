#include <MorseWriter.h>
#include <UnitTest++.h>
#include <iostream>

using namespace std;

TEST(Configuration_defaultValues) {
    MorseWriterConfiguration config;

    CHECK_EQUAL(config.inputFilename, "");
    CHECK_EQUAL(config.outputFilename, "");
    CHECK_EQUAL(config.punchiness, 0.f);
    CHECK_EQUAL(config.frequency, 600);
    CHECK_EQUAL(config.speedInWpm, 20);
}

TEST(ReadConfiguration_missingInputArguments) {
    vector<string> argumentsToCheckForMissingValues {"-i", "-o", "-s", "-f", "-p"};
    for (auto argument : argumentsToCheckForMissingValues) {

        vector<string> commandLineArguments { argument };
        CHECK_THROW(
            MorseWriterConfiguration && config = readConfiguration(commandLineArguments),
            ConfigurationException
        );
    }
}

TEST(ReadConfiguration_succes) {
    vector<string> commandLineArguments {
        "-i", "input.txt",
        "-o", "output.wav",
        "-s", "33",
        "-f", "1234",
        "-p", "1.234"
    };

	MorseWriterConfiguration && config = readConfiguration(commandLineArguments);
	
    CHECK_EQUAL(config.inputFilename, "input.txt");
    CHECK_EQUAL(config.outputFilename, "output.wav");
    CHECK_EQUAL(config.punchiness, 1.234f);
    CHECK_EQUAL(config.frequency, 1234);
    CHECK_EQUAL(config.speedInWpm, 33);
}

