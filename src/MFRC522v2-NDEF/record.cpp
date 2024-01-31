#include "record.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t *NdefRecord::encode() const
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

uint32_t NdefRecord::get_encoded_size() const
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

NdefRecord *NdefRecord::decode(const uint8_t &data, uint32_t data_length)
{
  const uint8_t *data_ptr = &data;
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

bool NdefRecord::operator==(const NdefRecord &other) const
{
  return type_name_format == other.type_name_format && _type == other._type &&
         _payload == other._payload && is_message_begin == other.is_message_begin &&
         is_message_end == other.is_message_end && _id == other._id;
}