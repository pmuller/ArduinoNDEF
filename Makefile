.PHONY: all clean build test

all: clean test

clean:
	find . -name '*.o' -o -name '*.out' -exec rm -f {} \;

build:
	make -C tests

test: build
	make -C tests runtests
