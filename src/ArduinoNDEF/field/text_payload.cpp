#include "text_payload.hpp"

namespace ArduinoNDEF
{
namespace Field
{

TextPayload *TextPayload::from_text(const char *text, const char *language_code)
{
  if (text == nullptr || language_code == nullptr)
    return nullptr;

  uint32_t language_code_length = strlen(language_code);

  if (language_code_length != 2 && language_code_length != 5)
    return nullptr;
  if (language_code_length == 5 && language_code[2] != '-')
    return nullptr;

  uint32_t header_length = 1 + language_code_length;
  uint8_t header[header_length];
  header[0] = language_code_length;
  memcpy(header + 1, language_code, language_code_length);

  uint32_t text_length = strlen(text);
  uint32_t payload_length = header_length + text_length;

  auto payload_data = new uint8_t[payload_length];
  if (payload_data == nullptr)
    return nullptr;

  memcpy(payload_data, header, header_length);
  memcpy(payload_data + header_length, text, text_length);

  return new TextPayload(payload_data, payload_length);
}

const char *TextPayload::to_text() const
{
  auto language_code_length = _data[0];
  auto header_length = /* language_length */ 1 + /* language */ language_code_length;
  auto text_length = _length - header_length;
  auto text = new char[text_length + 1];

  if (text == nullptr)
    return nullptr;

  memcpy(text, _data + header_length, text_length);
  text[text_length] = '\0';

  return text;
}

const char *TextPayload::to_language_code() const
{
  auto language_code_length = _data[0];
  auto language_code = new char[language_code_length + 1];

  if (language_code == nullptr)
    return nullptr;

  memcpy(language_code, _data + 1, language_code_length);
  language_code[language_code_length] = '\0';

  return language_code;
}

} // namespace Field
} // namespace ArduinoNDEF