#include "../record.hpp"

namespace ArduinoNDEF
{
namespace Record
{

/**
 * @brief NDEF Text Record
 */
class Text : public Record
{
  public:
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param type The type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return Text* The NDEF Text Record
     */
    static Text *create(
        const char *text,
        const char *language_code,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id
    );
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @return Text* The NDEF Text Record
     */
    static Text *create(const char *text, const char *language_code);
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @param id The record ID
     * @return Text* The NDEF Text Record
     */
    static Text *
    create(const char *text, const char *language_code, const Field::Id &id);
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return Text* The NDEF Text Record
     */
    static Text *create(
        const char *text,
        const char *language_code,
        bool is_message_begin,
        bool is_message_end
    );

  private:
    Text(
        const Field::Type &type,
        const Field::Payload &payload,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id
    ) :
        Record(TNF_WELL_KNOWN, type, payload, is_message_begin, is_message_end, id){};
};

} // namespace Record
} // namespace ArduinoNDEF