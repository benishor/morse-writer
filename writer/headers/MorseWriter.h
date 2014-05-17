#pragma once

#include <MorseWriterConfiguration.h>

class MorseWriter {
public:
	explicit MorseWriter(const MorseWriterConfiguration& config);
	
	void write();

private:
	const MorseWriterConfiguration& configuration;
};
