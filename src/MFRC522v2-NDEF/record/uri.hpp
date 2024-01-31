#include "../record.hpp"

namespace ArduinoNDEF
{
namespace Record
{

/**
 * @brief NDEF URI Record
 */
class Uri : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return Uri* The NDEF URI Record
     */
    static Uri *create(
        const char *uri, bool is_message_begin, bool is_message_end, const Field::Id &id
    );
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param id The record ID
     * @return Uri* The NDEF URI Record
     */
    static Uri *create(const char *uri, const Field::Id &id);
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return Uri* The NDEF URI Record
     */
    static Uri *create(const char *uri, bool is_message_begin, bool is_message_end);
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @return Uri* The NDEF URI Record
     */
    static Uri *create(const char *uri);

  private:
    Uri(const Field::Type &type,
        const Field::Payload &payload,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id) :
        NdefRecord(TNF_WELL_KNOWN, type, payload, is_message_begin, is_message_end, id){
        };
};

} // namespace Record
} // namespace ArduinoNDEF