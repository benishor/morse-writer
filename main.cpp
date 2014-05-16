#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <stdio.h>
#include <algorithm>

struct Config {
	std::string inputFilename;
	std::string outputFilename;
	int speedInWpm = 20;
	int frequency = 600;
	float punchiness = 0.0f;
};


// http://stackoverflow.com/a/217605

// trim from start
static inline std::string ltrim(const std::string& s) {
	std::string result(s);
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return result;
}

// trim from end
static inline std::string rtrim(const std::string& s) {
	std::string result(s);
	result.erase(std::find_if(result.rbegin(), result.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());
	return result;
}

// trim from both ends
static inline std::string trim(const std::string& s) {
	return ltrim(rtrim(s));
}

class ConfigurationException : public std::exception {
};


class BadConfigurationException : public ConfigurationException {
public:
	explicit BadConfigurationException(const std::string& reason) {
		message = reason;
	}
	virtual const char* what() const noexcept {
		return message.c_str();
	}
private:
	std::string message;
};


class InvalidArgumentException : public ConfigurationException {
public:
	explicit InvalidArgumentException(const std::string& argument) {
		message = "Unknown argument " + argument;
	}
	virtual const char* what() const noexcept {
		return message.c_str();
	}
private:
	std::string message;
};

class MissingArgumentValueException : public ConfigurationException {
public:
	explicit MissingArgumentValueException(const std::string& option) {
		message = "Missing argument for option " + option;
	}
	virtual const char* what() const noexcept {
		return message.c_str();
	}
private:
	std::string message;
};

class InvalidArgumentValueException : public ConfigurationException {
public:
	explicit InvalidArgumentValueException(const std::string& option) {
		message = "Invalid argument value for option " + option;
	}
	virtual const char* what() const noexcept {
		return message.c_str();
	}
private:
	std::string message;
};


Config readConfiguration(const std::vector<std::string> args) {
	Config config;

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

void validateConfiguration(const Config& config) {
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


void showUsage(const std::string& programName) {
	std::cout << std::endl;
	std::cout << "Usage: " << programName << " -i fileToConvert -o outputWavFile" << std::endl;
	std::cout << std::endl;
	std::cout << "\tOptional arguments:" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t-s speedInWpm. Integer, defaults to 20" << std::endl;
	std::cout << "\t\t-f frequencyInHz, Integer, defaults to 600" << std::endl;
	std::cout << "\t\t-p punchiness. Float, defaults to 0" << std::endl;
	std::cout << std::endl;
}

int main(int argc, char** argv) {

	try {
		std::vector<std::string> args(argv+1, argv+argc);
		Config&& config = readConfiguration(args);
		validateConfiguration(config);

	} catch (const ConfigurationException& e) {
		std::cerr << std::endl << "Error: " << e.what() << std::endl;
		showUsage(std::string(argv[0]));
		return 1;
	}

	return 0;
}