
# is either c++0x or c++11
cversion=c++0x

# basic parameters
CC = g++
baseCFLAGS = -c -g -Wall -O4 -std=$(cversion)
baseLDFLAGS = -g -L/usr/lib64 -std=$(cversion)
threadlib= -pthread

# find source and targets and set the object files
src = $(wildcard source/*.cpp)
obj = $(patsubst source/%.cpp, build/%.o, $(src))

all: adapter_trim

adapter_trim: build/main.o
	-mkdir -p bin/
	$(CC) build/main.o $(baseLDFLAGS) $(threadlib) \
		-Irwwb \
		-Iinclude \
		-lboost_program_options \
		-o bin/adapter_trim
	
	rm -r build

clean:
	-rm -rf build/*
	-rm bin/adapter_trim

build/%.o: source/%.cpp
	mkdir -p build
	$(CC) $(baseCFLAGS) \
		-Irwwb \
		-Iinclude \
		$(threadlib) \
		source/$*.cpp -o $@
