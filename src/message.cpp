#include "message.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

NdefMessage::NdefMessage()
{
  records = nullptr;
  record_count = 0;
}

NdefMessage::~NdefMessage()
{
  for (uint8_t i = 0; i < record_count; i++)
    delete &records[i];

  free(records);
}

int8_t NdefMessage::add_record(NdefRecord *record)
{
  NdefRecord *records =
      (NdefRecord *)realloc(this->records, sizeof(NdefRecord) * (record_count + 1));

  if (records == nullptr)
  {
    PRINTLN(F("NdefMessage::add_record failed to allocate memory"));
    return NDEF_ERROR_MALLOC_FAILED;
  }

  this->records = records;

  if (record_count == 0)
  {
    record->is_message_begin = true;
    record->is_message_end = true;
  }
  else
  {
    records[record_count - 1].is_message_end = false;
    record->is_message_end = true;
  }

  records[record_count++] = *record;

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_mime_media_record(
    uint8_t *mime_type,
    uint8_t mime_type_length,
    uint8_t *payload,
    uint32_t payload_length
)
{
  NdefRecord *record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_MIME_MEDIA);
  int8_t error;
  RETURN_IF_ERROR(record->set_type(mime_type, mime_type_length), {});
  RETURN_IF_ERROR(record->set_payload(payload, payload_length), {});
  add_record(record);
  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_text_record(char *text, const char *language_code)
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

  // Add record
  NdefRecord *record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_WELL_KNOWN);
  int8_t error;
  RETURN_IF_ERROR(record->set_type(NdefRecord::RTD_TEXT), {});
  RETURN_IF_ERROR(record->set_payload(payload, payload_length), {});
  add_record(record);

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_uri_record(char *uri)
{
  // Prepare payload
  uint32_t uri_length = strlen(uri);
  uint32_t payload_length = 1 + uri_length;
  uint8_t payload[payload_length];
  payload[0] = 0;
  memcpy(payload + 1, uri, uri_length);

  // Add record
  NdefRecord *record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_WELL_KNOWN);
  int8_t error;
  RETURN_IF_ERROR(record->set_type(NdefRecord::RTD_URI), {});
  RETURN_IF_ERROR(record->set_payload(payload, payload_length), {});
  add_record(record);

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_external_type_record(
    uint8_t *type, uint8_t type_length, uint8_t *payload, uint32_t payload_length
)
{
  NdefRecord *record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_EXTERNAL_TYPE);
  int8_t error;
  RETURN_IF_ERROR(record->set_type(type, type_length), {});
  RETURN_IF_ERROR(record->set_payload(payload, payload_length), {});
  add_record(record);
  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_empty_record()
{
  NdefRecord *record = new NdefRecord();
  record->set_type_name_format(NdefRecord::TNF_EMPTY);
  add_record(record);
  return NDEF_SUCCESS;
}

uint32_t NdefMessage::get_encoded_size()
{
  uint32_t result = 0;

  for (uint8_t i = 0; i < record_count; i++)
    result += records[i].get_encoded_size();

  return result;
}

uint8_t *NdefMessage::encode()
{
  SAFE_MALLOC(uint8_t *, result, get_encoded_size(), nullptr, {});
  uint8_t *result_ptr = result;

  for (uint8_t i = 0; i < record_count; i++)
  {
    uint8_t *record_encoded = records[i].encode();
    uint32_t record_encoded_size = records[i].get_encoded_size();
    memcpy(result_ptr, record_encoded, record_encoded_size);
    result_ptr += record_encoded_size;
    free(record_encoded);
  }

  return result;
}

int8_t NdefMessage::decode(uint8_t *data, uint32_t data_length)
{
  uint8_t *data_ptr = data;
  uint8_t *data_end = data + data_length;
  bool found_last_message = false;

  while (data_ptr < data_end && !found_last_message)
  {
    NdefRecord *record = new NdefRecord();
    int8_t error;
    RETURN_IF_ERROR(record->decode(data_ptr, data_end - data_ptr), { delete record; });
    found_last_message = record->is_message_end;
    RETURN_IF_ERROR(add_record(record), { delete record; });
    data_ptr += record->get_encoded_size();
  }

  return NDEF_SUCCESS;
}

uint8_t NdefMessage::get_record_count() { return record_count; }