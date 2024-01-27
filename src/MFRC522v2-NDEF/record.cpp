#include "record.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

NdefRecord::NdefRecord()
{
  is_message_begin = false;
  is_message_end = false;
  type_name_format = TNF_EMPTY;
  type = nullptr;
  type_length = 0;
  id = nullptr;
  id_length = 0;
  payload = nullptr;
  payload_length = 0;
}

NdefRecord::~NdefRecord()
{
  delete[] type;
  delete[] id;
  delete[] payload;
}

NdefRecord::TNF NdefRecord::get_type_name_format() { return type_name_format; }

void NdefRecord::set_type_name_format(TNF type_name_format)
{
  this->type_name_format = type_name_format;
}

#define REALLOC_FIELD_OR_FAIL(name, size)                                              \
  auto pointer = new uint8_t[size];                                                    \
  if (pointer == nullptr)                                                              \
  {                                                                                    \
    PRINT(F("NdefRecord::set_"));                                                      \
    PRINT(#name);                                                                      \
    PRINTLN(F(" failed to allocate memory"));                                          \
    return NDEF_ERROR_MALLOC_FAILED;                                                   \
  }                                                                                    \
  delete[] this->name;                                                                 \
  this->name = pointer;

#define SET_FIELD(name)                                                                \
  REALLOC_FIELD_OR_FAIL(name, name##_length);                                          \
  memcpy(this->name, name, name##_length);                                             \
  this->name##_length = name##_length;                                                 \
  return NDEF_SUCCESS;

uint8_t *NdefRecord::get_type() { return type; }

uint8_t NdefRecord::get_type_length() { return type_length; }

int8_t NdefRecord::set_type(uint8_t *type, uint8_t type_length) { SET_FIELD(type); }

int8_t NdefRecord::set_type(RTD type)
{
  REALLOC_FIELD_OR_FAIL(type, 1);
  this->type[0] = type;
  this->type_length = 1;
  return NDEF_SUCCESS;
}

uint8_t *NdefRecord::get_id() { return id; }

uint8_t NdefRecord::get_id_length() { return id_length; }

int8_t NdefRecord::set_id(uint8_t *id, uint8_t id_length) { SET_FIELD(id); }

uint8_t *NdefRecord::get_payload() { return payload; }

uint32_t NdefRecord::get_payload_length() { return payload_length; }

int8_t NdefRecord::set_payload(const uint8_t *payload, uint32_t payload_length)
{
  SET_FIELD(payload);
}

uint8_t *NdefRecord::encode()
{
  auto result = new uint8_t[get_encoded_size()];
  auto result_ptr = result;

  if (result == nullptr)
  {
    PRINTLN(F("NdefRecord::encode failed to allocate memory"));
    return nullptr;
  }

  // Build header
  uint8_t tnf_flags = 0;
  if (is_message_begin)
    tnf_flags |= NDEF_RECORD_HEADER_MB_FLAG_MASK;
  if (is_message_end)
    tnf_flags |= NDEF_RECORD_HEADER_ME_FLAG_MASK;
  if (payload_length < 256)
    tnf_flags |= NDEF_RECORD_HEADER_SR_FLAG_MASK;
  if (id_length > 0)
    tnf_flags |= NDEF_RECORD_HEADER_IL_FLAG_MASK;
  tnf_flags |= type_name_format;

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

uint32_t NdefRecord::get_encoded_size()
{
  return 2                              // TNF + flags + type length
       + (payload_length > 255 ? 4 : 1) // payload length size
       + (id_length > 0 ? 1 : 0)        // ID length size
       + type_length + id_length + payload_length;
}

#define INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(size)                                  \
  minimum_length += size;                                                              \
  if (data_length < minimum_length)                                                    \
  {                                                                                    \
    delete record;                                                                     \
    PRINT(F("NdefRecord::decode failed: data_length < minimum_length: "));             \
    PRINT(data_length);                                                                \
    PRINT(F(" < "));                                                                   \
    PRINT(minimum_length);                                                             \
    PRINTLN();                                                                         \
    return nullptr;                                                                    \
  }

NdefRecord *NdefRecord::decode(uint8_t *data, uint32_t data_length)
{
  uint8_t *data_ptr = data;
  uint8_t minimum_length = 0;

  // Decode flags and type name format
  uint8_t tnf_flags = *data_ptr++;

  if (tnf_flags & NDEF_RECORD_HEADER_CF_FLAG_MASK)
  {
    PRINTLN(F("NdefRecord::decode failed: chunked record not supported"));
    return nullptr;
  }

  auto record = new NdefRecord();
  record->is_message_begin = tnf_flags & NDEF_RECORD_HEADER_MB_FLAG_MASK;
  record->is_message_end = tnf_flags & NDEF_RECORD_HEADER_ME_FLAG_MASK;
  record->type_name_format = (TNF)(tnf_flags & NDEF_RECORD_HEADER_TNF_MASK);

  // Expect at least:
  // - 1 uint8_t for flags + TNF
  // - 1 uint8_t for type length
  // - 1 uint8_t for short payload length
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(3);

  // Type length
  record->type_length = *data_ptr++;
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(record->type_length);

  // Payload length
  if (tnf_flags & NDEF_RECORD_HEADER_SR_FLAG_MASK)
    record->payload_length = *data_ptr++;
  else
  {
    // Expect at least 3 more uint8_ts to store payload length
    INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(3);
    // Extract payload_length
    record->payload_length = *data_ptr++ << 24;
    record->payload_length |= *data_ptr++ << 16;
    record->payload_length |= *data_ptr++ << 8;
    record->payload_length |= *data_ptr++;
  }
  INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(record->payload_length);

  // ID length
  if (tnf_flags & 0b00001000) // IL=1
  {
    record->id_length = *data_ptr++;
    INCREASE_MINIMUM_LENGTH_OR_DECODE_ERROR(record->id_length);
  }
  else
    record->id_length = 0;

  // Type
  record->type = new uint8_t[record->type_length];

  if (record->type == nullptr)
    goto MALLOC_FAILED;

  memcpy(record->type, data_ptr, record->type_length);
  data_ptr += record->type_length;

  // ID
  if (record->id_length > 0)
  {
    record->id = new uint8_t[record->id_length];

    if (record->id == nullptr)
      goto MALLOC_FAILED;

    memcpy(record->id, data_ptr, record->id_length);
    data_ptr += record->id_length;
  }

  // Payload
  record->payload = new uint8_t[record->payload_length];

  if (record->payload == nullptr)
    goto MALLOC_FAILED;

  memcpy(record->payload, data_ptr, record->payload_length);
  data_ptr += record->payload_length;

  return record;

MALLOC_FAILED:
  PRINTLN(F("NdefRecord::decode failed to allocate memory"));
  delete record;
  return nullptr;
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
  uint8_t payload[payload_length];
  memcpy(payload, header, header_length);
  memcpy(payload + header_length, text, text_length);

  // Create record
  auto record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_WELL_KNOWN);

  if (record->set_type(NdefRecord::RTD_TEXT) != NDEF_SUCCESS ||
      record->set_payload(payload, payload_length) != NDEF_SUCCESS)
  {
    delete record;
    return nullptr;
  }

  return record;
}