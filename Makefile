.PHONY: all clean build test

all: clean test

clean:
	make -C tests clean

build:
	EXTRA_CXXFLAGS=-g make -C build

test:
	make -C tests test
