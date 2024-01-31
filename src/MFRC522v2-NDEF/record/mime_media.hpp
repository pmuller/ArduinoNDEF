#include "../record.hpp"

/**
 * @brief NDEF MIME Media Record
 */
class NdefMimeMediaRecord : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param payload_length The payload length
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(
        const char *mime_type,
        const uint8_t *payload,
        uint32_t payload_length,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param payload_length The payload length
     * @param id The record ID
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(
        const char *mime_type,
        const uint8_t *payload,
        uint32_t payload_length,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param payload_length The payload length
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(
        const char *mime_type,
        const uint8_t *payload,
        uint32_t payload_length,
        bool is_message_begin,
        bool is_message_end
    );
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param payload_length The payload length
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *
    create(const char *mime_type, const uint8_t *payload, uint32_t payload_length);
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(
        const char *mime_type,
        const char *payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(
        const char *mime_type,
        const char *payload,
        bool is_message_begin,
        bool is_message_end
    );
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @param id The record ID
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *
    create(const char *mime_type, const char *payload, const NdefRecordId &id);
    /**
     * @brief Create a new NDEF MIME Media Record
     *
     * @param mime_type The MIME type
     * @param payload The payload
     * @return NdefMimeMediaRecord* The NDEF MIME Media Record
     */
    static NdefMimeMediaRecord *create(const char *mime_type, const char *payload);

  private:
    NdefMimeMediaRecord(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(TNF_MIME_MEDIA, type, payload, is_message_begin, is_message_end, id){
        };
};