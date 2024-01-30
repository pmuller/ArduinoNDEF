#include "empty.hpp"

NdefEmptyRecord *NdefEmptyRecord::create(
    bool is_message_begin, bool is_message_end, const NdefRecordId &id
)
{
  auto type_field = new NdefRecordType();
  auto payload_field = new NdefRecordPayload();

  if (type_field == nullptr || payload_field == nullptr)
    return nullptr;

  return new NdefEmptyRecord(
      *type_field,
      *payload_field,
      is_message_begin,
      is_message_end,
      id
  );
}

NdefEmptyRecord *NdefEmptyRecord::create(const NdefRecordId &id)
{
  return create(false, false, id);
}

NdefEmptyRecord *NdefEmptyRecord::create(bool is_message_begin, bool is_message_end)
{
  auto id = new NdefRecordId();
  if (id == nullptr)
    return nullptr;

  return create(is_message_begin, is_message_end, *id);
}

NdefEmptyRecord *NdefEmptyRecord::create() { return create(false, false); }