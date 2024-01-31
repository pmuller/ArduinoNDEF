#pragma once

#include "record.hpp"

#include <stdint.h>

namespace ArduinoNDEF
{

/**
 * @brief NDEF Message
 */
class Message
{
  public:
    /**
     * @brief Create a new NDEF Message
     */
    Message();
    /**
     * @brief Destroy the NDEF Message
     */
    ~Message();

    /**
     * @brief Add a record to the message
     *
     * @param record The record to add
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t add_record(Record::Record *record);

    /**
     * @brief Get the size of the encoded NDEF message
     *
     * @return uint8_t Encoded size
     */
    uint32_t get_encoded_size();

    /**
     * @brief Encode the NDEF message
     *
     * @return uint8_t* Encoded NDEF message
     */
    uint8_t *encode();

    /**
     * @brief Decode an NDEF message
     *
     * @param data The data to decode
     * @param data_length The data length
     * @return Message* The decoded NDEF message
     */
    static Message *decode(const uint8_t *data, uint32_t length);

    /**
     * @brief Get the record count
     *
     * @return uint8_t Record count
     */
    uint8_t record_count();

    /**
     * @brief Get the records
     *
     * @return Record** The records
     */
    const Record::Record **records() const
    {
      return const_cast<const Record::Record **>(_records);
    }

    /**
     * @brief Get the record at the given index
     *
     * @param index The index of the record
     * @return Record* The record
     */
    const Record::Record *record(uint8_t index) const
    {
      return index < _record_count ? _records[index] : nullptr;
    }

  private:
    Message(Record::Record **records, uint8_t record_count) :
        _records(records), _record_count(record_count){};
    Record::Record **_records;
    uint8_t _record_count;
};

uint8_t count_ndef_message_records(const uint8_t *message, uint32_t size);

} // namespace ArduinoNDEF