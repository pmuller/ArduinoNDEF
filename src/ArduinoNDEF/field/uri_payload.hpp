#pragma once

#include "payload.hpp"

namespace ArduinoNDEF
{
namespace Field
{

class UriPayload : public Payload
{
  public:
    /**
     * @brief Create a NDEF URI payload from an URI C-string
     * @param uri The URI C-string
     * @return A pointer to a new NDEF URI payload object
     */
    static UriPayload *from_uri(const char *uri);

    /**
     * @brief Get the URI
     * @return The URI C-string
     * @note Memory is allocated for the URI string and it is the responsibility of the
     * caller to free it
     */
    const char *to_uri() const;

  private:
    UriPayload(const uint8_t *data, uint8_t length) :
        Payload(data, length, OwnershipUnique)
    {
    }
};

} // namespace Field
} // namespace ArduinoNDEF