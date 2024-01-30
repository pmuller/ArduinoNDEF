#include "../record.hpp"

/**
 * @brief NDEF External Type Record
 */
class NdefExternalTypeRecord : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param payload_length The payload length
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param payload_length The payload length
     * @param id The record ID
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param payload_length The payload length
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length,
        bool is_message_begin,
        bool is_message_end
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param payload_length The payload length
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const char *payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param id The record ID
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const char *payload,
        const NdefRecordId &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *create(
        const char *domain,
        const char *external_type,
        const char *payload,
        bool is_message_begin,
        bool is_message_end
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @return NdefExternalTypeRecord* The NDEF External Type Record
     */
    static NdefExternalTypeRecord *
    create(const char *domain, const char *external_type, const char *payload);

  private:
    NdefExternalTypeRecord(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(
            TNF_EXTERNAL_TYPE, type, payload, is_message_begin, is_message_end, id
        ){};
};