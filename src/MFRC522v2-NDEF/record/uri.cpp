#include "../uri.hpp"

#include "uri.hpp"

NdefUriRecord *NdefUriRecord::create(
    const char *uri, bool is_message_begin, bool is_message_end, const NdefRecordId &id
)
{
  NdefUriPayload payload(uri);
  if (!payload.is_valid())
    return nullptr;

  auto payload_field = new NdefRecordPayload(payload.data(), payload.length());
  auto type_field = new NdefRecordType(NdefRecordType::RTD_URI);

  if (payload_field == nullptr || type_field == nullptr)
    return nullptr;

  return new NdefUriRecord(
      *type_field,
      *payload_field,
      is_message_begin,
      is_message_end,
      id
  );
}

NdefUriRecord *NdefUriRecord::create(const char *uri, const NdefRecordId &id)
{
  return create(uri, false, false, id);
}

NdefUriRecord *
NdefUriRecord::create(const char *uri, bool is_message_begin, bool is_message_end)
{
  auto id = new NdefRecordId();
  if (id == nullptr)
    return nullptr;
  return create(uri, is_message_begin, is_message_end, *id);
}

NdefUriRecord *NdefUriRecord::create(const char *uri)
{
  return create(uri, false, false);
}