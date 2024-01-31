#include "../record.hpp"

namespace ArduinoNDEF
{

/**
 * @brief NDEF Text Record
 */
class NdefTextRecord : public NdefRecord
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
     * @return NdefTextRecord* The NDEF Text Record
     */
    static NdefTextRecord *create(
        const char *text,
        const char *language_code,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @return NdefTextRecord* The NDEF Text Record
     */
    static NdefTextRecord *create(const char *text, const char *language_code);
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @param id The record ID
     * @return NdefTextRecord* The NDEF Text Record
     */
    static NdefTextRecord *
    create(const char *text, const char *language_code, const NdefRecordId &id);
    /**
     * @brief Create a new NDEF Text Record
     *
     * @param text The text
     * @param language_code The language code
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefTextRecord* The NDEF Text Record
     */
    static NdefTextRecord *create(
        const char *text,
        const char *language_code,
        bool is_message_begin,
        bool is_message_end
    );

  private:
    NdefTextRecord(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(TNF_WELL_KNOWN, type, payload, is_message_begin, is_message_end, id){
        };
};

} // namespace ArduinoNDEF