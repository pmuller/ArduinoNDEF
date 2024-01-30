#pragma once

#include "record.hpp"

#include <stdint.h>

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
    int8_t add_record(NdefRecord *record);

    /**
     * @brief Add an empty record to the message
     *
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t add_empty_record();

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
     * @return int8_t NDEF_SUCCESS, NDEF_ERROR_MALLOC_FAILED,
     * NDEF_ERROR_INVALID_RECORD_COUNT, NDEF_ERROR_INVALID_RECORD_LENGTH or
     * NDEF_ERROR_CHUNK_NOT_SUPPORTED
     */
    int8_t decode(const uint8_t &data, uint32_t data_length);

    /**
     * @brief Get the record count
     *
     * @return uint8_t Record count
     */
    uint8_t get_record_count();

  private:
    NdefRecord **records;
    uint8_t record_count;
};