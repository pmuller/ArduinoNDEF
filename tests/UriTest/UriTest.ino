#line 2 "UriTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.hpp>

// NFCForum-TS-RTD_URI_1.0.pdf - A.1
test(build_ndef_uri_payload__simple)
{
  struct NdefUriPayload *payload = build_ndef_uri_payload("http://www.nfc.com");
  assertEqual(payload->length, (uint8_t)8);
  assertEqual(payload->data[0], (uint8_t)0x01); // http://www.
  assertEqual(payload->data[1], (uint8_t)'n');
  assertEqual(payload->data[2], (uint8_t)'f');
  assertEqual(payload->data[3], (uint8_t)'c');
  assertEqual(payload->data[4], (uint8_t)'.');
  assertEqual(payload->data[5], (uint8_t)'c');
  assertEqual(payload->data[6], (uint8_t)'o');
  assertEqual(payload->data[7], (uint8_t)'m');
  free_ndef_uri_payload(payload);
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.2
test(build_ndef_uri_payload__phone)
{
  struct NdefUriPayload *payload = build_ndef_uri_payload("tel:+35891234567");
  assertEqual(payload->length, (uint8_t)13);
  assertEqual(payload->data[0], (uint8_t)0x05); // tel:
  assertEqual(payload->data[1], (uint8_t)'+');
  assertEqual(payload->data[2], (uint8_t)'3');
  assertEqual(payload->data[3], (uint8_t)'5');
  assertEqual(payload->data[4], (uint8_t)'8');
  assertEqual(payload->data[5], (uint8_t)'9');
  assertEqual(payload->data[6], (uint8_t)'1');
  assertEqual(payload->data[7], (uint8_t)'2');
  assertEqual(payload->data[8], (uint8_t)'3');
  assertEqual(payload->data[9], (uint8_t)'4');
  assertEqual(payload->data[10], (uint8_t)'5');
  assertEqual(payload->data[11], (uint8_t)'6');
  assertEqual(payload->data[12], (uint8_t)'7');
  free_ndef_uri_payload(payload);
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.3
test(build_ndef_uri_payload__proprietary)
{
  struct NdefUriPayload *payload =
      build_ndef_uri_payload("mms://example.com/download.wmv");
  assertEqual(payload->length, (uint8_t)31);
  assertEqual(payload->data[0], (uint8_t)0x00); // No abbreviation
  assertEqual(payload->data[1], (uint8_t)'m');
  assertEqual(payload->data[2], (uint8_t)'m');
  assertEqual(payload->data[3], (uint8_t)'s');
  assertEqual(payload->data[4], (uint8_t)':');
  assertEqual(payload->data[5], (uint8_t)'/');
  assertEqual(payload->data[6], (uint8_t)'/');
  assertEqual(payload->data[7], (uint8_t)'e');
  assertEqual(payload->data[8], (uint8_t)'x');
  assertEqual(payload->data[9], (uint8_t)'a');
  assertEqual(payload->data[10], (uint8_t)'m');
  assertEqual(payload->data[11], (uint8_t)'p');
  assertEqual(payload->data[12], (uint8_t)'l');
  assertEqual(payload->data[13], (uint8_t)'e');
  assertEqual(payload->data[14], (uint8_t)'.');
  assertEqual(payload->data[15], (uint8_t)'c');
  assertEqual(payload->data[16], (uint8_t)'o');
  assertEqual(payload->data[17], (uint8_t)'m');
  assertEqual(payload->data[18], (uint8_t)'/');
  assertEqual(payload->data[19], (uint8_t)'d');
  assertEqual(payload->data[20], (uint8_t)'o');
  assertEqual(payload->data[21], (uint8_t)'w');
  assertEqual(payload->data[22], (uint8_t)'n');
  assertEqual(payload->data[23], (uint8_t)'l');
  assertEqual(payload->data[24], (uint8_t)'o');
  assertEqual(payload->data[25], (uint8_t)'a');
  assertEqual(payload->data[26], (uint8_t)'d');
  assertEqual(payload->data[27], (uint8_t)'.');
  assertEqual(payload->data[28], (uint8_t)'w');
  assertEqual(payload->data[29], (uint8_t)'m');
  assertEqual(payload->data[30], (uint8_t)'v');
  free_ndef_uri_payload(payload);
}

test(build_uri_from_ndef_uri_payload__simple)
{
  struct NdefUriPayload payload = {
      .data = (uint8_t *)"\1nfc.com",
      .length = (uint8_t)8,
  };
  char *uri = build_uri_from_ndef_uri_payload(&payload);
  assertEqual(uri, "http://www.nfc.com");
  free(uri);
}

test(build_uri_from_ndef_uri_payload__phone)
{
  struct NdefUriPayload payload = {
      .data = (uint8_t *)"\5+35891234567",
      .length = (uint8_t)13,
  };
  char *uri = build_uri_from_ndef_uri_payload(&payload);
  assertEqual(uri, "tel:+35891234567");
  free(uri);
}

test(build_uri_from_ndef_uri_payload__proprietary)
{
  struct NdefUriPayload payload = {
      .data = (uint8_t *)"\0mms://example.com/download.wmv",
      .length = (uint8_t)31,
  };
  char *uri = build_uri_from_ndef_uri_payload(&payload);
  assertEqual(uri, "mms://example.com/download.wmv");
  free(uri);
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