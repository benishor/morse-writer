#include <MorseWriter.h>
#include <iostream>

MorseWriter::MorseWriter(const MorseWriterConfiguration& config) 
	: configuration(config)
{
}
	
void MorseWriter::write() {
	
	std::cout << "Rendering " << configuration.inputFilename << " into " << configuration.outputFilename << " ..." << std::endl;
}
