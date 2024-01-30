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
  if (record == nullptr)
  {
    PRINTLN(F("NdefMessage::add_record failed"));
    return NDEF_ERROR_RECORD_IS_NULL;
  }

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

int8_t NdefMessage::decode(const uint8_t &data, uint32_t data_length)
{
  const uint8_t *data_ptr = &data;
  const uint8_t *data_end = &data + data_length;
  bool found_last_message = false;

  while (data_ptr < data_end && !found_last_message)
  {
    auto record = NdefRecord::decode(*data_ptr, data_end - data_ptr);

    if (record == nullptr)
    {
      PRINTLN(F("NdefMessage::decode failed"));
      return -1;
    }

    found_last_message = record->is_message_end;

    int8_t error = add_record(record);
    if (error != NDEF_SUCCESS)
    {
      delete record;
      return error;
    }

    data_ptr += record->get_encoded_size();
  }

  return NDEF_SUCCESS;
}

uint8_t NdefMessage::get_record_count() { return record_count; }