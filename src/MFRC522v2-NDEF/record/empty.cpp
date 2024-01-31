#include "empty.hpp"

namespace ArduinoNDEF
{
namespace Record
{

Empty *Empty::create(bool is_message_begin, bool is_message_end, const NdefRecordId &id)
{
  auto type_field = new NdefRecordType();
  auto payload_field = new NdefRecordPayload();

  if (type_field == nullptr || payload_field == nullptr)
    return nullptr;

  return new Empty(*type_field, *payload_field, is_message_begin, is_message_end, id);
}

Empty *Empty::create(const NdefRecordId &id) { return create(false, false, id); }

Empty *Empty::create(bool is_message_begin, bool is_message_end)
{
  auto id = new NdefRecordId();
  if (id == nullptr)
    return nullptr;

  return create(is_message_begin, is_message_end, *id);
}

Empty *Empty::create() { return create(false, false); }

} // namespace Record
} // namespace ArduinoNDEF