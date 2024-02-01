#include "text.hpp"

namespace ArduinoNDEF
{
namespace Record
{

Text *Text::create(
    const char *text,
    const char *language_code,
    bool is_message_begin,
    bool is_message_end,
    const Field::Id &id
)
{
  auto type_field = new Field::Type(Field::Type::RTD_TEXT);
  if (type_field == nullptr)
    return nullptr;

  auto payload_field = Field::TextPayload::from_text(text, language_code);
  if (payload_field == nullptr)
  {
    delete type_field;
    return nullptr;
  }

  return new Text(*type_field, *payload_field, is_message_begin, is_message_end, id);
}

Text *Text::create(const char *text, const char *language_code)
{
  auto id = new Field::Id();
  if (id == nullptr)
    return nullptr;
  return create(text, language_code, false, false, *id);
}

Text *Text::create(const char *text, const char *language_code, const Field::Id &id)
{
  return create(text, language_code, false, false, id);
}

Text *Text::create(
    const char *text,
    const char *language_code,
    bool is_message_begin,
    bool is_message_end
)
{
  auto id = new Field::Id();
  if (id == nullptr)
    return nullptr;
  return create(text, language_code, is_message_begin, is_message_end, *id);
}

} // namespace Record
} // namespace ArduinoNDEF