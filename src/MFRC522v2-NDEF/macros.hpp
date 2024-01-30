#pragma once

// Only log debug messages when DEBUG is defined
#ifdef DEBUG
#define PRINT(value) Serial.print(value)
#define PRINTLN(value) Serial.println(value)
#define PRINTLN_DEC(value) Serial.println(value, DEC)
#else // DEBUG
#define PRINT(value) (void)0
#define PRINTLN(value) (void)0
#define PRINTLN_DEC(value) (void)0
#endif // DEBUG

// ArduinoCore-avr defines it:
// https://github.com/arduino/ArduinoCore-avr/blob/63092126a406402022f943ac048fa195ed7e944b/cores/arduino/Arduino.h#L92
// But arduino-esp32 defines _min():
// https://github.com/espressif/arduino-esp32/blob/7d26b070d7e0a7a3ba67de42eae5158fb8d42f96/cores/esp32/Arduino.h#L74
#define min(a, b) ((a) < (b) ? (a) : (b))