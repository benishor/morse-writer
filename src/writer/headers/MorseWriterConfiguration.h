#pragma once

#include <string>
#include <vector>
#include <exception>


struct MorseWriterConfiguration {
	std::string inputFilename;
	std::string outputFilename;
	int speedInWpm = 20;
	int frequency = 600;
	float punchiness = 0.0f;
};

class ConfigurationException : public std::exception {
};

// -------------------------------------------------------------------------------------------------------------

MorseWriterConfiguration readConfiguration(const std::vector<std::string> args) throw (ConfigurationException);
void validateConfiguration(const MorseWriterConfiguration& config) throw (ConfigurationException);

// -------------------------------------------------------------------------------------------------------------

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

