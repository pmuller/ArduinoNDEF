#pragma once

#include "field.hpp"

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
 * @brief Class representing an NDEF record.
 *
 * An NDEF record is a data structure used in NFC (Near Field Communication) to store
 * and exchange data. This class provides methods to create, encode, decode, and
 * manipulate NDEF records.
 */
class NdefRecord
{
  public:
    /**
     * @brief Type Name Format (TNF) Definition
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
     * @brief Message Begin flag (MB)
     */
    bool is_message_begin;

    /**
     * @brief Message End flag (ME)
     */
    bool is_message_end;

    /**
     * @brief Type Name Format
     */
    TNF type_name_format;

    /**
     * @brief Create a NDEF record
     *
     * @param type_name_format Type Name Format
     * @param type The type
     * @param payload The payload
     * @param is_message_begin Message Begin flag (MB)
     * @param is_message_end Message End flag (ME)
     * @param id The ID
     */
    NdefRecord(
        TNF type_name_format,
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        is_message_begin(is_message_begin),
        is_message_end(is_message_end), type_name_format(type_name_format), _type(type),
        _id(id), _payload(payload){};

    /**
     * @brief Destroy the NDEF record
     */
    ~NdefRecord()
    {
      delete &_type;
      delete &_id;
      delete &_payload;
    };

    /**
     * @brief Get the size of the encoded NDEF record
     *
     * @return uint32_t Encoded size
     */
    uint32_t get_encoded_size() const;

    /**
     * @brief Encode the NDEF record
     *
     * @return uint8_t* Encoded NDEF record
     */
    uint8_t *encode() const;

    /**
     * @brief Check if the NDEF record is equal to another NDEF record
     *
     * @param other The other NDEF record
     * @return true The NDEF records are equal
     * @return false The NDEF records are not equal
     */
    bool operator==(const NdefRecord &other) const;

    /**
     * @brief Decode an encoded NDEF record
     *
     * @param data Encoded NDEF record
     * @param data_length The data length
     * @return NdefRecord* The decoded NDEF record
     */
    static NdefRecord *decode(const uint8_t &data, uint32_t data_length);

    const NdefRecordType &type() const { return _type; };
    const NdefRecordId &id() const { return _id; };
    const NdefRecordPayload &payload() const { return _payload; };

  protected:
    const NdefRecordType &_type;
    const NdefRecordId &_id;
    const NdefRecordPayload &_payload;
};
