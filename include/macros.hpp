#pragma once

#include <cstdlib>

// Early return when an error occurs
#define RETURN_IF_ERROR(expression, error_handling_block)                              \
  error = expression;                                                                  \
  if (error != NDEF_SUCCESS)                                                           \
  {                                                                                    \
    error_handling_block;                                                              \
    return error;                                                                      \
  }

/** malloc helper macro
 *
 * type:                    pointer type
 * pointer:                 pointer name
 * size:                    size of the memory to allocate
 * error_code:              error code to return if malloc fails
 * error_handling_block:    code to execute if malloc fails
 */
#define SAFE_MALLOC(type, pointer, size, error_code, error_handling_block)             \
  type pointer = (type)malloc(size);                                                   \
  if (pointer == NULL)                                                                 \
  {                                                                                    \
    error_handling_block;                                                              \
    PRINT(F("malloc failed: pointer="));                                               \
    PRINT(#pointer);                                                                   \
    PRINT(F(" size="));                                                                \
    PRINT(size);                                                                       \
    PRINT(F(" func="));                                                                \
    PRINT(__FUNCTION__);                                                               \
    PRINT(F(" file="));                                                                \
    PRINT(__FILE__);                                                                   \
    PRINT(F(" line="));                                                                \
    PRINTLN(__LINE__);                                                                 \
    return error_code;                                                                 \
  }

// Only log debug messages when DEBUG is defined
#ifdef DEBUG
#ifdef ARDUINO
#define PRINT(value) Serial.print(value)
#define PRINTLN(value) Serial.println(value)
#else // ARDUINO
#include <iostream>
#define PRINT(value) std::cout << value
#define PRINTLN(value) std::cout << value << std::endl
#endif // ARDUINO
#else  // DEBUG
#define PRINT(value) (void)0
#define PRINTLN(value) (void)0
#endif // DEBUG