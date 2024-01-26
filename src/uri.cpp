#include "uri.hpp"

#include "macros.hpp"

#include <stdlib.h>
#include <string.h>

static const NdefUriPrefix
    NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[NDEF_URI_CODE_IDENTIFIER_COUNT] = {
        {0x00,                           ""},
        {0x01,                "http://www."},
        {0x02,               "https://www."},
        {0x03,                    "http://"},
        {0x04,                   "https://"},
        {0x05,                       "tel:"},
        {0x06,                    "mailto:"},
        {0x07, "ftp://anonymous:anonymous@"},
        {0x08,                 "ftp://ftp."},
        {0x09,                    "ftps://"},
        {0x0A,                    "sftp://"},
        {0x0B,                     "smb://"},
        {0x0C,                     "nfs://"},
        {0x0D,                     "ftp://"},
        {0x0E,                     "dav://"},
        {0x0F,                      "news:"},
        {0x10,                  "telnet://"},
        {0x11,                      "imap:"},
        {0x12,                    "rtsp://"},
        {0x13,                       "urn:"},
        {0x14,                       "pop:"},
        {0x15,                       "sip:"},
        {0x16,                      "sips:"},
        {0x17,                      "tftp:"},
        {0x18,                   "btspp://"},
        {0x19,                 "btl2cap://"},
        {0x1A,                  "btgoep://"},
        {0x1B,                 "tcpobex://"},
        {0x1C,                "irdaobex://"},
        {0x1D,                    "file://"},
        {0x1E,                "urn:epc:id:"},
        {0x1F,               "urn:epc:tag:"},
        {0x20,               "urn:epc:pat:"},
        {0x21,               "urn:epc:raw:"},
        {0x22,                   "urn:epc:"},
        {0x23,                   "urn:nfc:"},
};

void free_ndef_uri_payload(struct NdefUriPayload *payload)
{
  if (payload != nullptr)
  {
    free(payload->data);
    free(payload);
  }
}

struct NdefUriPayload *build_ndef_uri_payload(const char *uri)
{
  size_t uri_length = strlen(uri);

  if (uri_length > 0xff)
  {
    PRINTLN(F("build_ndef_uri_payload uri too long"));
    return nullptr;
  }

  const char *prefix;
  uint8_t prefix_length;
  uint8_t uri_offset = 0;
  uint8_t code = 0x00; // No prefix

  // Check if the URI starts with a known prefix
  for (uint8_t i = 1; i < NDEF_URI_CODE_IDENTIFIER_COUNT; i++)
  {
    prefix = NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].prefix;
    prefix_length = strlen(prefix);

    if (strncmp(uri, prefix, prefix_length) == 0)
    {
      code = NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].code;
      uri_length -= prefix_length;
      uri_offset = prefix_length;
      break;
    }
  }

  struct NdefUriPayload *payload =
      (struct NdefUriPayload *)malloc(sizeof(struct NdefUriPayload));

  if (payload == nullptr)
  {
    PRINTLN(F("build_ndef_uri_payload failed to allocate payload memory"));
    return nullptr;
  }

  payload->length = uri_length + 1; // Add 1 for the identifier code byte
  payload->data = (uint8_t *)malloc(payload->length);

  if (payload->data == nullptr)
  {
    PRINTLN(F("build_ndef_uri_payload failed to allocate payload data memory"));
    free(payload);
    return nullptr;
  }

  payload->data[0] = code;
  memcpy(payload->data + 1, &uri[uri_offset], uri_length);

  return payload;
}

char *build_uri_from_ndef_uri_payload(struct NdefUriPayload *payload)
{
  if (payload == nullptr)
  {
    PRINTLN(F("build_uri_from_ndef_uri_payload payload is null"));
    return nullptr;
  }

  if (payload->length < 2)
  {
    PRINTLN(F("build_uri_from_ndef_uri_payload payload length is too short"));
    return nullptr;
  }

  const char *prefix = nullptr;
  uint8_t prefix_length = 0;
  char *uri = nullptr;
  size_t uri_length = 0;
  uint8_t code = payload->data[0];

  for (uint8_t i = 0; i < NDEF_URI_CODE_IDENTIFIER_COUNT; i++)
  {
    if (code == NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].code)
    {
      prefix = NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].prefix;
      prefix_length = strlen(prefix);
      break;
    }
  }

  if (prefix == nullptr)
  {
    PRINT(F("build_uri_from_ndef_uri_payload unknown code: "));
    PRINTLN_DEC(code);
    return nullptr;
  }

  // Implicitely includes null terminator byte because we do not subtract the identifier
  // code byte
  uri_length = prefix_length + payload->length;
  uri = (char *)malloc(uri_length);

  if (uri == nullptr)
  {
    PRINTLN(F("build_uri_from_ndef_uri_payload failed to allocate uri memory"));
    return nullptr;
  }

  memcpy(uri, prefix, prefix_length);
  memcpy(uri + prefix_length, payload->data + 1, uri_length);
  uri[uri_length - 1] = '\0';

  return uri;
}