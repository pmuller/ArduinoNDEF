#line 2 "MessageTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.h>

test(message_constructor)
{
  ArduinoNDEF::Message message;
  assertEqual(message.record_count(), 0);
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(0));
}

test(message_add_one_record)
{
  ArduinoNDEF::Message message;
  assertEqual(message.add_record(ArduinoNDEF::Record::Empty::create()), NDEF_SUCCESS);
  assertEqual(message.record_count(), 1);
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(3));
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[0], 0b11010000);
  assertEqual(encoded[1], 0);
  assertEqual(encoded[2], 0);
  delete[] encoded;
}

test(message_add_three_records)
{
  ArduinoNDEF::Message message;
  assertEqual(message.add_record(ArduinoNDEF::Record::Empty::create()), NDEF_SUCCESS);
  assertEqual(message.add_record(ArduinoNDEF::Record::Empty::create()), NDEF_SUCCESS);
  assertEqual(message.add_record(ArduinoNDEF::Record::Empty::create()), NDEF_SUCCESS);
  assertEqual(message.record_count(), 3);
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(9));
  uint8_t *encoded = message.encode();
  // MB=1, ME=0, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[0], 0b10010000);
  assertEqual(encoded[1], 0);
  assertEqual(encoded[2], 0);
  // MB=0, ME=0, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[3], 0b00010000);
  assertEqual(encoded[4], 0);
  assertEqual(encoded[5], 0);
  // MB=0, ME=1, CF=0, SR=1, IL=0, TNF=0x00
  assertEqual(encoded[6], 0b01010000);
  assertEqual(encoded[7], 0);
  assertEqual(encoded[8], 0);
  delete[] encoded;
}

test(message_add_mime_media_record)
{
  ArduinoNDEF::Message message;
  assertEqual(
      message.add_record(ArduinoNDEF::Record::MimeMedia::create(
          "text/plain",
          reinterpret_cast<const uint8_t *>("Hello"),
          5
      )),
      NDEF_SUCCESS
  );
  assertEqual(message.record_count(), static_cast<uint8_t>(1));
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(18));
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x02
  assertEqual(encoded[0], static_cast<uint8_t>(0b11010010));
  // Type length is 10
  assertEqual(encoded[1], static_cast<uint8_t>(10));
  // Payload length is 5
  assertEqual(encoded[2], static_cast<uint8_t>(5));
  // Type is "text/plain"
  assertEqual(encoded[3], static_cast<uint8_t>('t'));
  assertEqual(encoded[4], static_cast<uint8_t>('e'));
  assertEqual(encoded[5], static_cast<uint8_t>('x'));
  assertEqual(encoded[6], static_cast<uint8_t>('t'));
  assertEqual(encoded[7], static_cast<uint8_t>('/'));
  assertEqual(encoded[8], static_cast<uint8_t>('p'));
  assertEqual(encoded[9], static_cast<uint8_t>('l'));
  assertEqual(encoded[10], static_cast<uint8_t>('a'));
  assertEqual(encoded[11], static_cast<uint8_t>('i'));
  assertEqual(encoded[12], static_cast<uint8_t>('n'));
  // Payload is "Hello"
  assertEqual(encoded[13], static_cast<uint8_t>('H'));
  assertEqual(encoded[14], static_cast<uint8_t>('e'));
  assertEqual(encoded[15], static_cast<uint8_t>('l'));
  assertEqual(encoded[16], static_cast<uint8_t>('l'));
  assertEqual(encoded[17], static_cast<uint8_t>('o'));
  delete[] encoded;
}

test(message_add_uri_record)
{
  ArduinoNDEF::Message message;
  assertEqual(
      message.add_record(ArduinoNDEF::Record::Uri::create("https://www.google.com")),
      NDEF_SUCCESS
  );
  assertEqual(message.record_count(), static_cast<uint8_t>(1));
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(15));
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x01
  assertEqual(encoded[0], static_cast<uint8_t>(0b11010001));
  // Type length is 1
  assertEqual(encoded[1], static_cast<uint8_t>(1));
  // Payload length is 14
  assertEqual(encoded[2], static_cast<uint8_t>(11));
  // Type is RTD_URI
  assertEqual(encoded[3], static_cast<uint8_t>(ArduinoNDEF::Field::Type::RTD_URI));
  // Payload is "\2google.com"
  assertEqual(encoded[4], static_cast<uint8_t>(0x02));
  assertEqual(encoded[5], static_cast<uint8_t>('g'));
  assertEqual(encoded[6], static_cast<uint8_t>('o'));
  assertEqual(encoded[7], static_cast<uint8_t>('o'));
  assertEqual(encoded[8], static_cast<uint8_t>('g'));
  assertEqual(encoded[9], static_cast<uint8_t>('l'));
  assertEqual(encoded[10], static_cast<uint8_t>('e'));
  assertEqual(encoded[11], static_cast<uint8_t>('.'));
  assertEqual(encoded[12], static_cast<uint8_t>('c'));
  assertEqual(encoded[13], static_cast<uint8_t>('o'));
  assertEqual(encoded[14], static_cast<uint8_t>('m'));
  delete[] encoded;
}

test(message_add_external_type_record)
{
  ArduinoNDEF::Message message;
  assertEqual(
      message.add_record(ArduinoNDEF::Record::ExternalType::create(
          "com.example",
          "externalType",
          reinterpret_cast<const uint8_t *>("Hello"),
          5
      )),
      NDEF_SUCCESS
  );
  assertEqual(message.record_count(), static_cast<uint8_t>(1));
  assertEqual(message.get_encoded_size(), static_cast<uint32_t>(44));
  uint8_t *encoded = message.encode();
  // MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x04 (external)
  assertEqual(encoded[0], static_cast<uint8_t>(0b11010100));
  // Type length is 11
  assertEqual(encoded[1], static_cast<uint8_t>(36));
  // Payload length is 5
  assertEqual(encoded[2], static_cast<uint8_t>(5));
  // Type is "com.example:externalType"
  assertEqual(encoded[3], static_cast<uint8_t>('u'));
  assertEqual(encoded[4], static_cast<uint8_t>('r'));
  assertEqual(encoded[5], static_cast<uint8_t>('n'));
  assertEqual(encoded[6], static_cast<uint8_t>(':'));
  assertEqual(encoded[7], static_cast<uint8_t>('n'));
  assertEqual(encoded[8], static_cast<uint8_t>('f'));
  assertEqual(encoded[9], static_cast<uint8_t>('c'));
  assertEqual(encoded[10], static_cast<uint8_t>(':'));
  assertEqual(encoded[11], static_cast<uint8_t>('e'));
  assertEqual(encoded[12], static_cast<uint8_t>('x'));
  assertEqual(encoded[13], static_cast<uint8_t>('t'));
  assertEqual(encoded[14], static_cast<uint8_t>(':'));
  assertEqual(encoded[15], static_cast<uint8_t>('c'));
  assertEqual(encoded[16], static_cast<uint8_t>('o'));
  assertEqual(encoded[17], static_cast<uint8_t>('m'));
  assertEqual(encoded[18], static_cast<uint8_t>('.'));
  assertEqual(encoded[19], static_cast<uint8_t>('e'));
  assertEqual(encoded[20], static_cast<uint8_t>('x'));
  assertEqual(encoded[21], static_cast<uint8_t>('a'));
  assertEqual(encoded[22], static_cast<uint8_t>('m'));
  assertEqual(encoded[23], static_cast<uint8_t>('p'));
  assertEqual(encoded[24], static_cast<uint8_t>('l'));
  assertEqual(encoded[25], static_cast<uint8_t>('e'));
  assertEqual(encoded[26], static_cast<uint8_t>(':'));
  assertEqual(encoded[27], static_cast<uint8_t>('e'));
  assertEqual(encoded[28], static_cast<uint8_t>('x'));
  assertEqual(encoded[29], static_cast<uint8_t>('t'));
  assertEqual(encoded[30], static_cast<uint8_t>('e'));
  assertEqual(encoded[31], static_cast<uint8_t>('r'));
  assertEqual(encoded[32], static_cast<uint8_t>('n'));
  assertEqual(encoded[33], static_cast<uint8_t>('a'));
  assertEqual(encoded[34], static_cast<uint8_t>('l'));
  assertEqual(encoded[35], static_cast<uint8_t>('T'));
  assertEqual(encoded[36], static_cast<uint8_t>('y'));
  assertEqual(encoded[37], static_cast<uint8_t>('p'));
  assertEqual(encoded[38], static_cast<uint8_t>('e'));
  // Payload is "Hello"
  assertEqual(encoded[39], static_cast<uint8_t>('H'));
  assertEqual(encoded[40], static_cast<uint8_t>('e'));
  assertEqual(encoded[41], static_cast<uint8_t>('l'));
  assertEqual(encoded[42], static_cast<uint8_t>('l'));
  assertEqual(encoded[43], static_cast<uint8_t>('o'));
  delete[] encoded;
}

test(decode__1)
{
  const uint8_t encoded_message[] = {// MB=1, ME=1, CF=0, SR=1, IL=0, TNF=0x00 (empty)
                                     0b11010000,
                                     // Type length is 0
                                     0,
                                     // Payload length is 0
                                     0
  };
  ArduinoNDEF::Message *message = ArduinoNDEF::Message::decode(encoded_message, 3);
  assertEqual(message->record_count(), static_cast<uint8_t>(1));
  assertEqual(message->get_encoded_size(), static_cast<uint32_t>(3));
  auto empty_record = ArduinoNDEF::Record::Empty::create(
      /* is_message_begin */ true,
      /* is_message_end */ true
  );
  assertTrue(*message->record(0) == *empty_record);
  delete message;
  delete empty_record;
}

test(decode__2)
{
  const uint8_t encoded_message[] = {// MB=1, ME=0, CF=0, SR=1, IL=0, TNF=0x02 (MIME)
                                     0b10010010,
                                     // Type length is 10
                                     10,
                                     // Payload length is 5
                                     5,
                                     // Type is "text/plain"
                                     't',
                                     'e',
                                     'x',
                                     't',
                                     '/',
                                     'p',
                                     'l',
                                     'a',
                                     'i',
                                     'n',
                                     // Payload is "Hello"
                                     'H',
                                     'e',
                                     'l',
                                     'l',
                                     'o',
                                     // 2nd message is empty
                                     // MB=0, ME=1, CF=0, SR=1, IL=0, TNF=0x00 (empty)
                                     0b01010000,
                                     // Type length is 0
                                     0,
                                     // Payload length is 0
                                     0
  };
  ArduinoNDEF::Message *message =
      ArduinoNDEF::Message::decode(encoded_message, sizeof(encoded_message));
  assertEqual(message->record_count(), static_cast<uint8_t>(2));
  assertEqual(message->get_encoded_size(), static_cast<uint32_t>(21));
  auto mime_record = ArduinoNDEF::Record::MimeMedia::create(
      "text/plain",
      "Hello",
      /* is_message_begin */ true,
      /* is_message_end */ false
  );
  assertTrue(*message->record(0) == *mime_record);
  auto empty_record = ArduinoNDEF::Record::Empty::create(
      /* is_message_begin */ false,
      /* is_message_end */ true
  );
  assertTrue(*message->record(1) == *empty_record);
  delete message;
  delete mime_record;
  delete empty_record;
}

test(decode__3)
{
  const uint8_t encoded_message[] =
      {0b10010000, 0, 0, 0b00010000, 0, 0, 0b01010000, 0, 0};
  ArduinoNDEF::Message *message =
      ArduinoNDEF::Message::decode(encoded_message, sizeof(encoded_message));
  assertEqual(message->record_count(), static_cast<uint8_t>(3));
  assertEqual(message->get_encoded_size(), static_cast<uint32_t>(9));
  auto empty_record_begin = ArduinoNDEF::Record::Empty::create(
      /* is_message_begin */ true,
      /* is_message_end */ false
  );
  auto empty_record_2nd = ArduinoNDEF::Record::Empty::create(
      /* is_message_begin */ false,
      /* is_message_end */ false
  );
  auto empty_record_end = ArduinoNDEF::Record::Empty::create(
      /* is_message_begin */ false,
      /* is_message_end */ true
  );
  assertTrue(*message->record(0) == *empty_record_begin);
  assertTrue(*message->record(1) == *empty_record_2nd);
  assertTrue(*message->record(2) == *empty_record_end);
  delete message;
  delete empty_record_begin;
  delete empty_record_2nd;
  delete empty_record_end;
}

test(count_ndef_message_records__2)
{
  const uint8_t encoded_message[] = {// MB=1, ME=0, CF=0, SR=1, IL=0, TNF=0x02 (MIME)
                                     0b10010010,
                                     // Type length is 10
                                     10,
                                     // Payload length is 5
                                     5,
                                     // Type is "text/plain"
                                     't',
                                     'e',
                                     'x',
                                     't',
                                     '/',
                                     'p',
                                     'l',
                                     'a',
                                     'i',
                                     'n',
                                     // Payload is "Hello"
                                     'H',
                                     'e',
                                     'l',
                                     'l',
                                     'o',
                                     // 2nd message is empty
                                     // MB=0, ME=1, CF=0, SR=1, IL=0, TNF=0x00 (empty)
                                     0b01010000,
                                     // Type length is 0
                                     0,
                                     // Payload length is 0
                                     0
  };
  assertEqual(
      ArduinoNDEF::count_ndef_message_records(encoded_message, sizeof(encoded_message)),
      static_cast<uint8_t>(2)
  );
}

test(count_ndef_message_records__3_empty)
{
  const uint8_t encoded_message[] =
      {0b10010000, 0, 0, 0b00010000, 0, 0, 0b01010000, 0, 0};
  assertEqual(
      ArduinoNDEF::count_ndef_message_records(encoded_message, sizeof(encoded_message)),
      static_cast<uint8_t>(3)
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