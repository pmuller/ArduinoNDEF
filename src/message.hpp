#pragma once

#include "record.hpp"

#include <stdint.h>

class NdefMessage
{
  public:
    NdefMessage();
    ~NdefMessage();

    int8_t add_record(NdefRecord *record);
    int8_t add_mime_media_record(
        uint8_t *mime_type,
        uint8_t mime_type_length,
        uint8_t *payload,
        uint32_t payload_length
    );
    int8_t add_text_record(char *text, const char *language_code = "en");
    int8_t add_uri_record(char *uri);
    int8_t add_external_type_record(
        uint8_t *type, uint8_t type_length, uint8_t *payload, uint32_t payload_length
    );
    int8_t add_empty_record();
    uint32_t get_encoded_size();
    uint8_t *encode();
    int8_t decode(uint8_t *data, uint32_t data_length);

  private:
    NdefRecord *records;
    uint8_t record_count;
};