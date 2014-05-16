all: morse-writer

morse-writer: main.cpp Makefile
	clang++ -o morse-writer main.cpp -std=c++11
	chmod a+x morse-writer
