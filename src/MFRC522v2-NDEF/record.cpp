#include "record.hpp"

#include "constants.hpp"
#include "macros.hpp"
#include "uri.hpp"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *NdefRecord::encode()
{
  auto result = new uint8_t[get_encoded_size()];
  auto result_ptr = result;
  if (result == nullptr)
    return nullptr;

  bool is_short_record = _payload.length() <= 0xff;

  // Build header
  *result_ptr++ = 0 | (is_message_begin ? NDEF_RECORD_HEADER_MB_FLAG_MASK : 0) |
                  (is_message_end ? NDEF_RECORD_HEADER_ME_FLAG_MASK : 0) |
                  (is_short_record ? NDEF_RECORD_HEADER_SR_FLAG_MASK : 0) |
                  (_id.length() > 0 ? NDEF_RECORD_HEADER_IL_FLAG_MASK : 0) |
                  type_name_format;
  *result_ptr++ = _type.length();

  if (is_short_record)
    *result_ptr++ = _payload.length();
  else
  {
    *result_ptr++ = (_payload.length() >> 24) & 0xFF;
    *result_ptr++ = (_payload.length() >> 16) & 0xFF;
    *result_ptr++ = (_payload.length() >> 8) & 0xFF;
    *result_ptr++ = _payload.length() & 0xFF;
  }

  if (_id.length() > 0)
    *result_ptr++ = _id.length();

  memcpy(result_ptr, _type.data(), _type.length());
  result_ptr += _type.length();

  if (_id.length() > 0)
  {
    memcpy(result_ptr, _id.data(), _id.length());
    result_ptr += _id.length();
  }

  memcpy(result_ptr, _payload.data(), _payload.length());
  result_ptr += _payload.length();

  return result;
}

uint32_t NdefRecord::get_encoded_size()
{
  return 2                                 // TNF + flags + type length
       + (_payload.length() > 255 ? 4 : 1) // payload length size
       + (_id.length() > 0 ? 1 : 0)        // ID length size
       + _type.length() + _id.length() + _payload.length();
}

#define INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(size)                                  \
  minimum_length += size;                                                              \
  if (data_length < minimum_length)                                                    \
  {                                                                                    \
    PRINT(F("NdefRecord::decode failed: data_length < minimum_length: "));             \
    PRINT(data_length);                                                                \
    PRINT(F(" < "));                                                                   \
    PRINT(minimum_length);                                                             \
    PRINTLN();                                                                         \
    return nullptr;                                                                    \
  }

NdefRecord *NdefRecord::decode(const uint8_t *data, uint32_t data_length)
{
  const uint8_t *data_ptr = data;
  uint8_t minimum_length = 0;

  // Decode flags and type name format
  uint8_t tnf_flags = *data_ptr++;

  if (tnf_flags & NDEF_RECORD_HEADER_CF_FLAG_MASK)
  {
    PRINTLN(F("NdefRecord::decode failed: chunked record not supported"));
    return nullptr;
  }

  auto is_message_begin = tnf_flags & NDEF_RECORD_HEADER_MB_FLAG_MASK;
  auto is_message_end = tnf_flags & NDEF_RECORD_HEADER_ME_FLAG_MASK;
  auto type_name_format = (TNF)(tnf_flags & NDEF_RECORD_HEADER_TNF_MASK);

  // Expect at least:
  // - 1 uint8_t for flags + TNF
  // - 1 uint8_t for type length
  // - 1 uint8_t for short payload length
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(3);

  // Type length
  auto type_length = *data_ptr++;
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(type_length);

  // Payload length
  uint32_t payload_length;
  if (tnf_flags & NDEF_RECORD_HEADER_SR_FLAG_MASK)
    payload_length = *data_ptr++;
  else
  {
    // Expect at least 3 more uint8_ts to store payload length
    INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(3);
    // Extract payload_length
    payload_length = *data_ptr++ << 24;
    payload_length |= *data_ptr++ << 16;
    payload_length |= *data_ptr++ << 8;
    payload_length |= *data_ptr++;
  }
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(payload_length);

  // ID length
  uint8_t id_length = 0;
  if (tnf_flags & NDEF_RECORD_HEADER_IL_FLAG_MASK)
  {
    id_length = *data_ptr++;
    INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(id_length);
  }

  // Type
  auto type_data = new uint8_t[type_length];
  if (type_data == nullptr)
    return nullptr;
  memcpy(type_data, data_ptr, type_length);
  data_ptr += type_length;
  auto type =
      new NdefRecordType(type_data, type_length, NdefRecordType::OwnershipUnique);
  if (type == nullptr)
  {
    delete type_data;
    return nullptr;
  }

  // ID
  uint8_t *id_data = nullptr;
  if (id_length > 0)
  {
    id_data = new uint8_t[id_length];
    if (id_data == nullptr)
    {
      delete type;
      return nullptr;
    }
    memcpy(id_data, data_ptr, id_length);
    data_ptr += id_length;
  }
  auto id = new NdefRecordId(id_data, id_length, NdefRecordId::OwnershipUnique);
  if (id == nullptr)
  {
    delete type;
    delete id_data;
    return nullptr;
  }

  // Payload
  uint8_t *payload_data = new uint8_t[payload_length];
  if (payload_data == nullptr)
  {
    delete type;
    delete id;
    return nullptr;
  }
  memcpy(payload_data, data_ptr, payload_length);
  data_ptr += payload_length;
  auto payload = new NdefRecordPayload(
      payload_data,
      payload_length,
      NdefRecordPayload::OwnershipUnique
  );
  if (payload == nullptr)
  {
    delete type;
    delete id;
    delete payload_data;
    return nullptr;
  }

  return new NdefRecord(
      type_name_format,
      *type,
      *payload,
      is_message_begin,
      is_message_end,
      *id
  );
}

NdefRecord *NdefRecord::create_text_record(const char *text, const char *language_code)
{
  // Prepare header
  uint32_t language_code_length = strlen(language_code);
  uint32_t header_length = 1 + language_code_length;
  uint8_t header[header_length];
  header[0] = language_code_length;
  memcpy(header + 1, language_code, language_code_length);

  // Prepare payload
  uint32_t text_length = strlen(text);
  uint32_t payload_length = header_length + text_length;

  auto payload = new uint8_t[payload_length];
  if (payload == nullptr)
    return nullptr;

  memcpy(payload, header, header_length);
  memcpy(payload + header_length, text, text_length);

  return new NdefRecord(
      TNF_WELL_KNOWN,
      *(new NdefRecordType(NdefRecordType::RTD_TEXT)),
      *(new NdefRecordPayload(
          payload,
          payload_length,
          NdefRecordPayload::OwnershipUnique
      ))
  );
}

NdefRecord *NdefRecord::create_uri_record(const char *uri)
{
  NdefUriPayload payload(uri);
  if (!payload.is_valid())
    return nullptr;

  auto payload_field = new NdefRecordPayload(payload.data(), payload.length());
  if (payload_field == nullptr)
    return nullptr;

  return new NdefRecord(
      NdefRecord::TNF_WELL_KNOWN,
      *(new NdefRecordType(NdefRecordType::RTD_URI)),
      *payload_field
  );
}

NdefRecord *NdefRecord::create_mime_media_record(
    const char *mime_type, const uint8_t *payload, uint32_t payload_length
)
{
  return new NdefRecord(
      NdefRecord::TNF_MIME_MEDIA,
      *(new NdefRecordType(mime_type)),
      *(new NdefRecordPayload(payload, payload_length))
  );
}

#define NDEF_RECORD_EXTERNAL_TYPE_PREFIX "urn:nfc:ext:"
#define NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH 12

NdefRecord *NdefRecord::create_external_type_record(
    const char *domain,
    const char *external_type,
    uint8_t *payload,
    uint32_t payload_length
)
{
  if (domain == nullptr || external_type == nullptr || payload == nullptr)
    return nullptr;

  auto domain_length = strlen(domain);
  auto external_type_length = strlen(external_type);
  auto type_length = NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH + domain_length +
                     1 // ':' separator
                   + external_type_length;

  if (domain_length == 0 || external_type_length == 0 || type_length > 0xff)
    return nullptr;

  auto type = new uint8_t[type_length];
  auto pointer = type;
  if (type == nullptr)
    return nullptr;

  // Build type field
  // 1. Add prefix
  memcpy(
      pointer,
      NDEF_RECORD_EXTERNAL_TYPE_PREFIX,
      NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH
  );
  pointer += NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH;
  // 2. Add domain
  memcpy(pointer, domain, domain_length);
  pointer += domain_length;
  // 3. Add ':' separator
  *pointer = ':';
  pointer++;
  // 4. Add external type
  memcpy(pointer, external_type, external_type_length);

  return new NdefRecord(
      TNF_EXTERNAL_TYPE,
      *(new NdefRecordType(type, type_length, NdefRecordType::OwnershipUnique)),
      *(new NdefRecordPayload(payload, payload_length, NdefRecordPayload::OwnershipCopy)
      )
  );
}
