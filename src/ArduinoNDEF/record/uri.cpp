#include "uri.hpp"

#include "../field/uri_payload.hpp"

namespace ArduinoNDEF
{
namespace Record
{

Uri *Uri::create(
    const char *uri, bool is_message_begin, bool is_message_end, const Field::Id &id
)
{
  auto payload_field = Field::UriPayload::from_uri(uri);

  if (payload_field == nullptr)
    return nullptr;

  auto type_field = new Field::Type(Field::Type::RTD_URI);

  if (type_field == nullptr)
  {
    delete payload_field;
    return nullptr;
  }

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