#include "uri.hpp"

#include "macros.hpp"

#include <string.h>

/**
 * @brief NDEF URI identifier codes
 * @ref NFCForum-TS-RTD_URI_1.0.pdf - Table 3. Abbreviation Table
 */
#define NDEF_URI_CODE_IDENTIFIER_COUNT 36
struct NdefUriPrefix
{
    uint8_t code;
    const char *prefix;
};
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

NdefUriPayload::NdefUriPayload(const char *uri)
{
  size_t uri_length = strlen(uri);
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

  // XXX: we truncate long URLs, not great but it's better than nothing
  uint8_t length = min(uri_length + 1, 0xff); // Add 1 for the identifier code byte
  uint8_t *data = new uint8_t[length];

  if (data == nullptr)
    return;

  data[0] = code;
  memcpy(data + 1, &uri[uri_offset], uri_length);

  _data = data;
  _length = length;
}

bool NdefUriPayload::is_valid() const
{
  // Check if the data is valid
  if (_data == nullptr || _length < 2)
    return false;

  // Check if the identifier code is valid
  uint8_t code = _data[0];

  for (uint8_t i = 0; i < NDEF_URI_CODE_IDENTIFIER_COUNT; i++)
    if (code == NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].code)
      return true;

  return false;
}

uint8_t NdefUriPayload::code() const
{
  if (!is_valid())
    return 0;

  return _data[0];
}

const char *NdefUriPayload::uri() const
{
  if (!is_valid())
    return nullptr;

  const char *prefix = nullptr;
  uint8_t prefix_length = 0;
  char *uri = nullptr;
  size_t uri_length = 0;
  uint8_t code_ = code();

  for (uint8_t i = 0; i < NDEF_URI_CODE_IDENTIFIER_COUNT; i++)
    if (code_ == NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].code)
    {
      prefix = NDEF_URI_IDENTIFIER_CODE_IDENTIFIERS[i].prefix;
      prefix_length = strlen(prefix);
      break;
    }

  // We know that prefix *IS* populated because we called is_valid() earlier

  // Implicitely includes null terminator byte because we do not subtract the identifier
  // code byte
  uri_length = prefix_length + _length;
  uri = new char[uri_length];

  if (uri == nullptr)
    return nullptr;

  memcpy(uri, prefix, prefix_length);
  memcpy(uri + prefix_length, _data + 1, _length - 1);
  uri[uri_length - 1] = '\0';

  return uri;
}