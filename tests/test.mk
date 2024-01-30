EXTRA_CXXFLAGS := -g -fno-permissive
ARDUINO_LIBS := AUnit MFRC522v2-NDEF
include ../../../EpoxyDuino/EpoxyDuino.mk

valgrind_run:
	valgrind --error-exitcode=1 --leak-check=full --track-origins=yes ./$(APP_NAME).out

test: valgrind_run
