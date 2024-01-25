#line 2 "RecordTest.ino"

#include <AUnit.h>
#include <MFRC522v2-NDEF.hpp>

test(record_constructor_no_arguments)
{
  NdefRecord record;
  assertEqual(record.get_type_name_format(), NdefRecord::TNF_EMPTY);
  assertFalse(record.is_message_begin);
  assertFalse(record.is_message_end);
  assertEqual(record.get_type(), NULL);
  assertEqual(record.get_id(), NULL);
  assertEqual(record.get_payload(), NULL);
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