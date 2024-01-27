#line 2 "RecordTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.h>

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
  record.is_message_begin = true;
  record.is_message_end = true;
  record.set_type_name_format(NdefRecord::TNF_WELL_KNOWN);
  assertEqual(record.set_type(NdefRecord::RTD_TEXT), NDEF_SUCCESS);
  assertEqual(record.set_payload((uint8_t *)"Hello", 5), NDEF_SUCCESS);
  assertEqual(record.get_encoded_size(), (uint32_t)9);
  uint8_t *encoded = record.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
  assertEqual(encoded[0], (uint8_t)0b11010001);
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

test(record_decode_short)
{
  uint8_t encoded[] = {
      0b11010001,           // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
      1,                    // Type length is 1
      5,                    // Payload length is 5
      NdefRecord::RTD_TEXT, // Type is RTD_TEXT
      'H',
      'e',
      'l',
      'l',
      'o'
  };
  NdefRecord record;
  assertEqual(record.decode(encoded, sizeof(encoded)), NDEF_SUCCESS);
  assertTrue(record.is_message_begin);
  assertTrue(record.is_message_end);
  assertEqual(record.get_type_name_format(), NdefRecord::TNF_WELL_KNOWN);
  assertEqual(record.get_type_length(), (uint8_t)1);
  assertEqual(record.get_type()[0], (uint8_t)'T');
  assertEqual(record.get_payload_length(), (uint32_t)5);
  uint8_t *payload = record.get_payload();
  assertEqual(payload[0], (uint8_t)'H');
  assertEqual(payload[1], (uint8_t)'e');
  assertEqual(payload[2], (uint8_t)'l');
  assertEqual(payload[3], (uint8_t)'l');
  assertEqual(payload[4], (uint8_t)'o');
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
  assertEqual(record.set_type(NdefRecord::RTD_TEXT), NDEF_SUCCESS);
  assertEqual(record.set_payload((uint8_t *)LONG_TEXT, 386), NDEF_SUCCESS);
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

test(record_decode_long_text)
{
  uint8_t *encoded = (uint8_t *)malloc(393);
  // MB=0, ME=0, CF=0, SR=0, IL=0, TNF=0x01
  encoded[0] = 0b00000001;
  // Type length is 1
  encoded[1] = 1;
  // Payload length is 386
  encoded[2] = 0;
  encoded[3] = 0;
  encoded[4] = 1;
  encoded[5] = 130;
  // Type is RTD_TEXT
  encoded[6] = NdefRecord::RTD_TEXT;
  // Copy reference text
  memcpy(encoded + 7, LONG_TEXT, 386);
  // Create record
  NdefRecord record;
  // Check decoding
  assertEqual(record.decode(encoded, 393), NDEF_SUCCESS);
  free(encoded);
  assertFalse(record.is_message_begin);
  assertFalse(record.is_message_end);
  assertEqual(record.get_type_name_format(), NdefRecord::TNF_WELL_KNOWN);
  assertEqual(record.get_type_length(), (uint8_t)1);
  assertEqual(record.get_type()[0], (uint8_t)'T');
  assertEqual(record.get_payload_length(), (uint32_t)386);
  uint8_t *payload = record.get_payload();
  for (uint32_t i = 0; i < 386; i++)
    assertEqual(payload[i], LONG_TEXT[i]);
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