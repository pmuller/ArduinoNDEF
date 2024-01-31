#include "../record.hpp"

namespace ArduinoNDEF
{
namespace Record
{

class Empty : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return Empty* The NDEF Empty Record
     */
    static Empty *
    create(bool is_message_begin, bool is_message_end, const NdefRecordId &id);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param id The record ID
     * @return Empty* The NDEF Empty Record
     */
    static Empty *create(const NdefRecordId &id);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return Empty* The NDEF Empty Record
     */
    static Empty *create(bool is_message_begin, bool is_message_end);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @return Empty* The NDEF Empty Record
     */
    static Empty *create();

  private:
    Empty(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(TNF_EMPTY, type, payload, is_message_begin, is_message_end, id){};
};

} // namespace Record
} // namespace ArduinoNDEF