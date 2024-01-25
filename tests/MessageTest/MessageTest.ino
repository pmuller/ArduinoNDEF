#line 2 "MessageTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.hpp>

test(message_constructor)
{
  NdefMessage message;
  assertEqual(message.get_record_count(), (uint8_t)0);
  assertEqual(message.get_encoded_size(), (uint32_t)0);
}

test(message_add_one_record)
{
  NdefMessage message;
  NdefRecord *record = new NdefRecord();
  message.add_record(record);
  assertEqual(message.get_record_count(), (uint8_t)1);
  assertEqual(message.get_encoded_size(), (uint32_t)3);
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[0], (uint8_t)0b11010000);
  assertEqual(encoded[1], (uint8_t)0);
  assertEqual(encoded[2], (uint8_t)0);
  free(encoded);
}

test(message_add_three_records)
{
  NdefMessage message;
  NdefRecord *record1 = new NdefRecord();
  NdefRecord *record2 = new NdefRecord();
  NdefRecord *record3 = new NdefRecord();
  message.add_record(record1);
  message.add_record(record2);
  message.add_record(record3);
  assertEqual(message.get_record_count(), (uint8_t)3);
  assertEqual(message.get_encoded_size(), (uint32_t)9);
  uint8_t *encoded = message.encode();
  // MB=1, ME=0, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[0], (uint8_t)0b10010000);
  assertEqual(encoded[1], (uint8_t)0);
  assertEqual(encoded[2], (uint8_t)0);
  // MB=0, ME=0, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[3], (uint8_t)0b00010000);
  assertEqual(encoded[4], (uint8_t)0);
  assertEqual(encoded[5], (uint8_t)0);
  // MB=0, ME=1, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[6], (uint8_t)0b01010000);
  assertEqual(encoded[7], (uint8_t)0);
  assertEqual(encoded[8], (uint8_t)0);
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