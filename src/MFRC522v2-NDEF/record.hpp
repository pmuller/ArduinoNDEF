#pragma once
#include <stdint.h>

// Message Begin flag (MB)
#define NDEF_RECORD_HEADER_MB_FLAG_MASK 0b10000000
// Message End flag (ME)
#define NDEF_RECORD_HEADER_ME_FLAG_MASK 0b01000000
// Chunk Flag (CF)
#define NDEF_RECORD_HEADER_CF_FLAG_MASK 0b00100000
// Short Record flag (SR)
#define NDEF_RECORD_HEADER_SR_FLAG_MASK 0b00010000
// ID Length flag (IL)
#define NDEF_RECORD_HEADER_IL_FLAG_MASK 0b00001000
// Type Name Format
#define NDEF_RECORD_HEADER_TNF_MASK 0b00000111

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
     * @brief Create an empty NDEF record
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
     * @return NdefRecord* The decoded NDEF record
     */
    static NdefRecord *decode(uint8_t *data, uint32_t data_length);

    /**
     * @brief Create a text NDEF record
     *
     * @param text The text
     * @param language_code The language code
     * @return NdefRecord* The NDEF record
     */
    static NdefRecord *create_text_record(const char *text, const char *language_code);

    /**
     * @brief Create a URI NDEF record
     *
     * @param uri The URI
     * @return NdefRecord* The NDEF record
     */
    static NdefRecord *create_uri_record(const char *uri);

    /**
     * @brief Create a MIME media NDEF record
     *
     * @param mime_type The MIME media type
     * @param payload The payload
     * @param payload_length The payload length
     * @return NdefRecord* The NDEF record
     */
    static NdefRecord *create_mime_media_record(
        const char *mime_type, const uint8_t *payload, uint32_t payload_length
    );

    /**
     * @brief Create an external type NDEF record
     *
     * @param domain The domain
     * @param external_type The type
     * @param payload The payload
     * @param payload_length The payload length
     * @return NdefRecord* The NDEF record
     */
    static NdefRecord *create_external_type_record(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length
    );

  private:
    TNF type_name_format;
    uint8_t *type;
    uint8_t type_length;
    uint8_t *id;
    uint8_t id_length;
    uint8_t *payload;
    uint32_t payload_length;
};