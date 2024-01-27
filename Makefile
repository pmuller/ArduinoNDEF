.PHONY: all clean build test

all: clean test

clean:
	make -C tests clean

build:
	make -C build

test:
	make -C tests test
