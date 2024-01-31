#line 2 "RecordTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.h>

test(empty_record)
{
  auto record = ArduinoNDEF::Record::Empty::create();
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_EMPTY);
  assertFalse(record->is_message_begin);
  assertFalse(record->is_message_end);
  assertEqual(record->type().data(), NULL);
  assertEqual(record->id().data(), NULL);
  assertEqual(record->payload().data(), NULL);
  assertEqual(record->get_encoded_size(), static_cast<uint32_t>(3));
  uint8_t *encoded = record->encode();
  // Only the SR flag is 1
  assertEqual(encoded[0], 0b00010000);
  // Type length is 0
  assertEqual(encoded[1], 0);
  // Payload length is 0
  assertEqual(encoded[2], 0);
  delete[] encoded;
  delete record;
}

test(record_encode_short)
{
  ArduinoNDEF::Record::Record record(
      ArduinoNDEF::Record::TNF_WELL_KNOWN,
      *(new ArduinoNDEF::Field::Type(ArduinoNDEF::Field::Type::RTD_TEXT)),
      *(new ArduinoNDEF::Field::Payload("Hello")),
      true,
      true,
      *(new ArduinoNDEF::Field::Id())
  );
  assertEqual(record.get_encoded_size(), static_cast<uint32_t>(9));
  uint8_t *encoded = record.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
  assertEqual(encoded[0], 0b11010001);
  // Type length is 1
  assertEqual(encoded[1], 1);
  // Payload length is 5
  assertEqual(encoded[2], 5);
  // Type is RTD_TEXT
  assertEqual(encoded[3], static_cast<uint8_t>('T'));
  // Payload is "Hello"
  assertEqual(encoded[4], static_cast<uint8_t>('H'));
  assertEqual(encoded[5], static_cast<uint8_t>('e'));
  assertEqual(encoded[6], static_cast<uint8_t>('l'));
  assertEqual(encoded[7], static_cast<uint8_t>('l'));
  assertEqual(encoded[8], static_cast<uint8_t>('o'));
  delete[] encoded;
}

test(record_decode_short)
{
  uint8_t encoded[] = {
      0b11010001,                         // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
      1,                                  // Type length is 1
      5,                                  // Payload length is 5
      ArduinoNDEF::Field::Type::RTD_TEXT, // Type is RTD_TEXT
      'H',
      'e',
      'l',
      'l',
      'o'
  };
  auto record = ArduinoNDEF::Record::decode(*encoded, 9);
  assertTrue(record->is_message_begin);
  assertTrue(record->is_message_end);
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_WELL_KNOWN);
  assertEqual(record->type().length(), 1);
  assertEqual(record->type().data()[0], static_cast<uint8_t>('T'));
  assertEqual(record->payload().length(), static_cast<uint32_t>(5));
  const uint8_t *payload = record->payload().data();
  assertEqual(payload[0], static_cast<uint8_t>('H'));
  assertEqual(payload[1], static_cast<uint8_t>('e'));
  assertEqual(payload[2], static_cast<uint8_t>('l'));
  assertEqual(payload[3], static_cast<uint8_t>('l'));
  assertEqual(payload[4], static_cast<uint8_t>('o'));
  delete record;
}

const char LONG_TEXT[] =
    "For instance, on the planet Earth, man had always assumed that he was more "
    "intelligent than dolphins because he had achieved so much - the wheel, New York, "
    "wars and so on - whilst all the dolphins had ever done was muck about in the "
    "water having a good time. But conversely, the dolphins had always believed that "
    "they were far more intelligent than man - for precisely the same reasons.";

test(record_encode_long_text)
{
  ArduinoNDEF::Record::Record record(
      ArduinoNDEF::Record::TNF_WELL_KNOWN,
      *(new ArduinoNDEF::Field::Type(ArduinoNDEF::Field::Type::RTD_TEXT)),
      *(new ArduinoNDEF::Field::Payload(LONG_TEXT)),
      false,
      false,
      *(new ArduinoNDEF::Field::Id())
  );
  assertEqual(record.get_encoded_size(), static_cast<uint32_t>(393));
  uint8_t *encoded = record.encode();
  // SR flag is 0, TNF is 0x01
  assertEqual(encoded[0], 0b00000001);
  // Type length is 1
  assertEqual(encoded[1], 1);
  // Payload length is 386
  assertEqual(encoded[2], 0);
  assertEqual(encoded[3], 0);
  assertEqual(encoded[4], 1);
  assertEqual(encoded[5], 130);
  // Type is RTD_TEXT
  assertEqual(encoded[6], static_cast<uint8_t>('T'));
  // Check payload
  for (uint32_t i = 0; i < 386; i++)
    assertEqual(encoded[7 + i], static_cast<uint8_t>(LONG_TEXT[i]));
  delete[] encoded;
}

test(record_decode_long_text)
{
  auto encoded = new uint8_t[393]{// MB=0, ME=0, CF=0, SR=0, IL=0, TNF=0x01 (well known)
                                  0x1,
                                  // Type length is 1
                                  1,
                                  // Payload length is 386
                                  0,
                                  0,
                                  1,
                                  130,
                                  // Type is RTD_TEXT
                                  'T'
  };
  // Copy reference text
  memcpy(encoded + 7, LONG_TEXT, 386);
  // Check decoding
  auto record = ArduinoNDEF::Record::decode(*encoded, 393);
  delete[] encoded;
  assertFalse(record->is_message_begin);
  assertFalse(record->is_message_end);
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_WELL_KNOWN);
  assertEqual(record->type().length(), 1);
  assertEqual(record->type().data()[0], static_cast<uint8_t>('T'));
  assertEqual(record->payload().length(), static_cast<uint32_t>(386));
  const uint8_t *payload = record->payload().data();
  for (uint32_t i = 0; i < 386; i++)
    assertEqual(payload[i], static_cast<uint8_t>(LONG_TEXT[i]));
  delete record;
}

test(create_text_record)
{
  auto record = ArduinoNDEF::Record::Text::create("Hello", "en");
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_WELL_KNOWN);
  assertEqual(record->type().length(), 1);
  assertEqual(record->type().data()[0], ArduinoNDEF::Field::Type::RTD_TEXT);
  assertEqual(record->payload().length(), static_cast<uint32_t>(8));
  const uint8_t *payload = record->payload().data();
  assertEqual(payload[0], 0x02);
  assertEqual(payload[1], static_cast<uint8_t>('e'));
  assertEqual(payload[2], static_cast<uint8_t>('n'));
  assertEqual(payload[3], static_cast<uint8_t>('H'));
  assertEqual(payload[4], static_cast<uint8_t>('e'));
  assertEqual(payload[5], static_cast<uint8_t>('l'));
  assertEqual(payload[6], static_cast<uint8_t>('l'));
  assertEqual(payload[7], static_cast<uint8_t>('o'));
  delete record;
}

test(create_uri_record)
{
  auto record = ArduinoNDEF::Record::Uri::create("https://hackaday.com");
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_WELL_KNOWN);
  assertEqual(record->type().length(), 1);
  assertEqual(record->type().data()[0], ArduinoNDEF::Field::Type::RTD_URI);
  assertEqual(record->payload().length(), static_cast<uint32_t>(13));
  const uint8_t *payload = record->payload().data();
  assertEqual(payload[0], 0x04);
  assertEqual(payload[1], static_cast<uint8_t>('h'));
  assertEqual(payload[2], static_cast<uint8_t>('a'));
  assertEqual(payload[3], static_cast<uint8_t>('c'));
  assertEqual(payload[4], static_cast<uint8_t>('k'));
  assertEqual(payload[5], static_cast<uint8_t>('a'));
  assertEqual(payload[6], static_cast<uint8_t>('d'));
  assertEqual(payload[7], static_cast<uint8_t>('a'));
  assertEqual(payload[8], static_cast<uint8_t>('y'));
  assertEqual(payload[9], static_cast<uint8_t>('.'));
  assertEqual(payload[10], static_cast<uint8_t>('c'));
  assertEqual(payload[11], static_cast<uint8_t>('o'));
  assertEqual(payload[12], static_cast<uint8_t>('m'));
  delete record;
}

test(create_mime_media_record)
{
  auto record = ArduinoNDEF::Record::MimeMedia::create("text/plain", "Hello");
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_MIME_MEDIA);
  assertEqual(record->type().length(), 10);
  assertEqual(record->type().data()[0], static_cast<uint8_t>('t'));
  assertEqual(record->type().data()[1], static_cast<uint8_t>('e'));
  assertEqual(record->type().data()[2], static_cast<uint8_t>('x'));
  assertEqual(record->type().data()[3], static_cast<uint8_t>('t'));
  assertEqual(record->type().data()[4], static_cast<uint8_t>('/'));
  assertEqual(record->type().data()[5], static_cast<uint8_t>('p'));
  assertEqual(record->type().data()[6], static_cast<uint8_t>('l'));
  assertEqual(record->type().data()[7], static_cast<uint8_t>('a'));
  assertEqual(record->type().data()[8], static_cast<uint8_t>('i'));
  assertEqual(record->type().data()[9], static_cast<uint8_t>('n'));
  assertEqual(record->payload().length(), static_cast<uint32_t>(5));
  const uint8_t *payload = record->payload().data();
  assertEqual(payload[0], static_cast<uint8_t>('H'));
  assertEqual(payload[1], static_cast<uint8_t>('e'));
  assertEqual(payload[2], static_cast<uint8_t>('l'));
  assertEqual(payload[3], static_cast<uint8_t>('l'));
  assertEqual(payload[4], static_cast<uint8_t>('o'));
  delete record;
}

test(create_external_type_record)
{
  auto record =
      ArduinoNDEF::Record::ExternalType::create("com.example", "custom-type", "Hello");
  assertEqual(record->type_name_format, ArduinoNDEF::Record::TNF_EXTERNAL_TYPE);
  assertEqual(record->type().length(), 35);
  assertEqual(record->type().data()[0], static_cast<uint8_t>('u'));
  assertEqual(record->type().data()[1], static_cast<uint8_t>('r'));
  assertEqual(record->type().data()[2], static_cast<uint8_t>('n'));
  assertEqual(record->type().data()[3], static_cast<uint8_t>(':'));
  assertEqual(record->type().data()[4], static_cast<uint8_t>('n'));
  assertEqual(record->type().data()[5], static_cast<uint8_t>('f'));
  assertEqual(record->type().data()[6], static_cast<uint8_t>('c'));
  assertEqual(record->type().data()[7], static_cast<uint8_t>(':'));
  assertEqual(record->type().data()[8], static_cast<uint8_t>('e'));
  assertEqual(record->type().data()[9], static_cast<uint8_t>('x'));
  assertEqual(record->type().data()[10], static_cast<uint8_t>('t'));
  assertEqual(record->type().data()[11], static_cast<uint8_t>(':'));
  assertEqual(record->type().data()[12], static_cast<uint8_t>('c'));
  assertEqual(record->type().data()[13], static_cast<uint8_t>('o'));
  assertEqual(record->type().data()[14], static_cast<uint8_t>('m'));
  assertEqual(record->type().data()[15], static_cast<uint8_t>('.'));
  assertEqual(record->type().data()[16], static_cast<uint8_t>('e'));
  assertEqual(record->type().data()[17], static_cast<uint8_t>('x'));
  assertEqual(record->type().data()[18], static_cast<uint8_t>('a'));
  assertEqual(record->type().data()[19], static_cast<uint8_t>('m'));
  assertEqual(record->type().data()[20], static_cast<uint8_t>('p'));
  assertEqual(record->type().data()[21], static_cast<uint8_t>('l'));
  assertEqual(record->type().data()[22], static_cast<uint8_t>('e'));
  assertEqual(record->type().data()[23], static_cast<uint8_t>(':'));
  assertEqual(record->type().data()[24], static_cast<uint8_t>('c'));
  assertEqual(record->type().data()[25], static_cast<uint8_t>('u'));
  assertEqual(record->type().data()[26], static_cast<uint8_t>('s'));
  assertEqual(record->type().data()[27], static_cast<uint8_t>('t'));
  assertEqual(record->type().data()[28], static_cast<uint8_t>('o'));
  assertEqual(record->type().data()[29], static_cast<uint8_t>('m'));
  assertEqual(record->type().data()[30], static_cast<uint8_t>('-'));
  assertEqual(record->type().data()[31], static_cast<uint8_t>('t'));
  assertEqual(record->type().data()[32], static_cast<uint8_t>('y'));
  assertEqual(record->type().data()[33], static_cast<uint8_t>('p'));
  assertEqual(record->type().data()[34], static_cast<uint8_t>('e'));
  assertEqual(record->payload().length(), static_cast<uint32_t>(5));
  const uint8_t *payload = record->payload().data();
  assertEqual(payload[0], static_cast<uint8_t>('H'));
  assertEqual(payload[1], static_cast<uint8_t>('e'));
  assertEqual(payload[2], static_cast<uint8_t>('l'));
  assertEqual(payload[3], static_cast<uint8_t>('l'));
  assertEqual(payload[4], static_cast<uint8_t>('o'));
  delete record;
}

test(get_encoded_ndef_record_size__invalid)
{
  const uint8_t encoded[] = {0, 0, 0, 0};
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(nullptr, 42),
      static_cast<uint32_t>(0)
  );
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 0),
      static_cast<uint32_t>(0)
  );
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 1),
      static_cast<uint32_t>(0)
  );
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 2),
      static_cast<uint32_t>(0)
  );
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 3),
      static_cast<uint32_t>(0)
  );
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 4),
      static_cast<uint32_t>(0)
  );
}

test(get_encoded_ndef_record_size__empty)
{
  const uint8_t encoded[] = {0b00010000, 0, 0};
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 3),
      static_cast<uint32_t>(3)
  );
  // Ensure additional bytes are ignored
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 6),
      static_cast<uint32_t>(3)
  );
}

test(get_encoded_ndef_record_size__uri)
{
  const uint8_t encoded[] = {
      0b11010001,                        // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
      1,                                 // Type length is 1
      13,                                // Payload length is 13
      ArduinoNDEF::Field::Type::RTD_URI, // Type is RTD_URI
      0x04,                              // URI prefix is https://
      'h',
      'a',
      'c',
      'k',
      'a',
      'd',
      'a',
      'y',
      '.',
      'c',
      'o',
      'm'
  };
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 17),
      static_cast<uint32_t>(17)
  );
  // Ensure additional bytes are ignored and we don't touch non allocated memory
  // (valgrind will make this fail if we do)
  assertEqual(
      ArduinoNDEF::Record::get_encoded_ndef_record_size(encoded, 42),
      static_cast<uint32_t>(17)
  );
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