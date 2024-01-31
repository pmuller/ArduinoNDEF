#pragma once

#include "record.hpp"

#include <stdint.h>

namespace ArduinoNDEF
{

/**
 * @brief NDEF Message
 */
class NdefMessage
{
  public:
    /**
     * @brief Create a new NDEF Message
     */
    NdefMessage();
    /**
     * @brief Destroy the NDEF Message
     */
    ~NdefMessage();

    /**
     * @brief Add a record to the message
     *
     * @param record The record to add
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t add_record(Record::NdefRecord *record);

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
     * @return NdefMessage* The decoded NDEF message
     */
    static NdefMessage *decode(const uint8_t *data, uint32_t length);

    /**
     * @brief Get the record count
     *
     * @return uint8_t Record count
     */
    uint8_t get_record_count();

    /**
     * @brief Get the record at the given index
     *
     * @param index The index of the record
     * @return NdefRecord* The record
     */
    const Record::NdefRecord *record(uint8_t index) const
    {
      return index < record_count ? records[index] : nullptr;
    }

  private:
    NdefMessage(Record::NdefRecord **records, uint8_t record_count) :
        records(records), record_count(record_count){};
    Record::NdefRecord **records;
    uint8_t record_count;
};

uint8_t count_ndef_message_records(const uint8_t *message, uint32_t size);

} // namespace ArduinoNDEF