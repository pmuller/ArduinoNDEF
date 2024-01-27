EXTRA_CXXFLAGS := -g
ARDUINO_LIBS := AUnit MFRC522v2-NDEF
include ../../../EpoxyDuino/EpoxyDuino.mk

valgrind_run:
	valgrind --error-exitcode=1 --leak-check=full ./$(APP_NAME).out

test: valgrind_run
