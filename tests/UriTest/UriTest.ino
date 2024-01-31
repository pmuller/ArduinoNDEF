#line 2 "UriTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF/uri.hpp>

// NFCForum-TS-RTD_URI_1.0.pdf - A.1
test(build_ndef_uri_payload__simple)
{
  ArduinoNDEF::NdefUriPayload payload("http://www.nfc.com");
  assertEqual(payload.length(), 8);
  assertEqual(payload.data()[0], 0x01); // http://www.
  assertEqual(payload.data()[1], static_cast<uint8_t>('n'));
  assertEqual(payload.data()[2], static_cast<uint8_t>('f'));
  assertEqual(payload.data()[3], static_cast<uint8_t>('c'));
  assertEqual(payload.data()[4], static_cast<uint8_t>('.'));
  assertEqual(payload.data()[5], static_cast<uint8_t>('c'));
  assertEqual(payload.data()[6], static_cast<uint8_t>('o'));
  assertEqual(payload.data()[7], static_cast<uint8_t>('m'));
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.2
test(build_ndef_uri_payload__phone)
{
  ArduinoNDEF::NdefUriPayload payload("tel:+35891234567");
  assertEqual(payload.length(), 13);
  assertEqual(payload.data()[0], 0x05); // tel:
  assertEqual(payload.data()[1], static_cast<uint8_t>('+'));
  assertEqual(payload.data()[2], static_cast<uint8_t>('3'));
  assertEqual(payload.data()[3], static_cast<uint8_t>('5'));
  assertEqual(payload.data()[4], static_cast<uint8_t>('8'));
  assertEqual(payload.data()[5], static_cast<uint8_t>('9'));
  assertEqual(payload.data()[6], static_cast<uint8_t>('1'));
  assertEqual(payload.data()[7], static_cast<uint8_t>('2'));
  assertEqual(payload.data()[8], static_cast<uint8_t>('3'));
  assertEqual(payload.data()[9], static_cast<uint8_t>('4'));
  assertEqual(payload.data()[10], static_cast<uint8_t>('5'));
  assertEqual(payload.data()[11], static_cast<uint8_t>('6'));
  assertEqual(payload.data()[12], static_cast<uint8_t>('7'));
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.3
test(build_ndef_uri_payload__proprietary)
{
  ArduinoNDEF::NdefUriPayload payload("mms://example.com/download.wmv");
  assertEqual(payload.length(), 31);
  assertEqual(payload.data()[0], 0x00); // No abbreviation
  assertEqual(payload.data()[1], static_cast<uint8_t>('m'));
  assertEqual(payload.data()[2], static_cast<uint8_t>('m'));
  assertEqual(payload.data()[3], static_cast<uint8_t>('s'));
  assertEqual(payload.data()[4], static_cast<uint8_t>(':'));
  assertEqual(payload.data()[5], static_cast<uint8_t>('/'));
  assertEqual(payload.data()[6], static_cast<uint8_t>('/'));
  assertEqual(payload.data()[7], static_cast<uint8_t>('e'));
  assertEqual(payload.data()[8], static_cast<uint8_t>('x'));
  assertEqual(payload.data()[9], static_cast<uint8_t>('a'));
  assertEqual(payload.data()[10], static_cast<uint8_t>('m'));
  assertEqual(payload.data()[11], static_cast<uint8_t>('p'));
  assertEqual(payload.data()[12], static_cast<uint8_t>('l'));
  assertEqual(payload.data()[13], static_cast<uint8_t>('e'));
  assertEqual(payload.data()[14], static_cast<uint8_t>('.'));
  assertEqual(payload.data()[15], static_cast<uint8_t>('c'));
  assertEqual(payload.data()[16], static_cast<uint8_t>('o'));
  assertEqual(payload.data()[17], static_cast<uint8_t>('m'));
  assertEqual(payload.data()[18], static_cast<uint8_t>('/'));
  assertEqual(payload.data()[19], static_cast<uint8_t>('d'));
  assertEqual(payload.data()[20], static_cast<uint8_t>('o'));
  assertEqual(payload.data()[21], static_cast<uint8_t>('w'));
  assertEqual(payload.data()[22], static_cast<uint8_t>('n'));
  assertEqual(payload.data()[23], static_cast<uint8_t>('l'));
  assertEqual(payload.data()[24], static_cast<uint8_t>('o'));
  assertEqual(payload.data()[25], static_cast<uint8_t>('a'));
  assertEqual(payload.data()[26], static_cast<uint8_t>('d'));
  assertEqual(payload.data()[27], static_cast<uint8_t>('.'));
  assertEqual(payload.data()[28], static_cast<uint8_t>('w'));
  assertEqual(payload.data()[29], static_cast<uint8_t>('m'));
  assertEqual(payload.data()[30], static_cast<uint8_t>('v'));
}

test(build_uri_from_ndef_uri_payload__simple)
{
  const uint8_t *data = new uint8_t[8]{0x01, 'n', 'f', 'c', '.', 'c', 'o', 'm'};
  const char *uri = ArduinoNDEF::NdefUriPayload(data, 8).uri();
  assertEqual(uri, "http://www.nfc.com");
  delete[] uri;
}

test(build_uri_from_ndef_uri_payload__phone)
{
  const uint8_t *data =
      new uint8_t[13]{0x05, '+', '3', '5', '8', '9', '1', '2', '3', '4', '5', '6', '7'};
  const char *uri = ArduinoNDEF::NdefUriPayload(data, 13).uri();
  assertEqual(uri, "tel:+35891234567");
  delete[] uri;
}

test(build_uri_from_ndef_uri_payload__proprietary)
{
  const uint8_t *data =
      new uint8_t[31]{0x00, 'm', 'm', 's', ':', '/', '/', 'e', 'x', 'a', 'm',
                      'p',  'l', 'e', '.', 'c', 'o', 'm', '/', 'd', 'o', 'w',
                      'n',  'l', 'o', 'a', 'd', '.', 'w', 'm', 'v'};
  const char *uri = ArduinoNDEF::NdefUriPayload(data, 31).uri();
  assertEqual(uri, "mms://example.com/download.wmv");
  delete[] uri;
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