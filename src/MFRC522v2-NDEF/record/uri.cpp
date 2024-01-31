#include "../uri.hpp"

#include "uri.hpp"

namespace ArduinoNDEF
{
namespace Record
{

Uri *Uri::create(
    const char *uri, bool is_message_begin, bool is_message_end, const Field::Id &id
)
{
  NdefUriPayload payload(uri);
  if (!payload.is_valid())
    return nullptr;

  auto payload_field = new Field::Payload(payload.data(), payload.length());
  auto type_field = new Field::Type(Field::Type::RTD_URI);

  if (payload_field == nullptr || type_field == nullptr)
    return nullptr;

  return new Uri(*type_field, *payload_field, is_message_begin, is_message_end, id);
}

Uri *Uri::create(const char *uri, const Field::Id &id)
{
  return create(uri, false, false, id);
}

Uri *Uri::create(const char *uri, bool is_message_begin, bool is_message_end)
{
  auto id = new Field::Id();
  if (id == nullptr)
    return nullptr;
  return create(uri, is_message_begin, is_message_end, *id);
}

Uri *Uri::create(const char *uri) { return create(uri, false, false); }

} // namespace Record
} // namespace ArduinoNDEF