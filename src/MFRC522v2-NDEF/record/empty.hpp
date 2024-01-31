#include "../record.hpp"

namespace ArduinoNDEF
{

class NdefEmptyRecord : public NdefRecord
{
  public:
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @param id The record ID
     * @return NdefEmptyRecord* The NDEF Empty Record
     */
    static NdefEmptyRecord *
    create(bool is_message_begin, bool is_message_end, const NdefRecordId &id);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param id The record ID
     * @return NdefEmptyRecord* The NDEF Empty Record
     */
    static NdefEmptyRecord *create(const NdefRecordId &id);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @param is_message_begin Is this the first record in the message?
     * @param is_message_end Is this the last record in the message?
     * @return NdefEmptyRecord* The NDEF Empty Record
     */
    static NdefEmptyRecord *create(bool is_message_begin, bool is_message_end);
    /**
     * @brief Create a new NDEF Empty Record
     *
     * @return NdefEmptyRecord* The NDEF Empty Record
     */
    static NdefEmptyRecord *create();

  private:
    NdefEmptyRecord(
        const NdefRecordType &type,
        const NdefRecordPayload &payload,
        bool is_message_begin,
        bool is_message_end,
        const NdefRecordId &id
    ) :
        NdefRecord(TNF_EMPTY, type, payload, is_message_begin, is_message_end, id){};
};

}