#line 2 "UriTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF/uri.hpp>

// NFCForum-TS-RTD_URI_1.0.pdf - A.1
test(build_ndef_uri_payload__simple)
{
  NdefUriPayload payload("http://www.nfc.com");
  assertEqual(payload.get_length(), (uint8_t)8);
  const uint8_t *data = payload.get_data();
  assertEqual(data[0], (uint8_t)0x01); // http://www.
  assertEqual(data[1], (uint8_t)'n');
  assertEqual(data[2], (uint8_t)'f');
  assertEqual(data[3], (uint8_t)'c');
  assertEqual(data[4], (uint8_t)'.');
  assertEqual(data[5], (uint8_t)'c');
  assertEqual(data[6], (uint8_t)'o');
  assertEqual(data[7], (uint8_t)'m');
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.2
test(build_ndef_uri_payload__phone)
{
  NdefUriPayload payload("tel:+35891234567");
  assertEqual(payload.get_length(), (uint8_t)13);
  const uint8_t *data = payload.get_data();
  assertEqual(data[0], (uint8_t)0x05); // tel:
  assertEqual(data[1], (uint8_t)'+');
  assertEqual(data[2], (uint8_t)'3');
  assertEqual(data[3], (uint8_t)'5');
  assertEqual(data[4], (uint8_t)'8');
  assertEqual(data[5], (uint8_t)'9');
  assertEqual(data[6], (uint8_t)'1');
  assertEqual(data[7], (uint8_t)'2');
  assertEqual(data[8], (uint8_t)'3');
  assertEqual(data[9], (uint8_t)'4');
  assertEqual(data[10], (uint8_t)'5');
  assertEqual(data[11], (uint8_t)'6');
  assertEqual(data[12], (uint8_t)'7');
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.3
test(build_ndef_uri_payload__proprietary)
{
  NdefUriPayload payload("mms://example.com/download.wmv");
  assertEqual(payload.get_length(), (uint8_t)31);
  const uint8_t *data = payload.get_data();
  assertEqual(data[0], (uint8_t)0x00); // No abbreviation
  assertEqual(data[1], (uint8_t)'m');
  assertEqual(data[2], (uint8_t)'m');
  assertEqual(data[3], (uint8_t)'s');
  assertEqual(data[4], (uint8_t)':');
  assertEqual(data[5], (uint8_t)'/');
  assertEqual(data[6], (uint8_t)'/');
  assertEqual(data[7], (uint8_t)'e');
  assertEqual(data[8], (uint8_t)'x');
  assertEqual(data[9], (uint8_t)'a');
  assertEqual(data[10], (uint8_t)'m');
  assertEqual(data[11], (uint8_t)'p');
  assertEqual(data[12], (uint8_t)'l');
  assertEqual(data[13], (uint8_t)'e');
  assertEqual(data[14], (uint8_t)'.');
  assertEqual(data[15], (uint8_t)'c');
  assertEqual(data[16], (uint8_t)'o');
  assertEqual(data[17], (uint8_t)'m');
  assertEqual(data[18], (uint8_t)'/');
  assertEqual(data[19], (uint8_t)'d');
  assertEqual(data[20], (uint8_t)'o');
  assertEqual(data[21], (uint8_t)'w');
  assertEqual(data[22], (uint8_t)'n');
  assertEqual(data[23], (uint8_t)'l');
  assertEqual(data[24], (uint8_t)'o');
  assertEqual(data[25], (uint8_t)'a');
  assertEqual(data[26], (uint8_t)'d');
  assertEqual(data[27], (uint8_t)'.');
  assertEqual(data[28], (uint8_t)'w');
  assertEqual(data[29], (uint8_t)'m');
  assertEqual(data[30], (uint8_t)'v');
}

test(build_uri_from_ndef_uri_payload__simple)
{
  const uint8_t *data = new uint8_t[8]{0x01, 'n', 'f', 'c', '.', 'c', 'o', 'm'};
  const char *uri = NdefUriPayload(data, (uint8_t)8).get_uri();
  assertEqual(uri, "http://www.nfc.com");
  delete[] uri;
}

test(build_uri_from_ndef_uri_payload__phone)
{
  const uint8_t *data =
      new uint8_t[13]{0x05, '+', '3', '5', '8', '9', '1', '2', '3', '4', '5', '6', '7'};
  const char *uri = NdefUriPayload(data, (uint8_t)13).get_uri();
  assertEqual(uri, "tel:+35891234567");
  delete[] uri;
}

test(build_uri_from_ndef_uri_payload__proprietary)
{
  const uint8_t *data =
      new uint8_t[31]{0x00, 'm', 'm', 's', ':', '/', '/', 'e', 'x', 'a', 'm',
                      'p',  'l', 'e', '.', 'c', 'o', 'm', '/', 'd', 'o', 'w',
                      'n',  'l', 'o', 'a', 'd', '.', 'w', 'm', 'v'};
  const char *uri = NdefUriPayload(data, (uint8_t)31).get_uri();
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