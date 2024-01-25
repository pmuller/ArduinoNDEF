#line 2 "MessageTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.hpp>

test(message_constructor)
{
  NdefMessage message;
  assertEqual(message.get_record_count(), (uint8_t)0);
  assertEqual(message.get_encoded_size(), (uint32_t)0);
}

void setup()
{
#if !defined(EPOXY_DUINO)
  delay(1000); // Wait for stability on some boards to avoid garbage on Serial
#endif
  Serial.begin(115200);
  while (!Serial)
    ;
}

void loop() { aunit::TestRunner::run(); }