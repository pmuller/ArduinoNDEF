#include "unity.h"

#include <MFRC522v2-NDEF.hpp>

void test_record_constructor()
{
  NdefRecord record;
  TEST_ASSERT_EQUAL(record.get_type_name_format(), NdefRecord::TNF_EMPTY);
}

int run_unity_tests(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_record_constructor);
  return UNITY_END();
}

#if defined(ARDUINO)

#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);
  run_unity_tests();
}
void loop() {}

#else

int main() { return run_unity_tests(); }

#endif