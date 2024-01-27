#pragma once

// Early return when an error occurs
#define RETURN_IF_ERROR(expression, error_handling_block)                              \
  error = expression;                                                                  \
  if (error != NDEF_SUCCESS)                                                           \
  {                                                                                    \
    error_handling_block;                                                              \
    return error;                                                                      \
  }

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