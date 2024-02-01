#pragma once

#include "payload.hpp"

namespace ArduinoNDEF
{
namespace Field
{

class TextPayload : public Payload
{
  public:
    /**
     * @brief Create a NDEF text payload
     * @param text The text
     * @param language_code The language
     * @return A pointer to a new NDEF text payload object
     */
    static TextPayload *from_text(const char *text, const char *language_code);

    /**
     * @brief Get the text
     * @return The text C-string
     * @note Memory is allocated for the text string and it is the responsibility of the
     * caller to free it
     */
    const char *to_text() const;

    /**
     * @brief Get the language code
     * @return The language code C-string
     * @note Memory is allocated for the language code string and it is the
     * responsibility of the caller to free it
     */
    const char *to_language_code() const;

  private:
    TextPayload(const uint8_t *data, uint8_t length) :
        Payload(data, length, OwnershipUnique)
    {
    }
};

} // namespace Field
} // namespace ArduinoNDEF