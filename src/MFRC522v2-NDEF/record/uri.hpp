#include "../record.hpp"

/**
 * @brief NDEF URI Record
 */
class NdefUriRecord : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefUriRecord* The NDEF URI Record
     */
    static NdefUriRecord *create(
        const char *uri,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param id The record ID
     * @return NdefUriRecord* The NDEF URI Record
     */
    static NdefUriRecord *create(const char *uri, const NdefRecordId &id);
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefUriRecord* The NDEF URI Record
     */
    static NdefUriRecord *
    create(const char *uri, bool is_message_begin, bool is_message_end);
    /**
     * @brief Create a new NDEF URI Record
     *
     * @param uri The URI
     * @return NdefUriRecord* The NDEF URI Record
     */
    static NdefUriRecord *create(const char *uri);

  private:
    NdefUriRecord(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(TNF_WELL_KNOWN, type, payload, is_message_begin, is_message_end, id){
        };
};