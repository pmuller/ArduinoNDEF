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

test(record_encode_short)
{
  NdefRecord record;
  record.set_type_name_format(NdefRecord::TNF_WELL_KNOWN);
  record.set_type(NdefRecord::RTD_TEXT);
  record.set_payload((uint8_t *)"Hello", 5);
  assertEqual(record.get_encoded_size(), (uint32_t)9);
  uint8_t *encoded = record.encode();
  // SR flag is 1, TNF is 0x01
  assertEqual(encoded[0], (uint8_t)0b00010001);
  // Type length is 1
  assertEqual(encoded[1], (uint8_t)1);
  // Payload length is 5
  assertEqual(encoded[2], (uint8_t)5);
  // Type is RTD_TEXT
  assertEqual(encoded[3], (uint8_t)NdefRecord::RTD_TEXT);
  // Payload is "Hello"
  assertEqual(encoded[4], (uint8_t)'H');
  assertEqual(encoded[5], (uint8_t)'e');
  assertEqual(encoded[6], (uint8_t)'l');
  assertEqual(encoded[7], (uint8_t)'l');
  assertEqual(encoded[8], (uint8_t)'o');
  free(encoded);
}

const uint8_t LONG_TEXT[] =
    "For instance, on the planet Earth, man had always assumed that he was more "
    "intelligent than dolphins because he had achieved so much - the wheel, New York, "
    "wars and so on - whilst all the dolphins had ever done was muck about in the "
    "water having a good time. But conversely, the dolphins had always believed that "
    "they were far more intelligent than man - for precisely the same reasons.";

test(record_encode_long_text)
{
  NdefRecord record;
  record.set_type_name_format(NdefRecord::TNF_WELL_KNOWN);
  record.set_type(NdefRecord::RTD_TEXT);
  record.set_payload((uint8_t *)LONG_TEXT, 386);
  assertEqual(record.get_encoded_size(), (uint32_t)393);
  uint8_t *encoded = record.encode();
  // SR flag is 0, TNF is 0x01
  assertEqual(encoded[0], (uint8_t)0b00000001);
  // Type length is 1
  assertEqual(encoded[1], (uint8_t)1);
  // Payload length is 386
  assertEqual(encoded[2], (uint8_t)0);
  assertEqual(encoded[3], (uint8_t)0);
  assertEqual(encoded[4], (uint8_t)1);
  assertEqual(encoded[5], (uint8_t)130);
  // Type is RTD_TEXT
  assertEqual(encoded[6], (uint8_t)NdefRecord::RTD_TEXT);
  // Check payload
  for (uint32_t i = 0; i < 386; i++)
    assertEqual(encoded[7 + i], LONG_TEXT[i]);
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