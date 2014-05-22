morse-writer
============

Utility program that helps rendering a text file into a morse code wav file.

###Clone

```
git clone git@github.com:benishor/morse-writer.git
cd morse-writer
git submodule init
git submodule update
```

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
		-sf Farsnworth feel-like speed. Integer, defaults to configured speed
		-f frequencyInHz, Integer, defaults to 600
		-p punchiness. Float, defaults to 1
		-sr samplerate. Integer, defaults to 44100
		-c channels. Integer, defaults to 1
```

Example: `./morse-writer -i book.txt book.wav -s 33`
