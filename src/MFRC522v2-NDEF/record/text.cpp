#include "text.hpp"

NdefTextRecord *NdefTextRecord::create(
    const char *text,
    const char *language_code,
    bool is_message_begin,
    bool is_message_end,
    const NdefRecordId &id
)
{
  // 1. Prepare header
  uint32_t language_code_length = strlen(language_code);

  if (language_code_length != 2 && language_code_length != 5)
    return nullptr;
  if (language_code_length == 5 && language_code[2] != '-')
    return nullptr;

  uint32_t header_length = 1 + language_code_length;
  uint8_t header[header_length];
  header[0] = language_code_length;
  memcpy(header + 1, language_code, language_code_length);

  // 2. Prepare payload
  uint32_t text_length = strlen(text);
  uint32_t payload_length = header_length + text_length;

  auto payload_data = new uint8_t[payload_length];
  if (payload_data == nullptr)
    return nullptr;

  memcpy(payload_data, header, header_length);
  memcpy(payload_data + header_length, text, text_length);

  auto type = new NdefRecordType(NdefRecordType::RTD_TEXT);
  auto payload = new NdefRecordPayload(
      payload_data,
      payload_length,
      NdefRecordPayload::OwnershipUnique
  );
  if (type == nullptr || payload == nullptr)
    return nullptr;

  return new NdefTextRecord(*type, *payload, is_message_begin, is_message_end, id);
}

NdefTextRecord *NdefTextRecord::create(const char *text, const char *language_code)
{
  auto id = new NdefRecordId();
  if (id == nullptr)
    return nullptr;
  return create(text, language_code, false, false, *id);
}

NdefTextRecord *NdefTextRecord::create(
    const char *text, const char *language_code, const NdefRecordId &id
)
{
  return create(text, language_code, false, false, id);
}

NdefTextRecord *NdefTextRecord::create(
    const char *text,
    const char *language_code,
    bool is_message_begin,
    bool is_message_end
)
{
  auto id = new NdefRecordId();
  if (id == nullptr)
    return nullptr;
  return create(text, language_code, is_message_begin, is_message_end, *id);
}