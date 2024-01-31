#pragma once

namespace ArduinoNDEF
{
namespace Error
{
enum Error
{
  Success,
  OutOfMemory,
  NullRecord,
  TextRecordCreationFailed,
  UriRecordCreationFailed,
  MimeMediaRecordCreationFailed,
  ExternalTypeRecordCreationFailed
};
} // namespace Error
} // namespace ArduinoNDEF