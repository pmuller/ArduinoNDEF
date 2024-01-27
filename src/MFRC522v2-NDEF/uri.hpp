#pragma once

#include <stdint.h>

#define NDEF_URI_CODE_IDENTIFIER_COUNT 36

struct NdefUriPrefix
{
    uint8_t code;
    const char *prefix;
};

class NdefUriPayload
{
  public:
    NdefUriPayload(const uint8_t *data, uint8_t length) : data(data), length(length){};
    NdefUriPayload() : data(nullptr), length(0){};
    NdefUriPayload(const NdefUriPayload &other) :
        data(other.data), length(other.length){};
    NdefUriPayload(const char *uri);
    ~NdefUriPayload() { delete[] data; };
    bool is_valid() const;
    uint8_t get_code() const;
    const uint8_t *get_data() const { return data; };
    uint8_t get_length() const { return length; };
    const char *get_uri() const;

  private:
    const uint8_t *data;
    uint8_t length;
};