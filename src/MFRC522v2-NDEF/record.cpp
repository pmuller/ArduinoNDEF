#include "record.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace ArduinoNDEF
{
namespace Record
{

uint8_t *Record::encode() const
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

uint32_t Record::get_encoded_size() const
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
    PRINT(F("Record::decode failed: data_length < minimum_length: "));                 \
    PRINT(data_length);                                                                \
    PRINT(F(" < "));                                                                   \
    PRINT(minimum_length);                                                             \
    PRINTLN();                                                                         \
    return nullptr;                                                                    \
  }

Record *Record::decode(const uint8_t &data, uint32_t data_length)
{
  const uint8_t *data_ptr = &data;
  uint8_t minimum_length = 0;

  // Decode flags and type name format
  uint8_t tnf_flags = *data_ptr++;

  if (tnf_flags & NDEF_RECORD_HEADER_CF_FLAG_MASK)
  {
    PRINTLN(F("Record::decode failed: chunked record not supported"));
    return nullptr;
  }

  auto is_message_begin = tnf_flags & NDEF_RECORD_HEADER_MB_FLAG_MASK;
  auto is_message_end = tnf_flags & NDEF_RECORD_HEADER_ME_FLAG_MASK;
  auto type_name_format = static_cast<TNF>(tnf_flags & NDEF_RECORD_HEADER_TNF_MASK);

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
  Field::Type *type = nullptr;
  if (type_length > 0)
  {
    auto type_data = new uint8_t[type_length];
    if (type_data == nullptr)
      return nullptr;
    memcpy(type_data, data_ptr, type_length);
    data_ptr += type_length;
    type = new Field::Type(type_data, type_length, Field::Type::OwnershipUnique);
    if (type == nullptr)
    {
      delete type_data;
      return nullptr;
    }
  }
  else
    type = new Field::Type();

  // ID
  uint8_t *id_data = nullptr;
  Field::Id *id = nullptr;
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
    id = new Field::Id(id_data, id_length, Field::Id::OwnershipUnique);
  }
  else
    id = new Field::Id();
  if (id == nullptr)
  {
    delete type;
    delete id_data;
    return nullptr;
  }

  // Payload
  uint8_t *payload_data = nullptr;
  Field::Payload *payload = nullptr;
  if (payload_length > 0)
  {
    payload_data = new uint8_t[payload_length];
    if (payload_data == nullptr)
    {
      delete type;
      delete id;
      return nullptr;
    }
    memcpy(payload_data, data_ptr, payload_length);
    data_ptr += payload_length;
    payload = new Field::Payload(
        payload_data,
        payload_length,
        Field::Payload::OwnershipUnique
    );
  }
  else
    payload = new Field::Payload();
  if (payload == nullptr)
  {
    delete type;
    delete id;
    delete payload_data;
    return nullptr;
  }

  return new Record(
      type_name_format,
      *type,
      *payload,
      is_message_begin,
      is_message_end,
      *id
  );
}

bool Record::operator==(const Record &other) const
{
  return type_name_format == other.type_name_format && _type == other._type &&
         _payload == other._payload && is_message_begin == other.is_message_begin &&
         is_message_end == other.is_message_end && _id == other._id;
}

#define NDEF_EMPTY_RECORD_SIZE 3

uint32_t get_encoded_ndef_record_size(const uint8_t *data, uint32_t length)
{
  if (data == nullptr)
    return 0;

  uint32_t index = 0;
  auto remaining = length;

  if (remaining < NDEF_EMPTY_RECORD_SIZE)
    return 0;

  uint8_t tnf_flags = data[index++];
  uint8_t type_length = data[index++];
  remaining -= 2;

  // Payload length: can be 1 or 4 bytes depending on the SR flag
  uint8_t payload_length_size = tnf_flags & NDEF_RECORD_HEADER_SR_FLAG_MASK ? 1 : 4;
  if (remaining < payload_length_size)
    return 0;
  uint32_t payload_length = 0;
  for (uint8_t i = 0; i < payload_length_size; i++)
    payload_length = (payload_length << 8) | data[index++];
  if (remaining < payload_length_size + payload_length)
    return 0;
  remaining -= payload_length_size + payload_length;

  // ID field is optional
  uint8_t id_length = tnf_flags & NDEF_RECORD_HEADER_IL_FLAG_MASK ? data[index++] : 0;
  if (remaining < id_length)
    return 0;

  return index + type_length + id_length + payload_length;
}

} // namespace Record
} // namespace ArduinoNDEF