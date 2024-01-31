#include "empty.hpp"

namespace ArduinoNDEF
{
namespace Record
{

Empty *Empty::create(bool is_message_begin, bool is_message_end, const Field::Id &id)
{
  auto type_field = new Field::Type();
  auto payload_field = new Field::Payload();

  if (type_field == nullptr || payload_field == nullptr)
    return nullptr;

  return new Empty(*type_field, *payload_field, is_message_begin, is_message_end, id);
}

Empty *Empty::create(const Field::Id &id) { return create(false, false, id); }

Empty *Empty::create(bool is_message_begin, bool is_message_end)
{
  auto id = new Field::Id();
  if (id == nullptr)
    return nullptr;

  return create(is_message_begin, is_message_end, *id);
}

Empty *Empty::create() { return create(false, false); }

} // namespace Record
} // namespace ArduinoNDEF