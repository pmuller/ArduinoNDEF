#line 2 "MessageTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.h>

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
  assertEqual(message.add_record(record), NDEF_SUCCESS);
  assertEqual(message.get_record_count(), (uint8_t)1);
  assertEqual(message.get_encoded_size(), (uint32_t)3);
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[0], (uint8_t)0b11010000);
  assertEqual(encoded[1], (uint8_t)0);
  assertEqual(encoded[2], (uint8_t)0);
  delete[] encoded;
}

test(message_add_three_records)
{
  NdefMessage message;
  NdefRecord *record1 = new NdefRecord();
  NdefRecord *record2 = new NdefRecord();
  NdefRecord *record3 = new NdefRecord();
  assertEqual(message.add_record(record1), NDEF_SUCCESS);
  assertEqual(message.add_record(record2), NDEF_SUCCESS);
  assertEqual(message.add_record(record3), NDEF_SUCCESS);
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
  delete[] encoded;
}

test(message_add_mime_media_record)
{
  NdefMessage message;
  assertEqual(
      message.add_mime_media_record("text/plain", (uint8_t *)"Hello", 5),
      NDEF_SUCCESS
  );
  assertEqual(message.get_record_count(), (uint8_t)1);
  assertEqual(message.get_encoded_size(), (uint32_t)18);
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x02
  assertEqual(encoded[0], (uint8_t)0b11010010);
  // Type length is 10
  assertEqual(encoded[1], (uint8_t)10);
  // Payload length is 5
  assertEqual(encoded[2], (uint8_t)5);
  // Type is "text/plain"
  assertEqual(encoded[3], (uint8_t)'t');
  assertEqual(encoded[4], (uint8_t)'e');
  assertEqual(encoded[5], (uint8_t)'x');
  assertEqual(encoded[6], (uint8_t)'t');
  assertEqual(encoded[7], (uint8_t)'/');
  assertEqual(encoded[8], (uint8_t)'p');
  assertEqual(encoded[9], (uint8_t)'l');
  assertEqual(encoded[10], (uint8_t)'a');
  assertEqual(encoded[11], (uint8_t)'i');
  assertEqual(encoded[12], (uint8_t)'n');
  // Payload is "Hello"
  assertEqual(encoded[13], (uint8_t)'H');
  assertEqual(encoded[14], (uint8_t)'e');
  assertEqual(encoded[15], (uint8_t)'l');
  assertEqual(encoded[16], (uint8_t)'l');
  assertEqual(encoded[17], (uint8_t)'o');
  delete[] encoded;
}

test(message_add_uri_record)
{
  NdefMessage message;
  assertEqual(message.add_uri_record("https://www.google.com"), NDEF_SUCCESS);
  assertEqual(message.get_record_count(), (uint8_t)1);
  assertEqual(message.get_encoded_size(), (uint32_t)15);
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
  assertEqual(encoded[0], (uint8_t)0b11010001);
  // Type length is 1
  assertEqual(encoded[1], (uint8_t)1);
  // Payload length is 14
  assertEqual(encoded[2], (uint8_t)11);
  // Type is RTD_URI
  assertEqual(encoded[3], (uint8_t)NdefRecord::RTD_URI);
  // Payload is "\2google.com"
  assertEqual(encoded[4], (uint8_t)0x02);
  assertEqual(encoded[5], (uint8_t)'g');
  assertEqual(encoded[6], (uint8_t)'o');
  assertEqual(encoded[7], (uint8_t)'o');
  assertEqual(encoded[8], (uint8_t)'g');
  assertEqual(encoded[9], (uint8_t)'l');
  assertEqual(encoded[10], (uint8_t)'e');
  assertEqual(encoded[11], (uint8_t)'.');
  assertEqual(encoded[12], (uint8_t)'c');
  assertEqual(encoded[13], (uint8_t)'o');
  assertEqual(encoded[14], (uint8_t)'m');
  delete[] encoded;
}

test(message_add_external_type_record)
{
  NdefMessage message;
  assertEqual(
      message.add_external_type_record(
          "com.example",
          "externalType",
          (uint8_t *)"Hello",
          5
      ),
      NDEF_SUCCESS
  );
  assertEqual(message.get_record_count(), (uint8_t)1);
  assertEqual(message.get_encoded_size(), (uint32_t)44);
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x04 (external)
  assertEqual(encoded[0], (uint8_t)0b11010100);
  // Type length is 11
  assertEqual(encoded[1], (uint8_t)36);
  // Payload length is 5
  assertEqual(encoded[2], (uint8_t)5);
  // Type is "com.example:externalType"
  assertEqual(encoded[3], (uint8_t)'u');
  assertEqual(encoded[4], (uint8_t)'r');
  assertEqual(encoded[5], (uint8_t)'n');
  assertEqual(encoded[6], (uint8_t)':');
  assertEqual(encoded[7], (uint8_t)'n');
  assertEqual(encoded[8], (uint8_t)'f');
  assertEqual(encoded[9], (uint8_t)'c');
  assertEqual(encoded[10], (uint8_t)':');
  assertEqual(encoded[11], (uint8_t)'e');
  assertEqual(encoded[12], (uint8_t)'x');
  assertEqual(encoded[13], (uint8_t)'t');
  assertEqual(encoded[14], (uint8_t)':');
  assertEqual(encoded[15], (uint8_t)'c');
  assertEqual(encoded[16], (uint8_t)'o');
  assertEqual(encoded[17], (uint8_t)'m');
  assertEqual(encoded[18], (uint8_t)'.');
  assertEqual(encoded[19], (uint8_t)'e');
  assertEqual(encoded[20], (uint8_t)'x');
  assertEqual(encoded[21], (uint8_t)'a');
  assertEqual(encoded[22], (uint8_t)'m');
  assertEqual(encoded[23], (uint8_t)'p');
  assertEqual(encoded[24], (uint8_t)'l');
  assertEqual(encoded[25], (uint8_t)'e');
  assertEqual(encoded[26], (uint8_t)':');
  assertEqual(encoded[27], (uint8_t)'e');
  assertEqual(encoded[28], (uint8_t)'x');
  assertEqual(encoded[29], (uint8_t)'t');
  assertEqual(encoded[30], (uint8_t)'e');
  assertEqual(encoded[31], (uint8_t)'r');
  assertEqual(encoded[32], (uint8_t)'n');
  assertEqual(encoded[33], (uint8_t)'a');
  assertEqual(encoded[34], (uint8_t)'l');
  assertEqual(encoded[35], (uint8_t)'T');
  assertEqual(encoded[36], (uint8_t)'y');
  assertEqual(encoded[37], (uint8_t)'p');
  assertEqual(encoded[38], (uint8_t)'e');
  // Payload is "Hello"
  assertEqual(encoded[39], (uint8_t)'H');
  assertEqual(encoded[40], (uint8_t)'e');
  assertEqual(encoded[41], (uint8_t)'l');
  assertEqual(encoded[42], (uint8_t)'l');
  assertEqual(encoded[43], (uint8_t)'o');
  delete[] encoded;
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