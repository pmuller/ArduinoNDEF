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
    delete records[i];

  delete[] records;
}

int8_t NdefMessage::add_record(NdefRecord *record)
{
  NdefRecord **records = new NdefRecord *[record_count + 1];

  if (records == nullptr)
  {
    PRINTLN(F("NdefMessage::add_record failed to allocate memory"));
    return NDEF_ERROR_MALLOC_FAILED;
  }

  if (this->records != nullptr)
  {
    memcpy(records, this->records, sizeof(NdefRecord *) * record_count);
    delete[] this->records;
  }

  this->records = records;

  if (record_count == 0)
  {
    record->is_message_begin = true;
    record->is_message_end = true;
  }
  else
  {
    records[record_count - 1]->is_message_end = false;
    record->is_message_end = true;
  }

  records[record_count++] = record;

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_mime_media_record(
    const char *mime_type, const uint8_t *payload, uint32_t payload_length
)
{
  auto record =
      NdefRecord::create_mime_media_record(mime_type, payload, payload_length);

  if (record == nullptr)
  {
    PRINTLN(F("NdefMessage::add_mime_media_record failed to create MIME media record"));
    return NDEF_ERROR_MIME_MEDIA_RECORD_CREATION_FAILED;
  }

  add_record(record);

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_text_record(char *text, const char *language_code)
{
  auto record = NdefRecord::create_text_record(text, language_code);

  if (record == nullptr)
  {
    PRINTLN(F("NdefMessage::add_text_record failed to create text record"));
    return NDEF_ERROR_TEXT_RECORD_CREATION_FAILED;
  }

  int8_t error = add_record(record);

  if (error != NDEF_SUCCESS)
  {
    delete record;
    PRINTLN(F("NdefMessage::add_text_record failed to add record"));
    return error;
  }

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_uri_record(const char *uri)
{
  auto record = NdefRecord::create_uri_record(uri);

  if (record == nullptr)
  {
    PRINTLN(F("NdefMessage::add_uri_record failed to create URI record"));
    return NDEF_ERROR_URI_RECORD_CREATION_FAILED;
  }

  add_record(record);

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_external_type_record(
    const char *domain,
    const char *external_type,
    uint8_t *payload,
    uint32_t payload_length
)
{
  auto record = NdefRecord::create_external_type_record(
      domain,
      external_type,
      payload,
      payload_length
  );

  if (record == nullptr)
  {
    PRINTLN(
        F("NdefMessage::add_external_type_record failed to create external type record")
    );
    return NDEF_ERROR_EXTERNAL_TYPE_RECORD_CREATION_FAILED;
  }

  int8_t error = add_record(record);
  if (error != NDEF_SUCCESS)
  {
    delete record;
    PRINTLN(F("NdefMessage::add_external_type_record failed to add record"));
    return error;
  }

  return NDEF_SUCCESS;
}

int8_t NdefMessage::add_empty_record() { return add_record(new NdefRecord()); }

uint32_t NdefMessage::get_encoded_size()
{
  uint32_t result = 0;

  for (uint8_t i = 0; i < record_count; i++)
    result += records[i]->get_encoded_size();

  return result;
}

uint8_t *NdefMessage::encode()
{
  auto result = new uint8_t[get_encoded_size()];
  uint8_t *result_ptr = result;

  if (result == nullptr)
  {
    PRINTLN(F("NdefMessage::encode failed to allocate memory"));
    return nullptr;
  }

  for (uint8_t i = 0; i < record_count; i++)
  {
    uint8_t *record_encoded = records[i]->encode();
    uint32_t record_encoded_size = records[i]->get_encoded_size();
    memcpy(result_ptr, record_encoded, record_encoded_size);
    result_ptr += record_encoded_size;
    delete[] record_encoded;
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
    auto record = NdefRecord::decode(data_ptr, data_end - data_ptr);

    if (record == nullptr)
    {
      PRINTLN(F("NdefMessage::decode failed"));
      return -1;
    }

    found_last_message = record->is_message_end;
    int8_t error;
    RETURN_IF_ERROR(add_record(record), { delete record; });
    data_ptr += record->get_encoded_size();
  }

  return NDEF_SUCCESS;
}

uint8_t NdefMessage::get_record_count() { return record_count; }