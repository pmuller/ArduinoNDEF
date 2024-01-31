#include "mime_media.hpp"

#include <string.h>

namespace ArduinoNDEF
{
namespace Record
{

MimeMedia *MimeMedia::create(
    const char *mime_type,
    const uint8_t *payload,
    uint32_t payload_length,
    bool is_message_begin,
    bool is_message_end,
    const NdefRecordId &id
)
{
  auto type_field = new NdefRecordType(mime_type);
  auto payload_field = new NdefRecordPayload(payload, payload_length);

  if (type_field == nullptr || payload_field == nullptr)
    return nullptr;

  return new MimeMedia(
      *type_field,
      *payload_field,
      is_message_begin,
      is_message_end,
      id
  );
}

MimeMedia *MimeMedia::create(
    const char *mime_type,
    const uint8_t *payload,
    uint32_t payload_length,
    bool is_message_begin,
    bool is_message_end
)
{
  auto id = new NdefRecordId();

  if (id == nullptr)
    return nullptr;

  return create(
      mime_type,
      payload,
      payload_length,
      is_message_begin,
      is_message_end,
      *id
  );
}

MimeMedia *MimeMedia::create(
    const char *mime_type,
    const uint8_t *payload,
    uint32_t payload_length,
    const NdefRecordId &id
)
{
  return create(mime_type, payload, payload_length, false, false, id);
}

MimeMedia *MimeMedia::create(
    const char *mime_type, const uint8_t *payload, uint32_t payload_length
)
{
  return create(mime_type, payload, payload_length, false, false);
}

MimeMedia *MimeMedia::create(
    const char *mime_type,
    const char *payload,
    bool is_message_begin,
    bool is_message_end,
    const NdefRecordId &id
)
{
  return create(
      mime_type,
      reinterpret_cast<const uint8_t *>(payload),
      strlen(payload),
      is_message_begin,
      is_message_end,
      id
  );
}

MimeMedia *MimeMedia::create(
    const char *mime_type,
    const char *payload,
    bool is_message_begin,
    bool is_message_end
)
{
  return create(
      mime_type,
      reinterpret_cast<const uint8_t *>(payload),
      strlen(payload),
      is_message_begin,
      is_message_end
  );
}

MimeMedia *
MimeMedia::create(const char *mime_type, const char *payload, const NdefRecordId &id)
{
  return create(
      mime_type,
      reinterpret_cast<const uint8_t *>(payload),
      strlen(payload),
      id
  );
}

MimeMedia *MimeMedia::create(const char *mime_type, const char *payload)
{
  return create(mime_type, reinterpret_cast<const uint8_t *>(payload), strlen(payload));
}

} // namespace Record
} // namespace ArduinoNDEF