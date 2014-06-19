#pragma once

#include <AudioSettings.h>
#include <string>
#include <fstream>

class WavWriter {
public:
    /**
     * @brief Constructs a wav writer configured with the provided settings
     *
     * @param settings audio settings which determine the size of the samples
     * @param outputFilename the filename to output content to
     */
    explicit WavWriter(const AudioSettings& settings, const std::string& outputFilename);

    /**
     * @brief Opens the output file and writes the wav header
     */
    void begin();

    /**
     * @brief Writes the provided data buffer to the output
     * @details The size to be written is calculated from the number of samples and current audio settings
     *
     * @param buffer the buffer containing the data to be written
     * @param samples number of samples to write from the buffer
     */
    void write(short* buffer, int samples);

    /**
     * @brief Ends the writing process and closes the file
     * @details Corrects the wav header by overwriting the data size field and closes the file.
     */
    void end();

private:

    void writeWavHeader();
    void writeWavRealSize();
    void writeString(const std::string& text);
    void writeLittleEndian(unsigned int value, int bytesToWrite);

    const AudioSettings& settings;
    const std::string& filename;
    std::fstream file;
    unsigned int writtenDataBytes;
};