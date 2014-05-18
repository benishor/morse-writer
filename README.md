morse-writer
============

Utility program that helps rendering a text file into a morse code wav file.

###Build

```
mkdir build && cd build
cmake .. && make
```

To run unit tests `make test`

###Usage

```
./morse-writer -i fileToConvert -o outputWavFile

	Optional arguments:

		-s speedInWpm. Integer, defaults to 20
		-f frequencyInHz, Integer, defaults to 600
		-p punchiness. Float, defaults to 0
		-sr sampleRate. Integer, defaults to 44100
```

Example: `./morse-writer -i book.txt book.wav -s 33`