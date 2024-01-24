#pragma once

#include <cstddef>
#include <cstdint>

#define NDEF_RECORD_DECODE_ERROR_INVALID_LENGTH -2
#define NDEF_RECORD_DECODE_ERROR_CHUNK_NOT_SUPPORTED -3
#define NDEF_RECORD_ENCODE_ERROR_MALLOC_FAILED NULL

class NdefRecord
{
  public:
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
    enum RTD
    {
      RTD_TEXT = 0x54,
      RTD_URI = 0x55
    };

    bool is_message_begin;
    bool is_message_end;

    NdefRecord();
    ~NdefRecord();

    TNF get_type_name_format();
    void set_type_name_format(TNF type_name_format);
    uint8_t *get_type();
    int8_t set_type(uint8_t *type, uint8_t type_length);
    int8_t set_type(RTD type);
    int8_t set_id(uint8_t *id, uint8_t id_length);
    int8_t set_payload(uint8_t *payload, size_t payload_length);
    size_t get_encoded_size();
    uint8_t *encode();
    int8_t decode(uint8_t *data, size_t data_length);

  private:
    TNF type_name_format;
    uint8_t *type;
    uint8_t type_length;
    uint8_t *id;
    uint8_t id_length;
    uint8_t *payload;
    size_t payload_length;
};