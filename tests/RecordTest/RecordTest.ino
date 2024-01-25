#line 2 "RecordTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.hpp>

test(record_constructor)
{
  NdefRecord record;
  assertEqual(record.get_type_name_format(), NdefRecord::TNF_EMPTY);
  assertFalse(record.is_message_begin);
  assertFalse(record.is_message_end);
  assertEqual(record.get_type(), NULL);
  assertEqual(record.get_id(), NULL);
  assertEqual(record.get_payload(), NULL);
  assertEqual(record.get_encoded_size(), (uint32_t)3);
}

test(record_encode_empty)
{
  NdefRecord record;
  uint8_t *encoded = record.encode();
  // Only the SR flag is 1
  assertEqual(encoded[0], (uint8_t)0b00010000);
  // Type length is 0
  assertEqual(encoded[1], (uint8_t)0);
  // Payload length is 0
  assertEqual(encoded[2], (uint8_t)0);
  free(encoded);
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