#include "message.hpp"

#include "constants.hpp"
#include "macros.hpp"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace ArduinoNDEF
{

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

int8_t NdefMessage::add_record(Record::Record *record)
{
  if (record == nullptr)
  {
    PRINTLN(F("NdefMessage::add_record failed"));
    return NDEF_ERROR_RECORD_IS_NULL;
  }

  Record::Record **records = new Record::Record *[record_count + 1];

  if (records == nullptr)
  {
    PRINTLN(F("NdefMessage::add_record failed to allocate memory"));
    return NDEF_ERROR_MALLOC_FAILED;
  }

  if (this->records != nullptr)
  {
    memcpy(records, this->records, sizeof(Record::Record *) * record_count);
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

uint8_t count_ndef_message_records(const uint8_t *message, uint32_t length)
{
  if (message == nullptr || length == 0)
    return 0;

  uint8_t count = 0;
  uint32_t index = 0;
  uint32_t record_size;

  while (index < length)
  {
    // Get current record size
    record_size = Record::get_encoded_ndef_record_size(&message[index], length - index);
    // Stop if current record size is 0 (probably invalid)
    if (record_size == 0)
      break;
    // Increment record count
    count++;
    // Stop on last record
    if (message[index] & NDEF_RECORD_HEADER_ME_FLAG_MASK)
      break;
    // Increment index by record size
    index += record_size;
  }

  return count;
}

NdefMessage *NdefMessage::decode(const uint8_t *data, uint32_t length)
{
  if (data == nullptr || length == 0)
    return nullptr;

  auto record_index = 0;
  auto record_count = count_ndef_message_records(data, length);
  auto records = new Record::Record *[record_count];
  auto data_ptr = data;
  auto data_end = data_ptr + length;

  while (data_ptr < data_end)
  {
    auto record = Record::Record::decode(*data_ptr, data_end - data_ptr);

    if (record == nullptr)
    {
      PRINTLN(F("NdefMessage::decode failed"));
      for (uint8_t i = 0; i < record_index; i++)
        delete records[i];
      delete records;
      return nullptr;
    }

    data_ptr += record->get_encoded_size();
    records[record_index++] = record;

    if (record->is_message_end)
      break;
  }

  return new NdefMessage(records, record_count);
}

uint8_t NdefMessage::get_record_count() { return record_count; }

} // namespace ArduinoNDEF