#include <WavWriter.h>

WavWriter::WavWriter(const AudioSettings& audioSettings, const std::string& outputFilename)
    : settings(audioSettings),
      filename(outputFilename) {}

void WavWriter::begin() {
    file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    writeWavHeader();
    writtenDataBytes = 0;
}

void WavWriter::write(short* buffer, int samples) {
    for (int i = 0; i < samples; i++) {
        for (int j = 0; j < settings.channels; j++) {
            writeLittleEndian((unsigned int)(*buffer++), settings.bytesPerSample());
        }
    }
    writtenDataBytes += samples * settings.bytesPerSample() * settings.channels;
}

void WavWriter::end() {
    writeWavRealSize();
    file.close();
}

void WavWriter::writeWavHeader() {
    // RIFF header
    writeString("RIFF");
    writeLittleEndian(0, 4); // total file size in bytes. unknown at this time
    writeString("WAVE");

    // FMT subchunk
    writeString("fmt ");
    writeLittleEndian(16, 4); // 16 bytes is the subchunk size (following)
    writeLittleEndian(1, 2); // pcm format
    writeLittleEndian(settings.channels, 2);
    writeLittleEndian(settings.sampleRate, 4);

    unsigned int byteRate = settings.sampleRate * settings.channels * settings.bytesPerSample();
    writeLittleEndian(byteRate, 4);

    writeLittleEndian(settings.bytesPerSample() * settings.channels, 2); // block align
    writeLittleEndian(8 * settings.bytesPerSample(), 2); // bits / sample

    // DATA subchunk
    writeString("data");
    writeLittleEndian(0, 4); // size of sunchunk in bytes. unknown at this time
}

void WavWriter::writeWavRealSize() {
	file.seekg(4, std::ios::beg);
	unsigned int totalFileSize = writtenDataBytes + 44;
	writeLittleEndian(totalFileSize, 4);

	file.seekg(40, std::ios::beg);
	writeLittleEndian(writtenDataBytes, 4);
}

void WavWriter::writeString(const std::string& text) {
    file.write(text.c_str(), text.size());
}

void WavWriter::writeLittleEndian(unsigned int value, int bytesToWrite) {
    unsigned char byte;
    while (bytesToWrite--) {
        byte = value & 0xff;
        file.write((char*)&byte, 1);
        value >>= 8;
    }
}
