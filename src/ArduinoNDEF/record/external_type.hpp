#include "../record.hpp"

namespace ArduinoNDEF
{
namespace Record
{

/**
 * @brief NDEF External Type Record
 */
class ExternalType : public Record
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
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param payload_length The payload length
     * @param id The record ID
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
        const char *domain,
        const char *external_type,
        const uint8_t *payload,
        uint32_t payload_length,
        const Field::Id &id
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
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
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
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
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
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
        const char *domain,
        const char *external_type,
        const char *payload,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param id The record ID
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
        const char *domain,
        const char *external_type,
        const char *payload,
        const Field::Id &id
    );
    /**
     * @brief Create a new NDEF External Type Record
     *
     * @param domain The domain
     * @param external_type The external type
     * @param payload The payload
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *create(
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
     * @return ExternalType* The NDEF External Type Record
     */
    static ExternalType *
    create(const char *domain, const char *external_type, const char *payload);

  private:
    ExternalType(
        const Field::Type &type,
        const Field::Payload &payload,
        bool is_message_begin,
        bool is_message_end,
        const Field::Id &id
    ) :
        Record(TNF_EXTERNAL_TYPE, type, payload, is_message_begin, is_message_end, id){
        };
};

} // namespace Record
} // namespace ArduinoNDEF