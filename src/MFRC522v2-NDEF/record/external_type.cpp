#include "external_type.hpp"

#define NDEF_RECORD_EXTERNAL_TYPE_PREFIX "urn:nfc:ext:"
#define NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH 12

namespace ArduinoNDEF
{
namespace Record
{

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const uint8_t *payload,
    uint32_t payload_length,
    bool is_message_begin,
    bool is_message_end,
    const NdefRecordId &id
)
{
  if (domain == nullptr || external_type == nullptr || payload == nullptr ||
      payload_length == 0)
    return nullptr;

  auto domain_length = strlen(domain);
  auto external_type_length = strlen(external_type);
  auto type_length = NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH + domain_length +
                     1 // ':' separator
                   + external_type_length;

  if (domain_length == 0 || external_type_length == 0 || type_length > 0xff)
    return nullptr;

  auto type = new uint8_t[type_length];
  auto pointer = type;
  if (type == nullptr)
    return nullptr;

  // Build type field
  // 1. Add prefix
  memcpy(
      pointer,
      NDEF_RECORD_EXTERNAL_TYPE_PREFIX,
      NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH
  );
  pointer += NDEF_RECORD_EXTERNAL_TYPE_PREFIX_LENGTH;
  // 2. Add domain
  memcpy(pointer, domain, domain_length);
  pointer += domain_length;
  // 3. Add ':' separator
  *pointer = ':';
  pointer++;
  // 4. Add external type
  memcpy(pointer, external_type, external_type_length);

  const NdefRecordType *type_field =
      new NdefRecordType(type, type_length, NdefRecordType::OwnershipUnique);
  const NdefRecordPayload *payload_field =
      new NdefRecordPayload(payload, payload_length, NdefRecordPayload::OwnershipCopy);

  if (type_field == nullptr || payload_field == nullptr)
  {
    delete type_field;
    delete payload_field;
    delete[] type;
    return nullptr;
  }

  return new ExternalType(
      *type_field,
      *payload_field,
      is_message_begin,
      is_message_end,
      id
  );
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
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
      domain,
      external_type,
      payload,
      payload_length,
      is_message_begin,
      is_message_end,
      *id
  );
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const uint8_t *payload,
    uint32_t payload_length,
    const NdefRecordId &id
)
{
  return create(domain, external_type, payload, payload_length, false, false, id);
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const uint8_t *payload,
    uint32_t payload_length
)
{
  return create(domain, external_type, payload, payload_length, false, false);
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const char *payload,
    bool is_message_begin,
    bool is_message_end,
    const NdefRecordId &id
)
{
  if (domain == nullptr || external_type == nullptr || payload == nullptr)
    return nullptr;

  return create(
      domain,
      external_type,
      reinterpret_cast<const uint8_t *>(payload),
      strlen(payload),
      is_message_begin,
      is_message_end,
      id
  );
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const char *payload,
    bool is_message_begin,
    bool is_message_end
)
{
  auto id = new NdefRecordId();

  if (id == nullptr)
    return nullptr;

  return create(domain, external_type, payload, is_message_begin, is_message_end, *id);
}

ExternalType *ExternalType::create(
    const char *domain,
    const char *external_type,
    const char *payload,
    const NdefRecordId &id
)
{
  return create(domain, external_type, payload, false, false, id);
}

ExternalType *
ExternalType::create(const char *domain, const char *external_type, const char *payload)
{
  return create(domain, external_type, payload, false, false);
}

} // namespace Record
} // namespace ArduinoNDEF