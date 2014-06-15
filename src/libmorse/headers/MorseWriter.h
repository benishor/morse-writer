#pragma once

#include <MorseWriterConfiguration.h>

class MorseWriterException : public std::exception {
public:
    explicit MorseWriterException() {
    }

    explicit MorseWriterException(const std::string& reason)
        : message(reason) {
    }

    virtual const char* what() const noexcept {
        return message.c_str();
    }
private:
    std::string message;
};



class MorseWriter {
public:
    explicit MorseWriter(const MorseWriterConfiguration& config);

    void write();

private:
    const MorseWriterConfiguration& configuration;
};
