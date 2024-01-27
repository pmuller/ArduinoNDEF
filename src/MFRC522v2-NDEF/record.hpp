#pragma once
#include <stdint.h>

/**
 * @brief A NDEF message record
 */
class NdefRecord
{
  public:
    /**
     * @brief Type Name Format
     */
    enum TNF
    {
      TNF_EMPTY,
      TNF_WELL_KNOWN,
      TNF_MIME_MEDIA,
      TNF_ABSOLUTE_URI,
      TNF_EXTERNAL_TYPE,
      TNF_UNKNOWN,
      TNF_UNCHANGED,
      TNF_RESERVED
    };

    /**
     * @brief NFC Record Type Definition
     */
    enum RTD
    {
      RTD_TEXT = 0x54,
      RTD_URI = 0x55
    };

    /**
     * @brief Message Begin flag (MB)
     */
    bool is_message_begin;
    /**
     * @brief Message End flag (ME)
     */
    bool is_message_end;

    /**
     * @brief Create a new NDEF record
     */
    NdefRecord();
    /**
     * @brief Destroy the NDEF record
     */
    ~NdefRecord();

    /**
     * @brief Get the record Type Name Format
     *
     * @return TNF The Type Name Format
     */
    TNF get_type_name_format();

    /**
     * @brief Set the record Type Name Format
     *
     * @param type_name_format The Type Name Format
     */
    void set_type_name_format(TNF type_name_format);

    /**
     * @brief Get the record type field
     *
     * @return uint8_t* The type
     */
    uint8_t *get_type();

    /**
     * @brief Get the record type field length
     *
     * @return uint8_t The type length
     */
    uint8_t get_type_length();

    /**
     * @brief Set the record type field
     *
     * @param type The type
     * @param type_length The type length
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t set_type(uint8_t *type, uint8_t type_length);

    /**
     * @brief Set the record type field
     *
     * @param type The type
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t set_type(RTD type);

    /**
     * @brief Get the record ID field
     *
     * @return uint8_t* The ID
     */
    uint8_t *get_id();

    /**
     * @brief Get the record ID field length
     *
     * @return uint8_t The ID length
     */
    uint8_t get_id_length();

    /**
     * @brief Set the record ID field
     *
     * @param id The ID
     * @param id_length The ID length
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t set_id(uint8_t *id, uint8_t id_length);

    /**
     * @brief Get the record payload field
     *
     * @return uint8_t* The payload
     */
    uint8_t *get_payload();

    /**
     * @brief Get the record payload field length
     *
     * @return uint32_t The payload length
     */
    uint32_t get_payload_length();

    /**
     * @brief Set the record payload field
     *
     * @param payload The payload
     * @param payload_length The payload length
     * @return int8_t NDEF_SUCCESS or NDEF_ERROR_MALLOC_FAILED
     */
    int8_t set_payload(const uint8_t *payload, uint32_t payload_length);

    /**
     * @brief Get the size of the encoded NDEF record
     *
     * @return uint32_t Encoded size
     */
    uint32_t get_encoded_size();

    /**
     * @brief Encode the NDEF record
     *
     * @return uint8_t* Encoded NDEF record
     */
    uint8_t *encode();

    /**
     * @brief Decode an encoded NDEF record
     *
     * @param data Encoded NDEF record
     * @param data_length The data length
     * @return uint8_t NDEF_SUCCESS or NDEF_RECORD_DECODE_ERROR_*
     */
    int8_t decode(uint8_t *data, uint32_t data_length);

  private:
    TNF type_name_format;
    uint8_t *type;
    uint8_t type_length;
    uint8_t *id;
    uint8_t id_length;
    uint8_t *payload;
    uint32_t payload_length;
};