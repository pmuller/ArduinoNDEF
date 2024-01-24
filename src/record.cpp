#include "record.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <cstring>

NdefRecord::NdefRecord()
{
  is_message_begin = false;
  is_message_end = false;
  type_name_format = TNF_EMPTY;
  type = NULL;
  type_length = 0;
  id = NULL;
  id_length = 0;
  payload = NULL;
  payload_length = 0;
}

NdefRecord::~NdefRecord()
{
  free(type);
  free(id);
  free(payload);
}

NdefRecord::TNF NdefRecord::get_type_name_format() { return type_name_format; }

void NdefRecord::set_type_name_format(TNF type_name_format)
{
  this->type_name_format = type_name_format;
}

#define REALLOC_FIELD_OR_FAIL(name, size)                                              \
  uint8_t *pointer = (uint8_t *)realloc(this->name, size);                             \
  if (pointer == NULL)                                                                 \
  {                                                                                    \
    PRINT(F("NdefRecord::set_"));                                                      \
    PRINT(#name);                                                                      \
    PRINTLN(F(" failed to allocate memory"));                                          \
    return NDEF_ERROR_MALLOC_FAILED;                                                   \
  }                                                                                    \
  this->name = pointer;

#define DECLARE_FIELD_SETTER(name, size_type)                                          \
  int8_t NdefRecord::set_##name(uint8_t *name, size_type name##_length)                \
  {                                                                                    \
    REALLOC_FIELD_OR_FAIL(name, name##_length);                                        \
    memcpy(this->name, name, name##_length);                                           \
    this->name##_length = name##_length;                                               \
    return NDEF_SUCCESS;                                                               \
  }

DECLARE_FIELD_SETTER(type, uint8_t)
DECLARE_FIELD_SETTER(id, uint8_t)
DECLARE_FIELD_SETTER(payload, size_t)

int8_t NdefRecord::set_type(RTD type)
{
  REALLOC_FIELD_OR_FAIL(type, 1);
  this->type[0] = type;
  this->type_length = 1;
  return NDEF_SUCCESS;
}

uint8_t *NdefRecord::encode()
{
  SAFE_MALLOC(
      uint8_t *,
      result,
      get_encoded_size(),
      NDEF_RECORD_ENCODE_ERROR_MALLOC_FAILED,
      {}
  );
  uint8_t *result_ptr = result;

  // Build TNF + flags uint8_t
  uint8_t tnf_flags = 0;
  if (is_message_begin) // MB
    tnf_flags |= 0b10000000;
  if (is_message_end) // ME
    tnf_flags |= 0b01000000;
  if (payload_length < 256) // SR
    tnf_flags |= 0b00010000;
  if (id_length > 0) // IL
    tnf_flags |= 0b00001000;
  tnf_flags |= type_name_format; // TNF

  *result_ptr++ = tnf_flags;
  *result_ptr++ = type_length;

  if (payload_length > 255)
  {
    *result_ptr++ = (payload_length >> 24) & 0xFF;
    *result_ptr++ = (payload_length >> 16) & 0xFF;
    *result_ptr++ = (payload_length >> 8) & 0xFF;
    *result_ptr++ = payload_length & 0xFF;
  }
  else
  {
    *result_ptr++ = payload_length;
  }

  if (id_length > 0)
  {
    *result_ptr++ = id_length;
  }

  memcpy(result_ptr, type, type_length);
  result_ptr += type_length;

  if (id_length > 0)
  {
    memcpy(result_ptr, id, id_length);
    result_ptr += id_length;
  }

  memcpy(result_ptr, payload, payload_length);
  result_ptr += payload_length;

  return result;
}

size_t NdefRecord::get_encoded_size()
{
  return 2                              // TNF + flags + type length
       + (payload_length > 255 ? 4 : 1) // payload length size
       + (id_length > 0 ? 1 : 0)        // ID length size
       + id_length + payload_length;
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
    return NDEF_RECORD_DECODE_ERROR_INVALID_LENGTH;                                    \
  }

int8_t NdefRecord::decode(uint8_t *data, size_t data_length)
{
  uint8_t *data_ptr = data;
  uint8_t minimum_length = 0;

  // Expect at least:
  // - 1 uint8_t for flags + TNF
  // - 1 uint8_t for type length
  // - 1 uint8_t for short payload length
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(3);

  // Decode flags and type name format
  uint8_t tnf_flags = *data_ptr++;
  bool is_message_begin = tnf_flags & 0b10000000;
  bool is_message_end = tnf_flags & 0b01000000;
  bool is_chunked = tnf_flags & 0b00100000;
  bool is_short_record = tnf_flags & 0b00010000;
  bool has_id_length = tnf_flags & 0b00001000;
  TNF type_name_format = (TNF)(tnf_flags & 0b00000111);

  if (is_chunked)
  {
    PRINTLN(F("NdefRecord::decode failed: chunked record not supported"));
    return NDEF_RECORD_DECODE_ERROR_CHUNK_NOT_SUPPORTED;
  }

  // Type length
  uint8_t type_length = *data_ptr++;
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(type_length);

  // Payload length
  size_t payload_length;
  if (is_short_record)
  {
    payload_length = *data_ptr++;
  }
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
  if (has_id_length) // Has ID length field
  {
    id_length = *data_ptr++;
    INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(id_length);
  }

  // Type
  SAFE_MALLOC(uint8_t *, type, type_length, NDEF_ERROR_MALLOC_FAILED, {});
  memcpy(type, data_ptr, type_length);
  data_ptr += type_length;

  // ID
  uint8_t *id = NULL;
  if (id_length > 0)
  {
    SAFE_MALLOC(uint8_t *, id, id_length, NDEF_ERROR_MALLOC_FAILED, { free(type); });
    memcpy(id, data_ptr, id_length);
    data_ptr += id_length;
  }

  // Payload
  SAFE_MALLOC(uint8_t *, payload, payload_length, NDEF_ERROR_MALLOC_FAILED, {
    free(type);
    free(id);
  });
  memcpy(payload, data_ptr, payload_length);
  data_ptr += payload_length;

  // Initialize record
  this->type_name_format = type_name_format;
  this->type = type;
  this->type_length = type_length;
  this->id = id;
  this->id_length = id_length;
  this->payload = payload;
  this->payload_length = payload_length;

  return NDEF_SUCCESS;
}