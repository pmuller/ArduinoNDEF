#pragma once

#include <stdint.h>

/**
 * @brief NDEF URI payload builder
 * @ref NFCForum-TS-RTD_URI_1.0.pdf
 */
class NdefUriPayload
{
  public:
    /**
     * @brief Create a NDEF URI payload from a data array
     * @param data The data array
     * @param length The length of the data array
     * @return A new NDEF URI payload object
     * @note The data array is owned by the NDEF URI payload object
     */
    NdefUriPayload(const uint8_t *data, uint8_t length) :
        _data(data), _length(length){};

    // NdefUriPayload() : _data(nullptr), _length(0){};
    // NdefUriPayload(const NdefUriPayload &other) :
    //     _data(other._data), _length(other._length){};

    /**
     * @brief Create a NDEF URI payload from a URI string
     * @param uri The URI string
     * @return A new NDEF URI payload object
     */
    NdefUriPayload(const char *uri);

    /**
     * @brief Destroy the NDEF URI payload object
     */
    ~NdefUriPayload() { delete[] _data; };

    /**
     * @brief Check if the NDEF URI payload is valid
     * @return true if the NDEF URI payload is valid
     */
    bool is_valid() const;

    /**
     * @brief Get the URI identifier code
     * @return The identifier code
     */
    uint8_t code() const;

    /**
     * @brief Get the URI data
     * @return The URI data
     */
    const uint8_t *data() const { return _data; };

    /**
     * @brief Get the URI data length
     * @return The URI data length
     */
    uint8_t length() const { return _length; };

    /**
     * @brief Get the URI string
     * @return The URI string
     * @note The URI string memory is automtically allocated but not owned by the NDEF
     * URI payload object
     */
    const char *uri() const;

  private:
    const uint8_t *_data;
    uint8_t _length;
};