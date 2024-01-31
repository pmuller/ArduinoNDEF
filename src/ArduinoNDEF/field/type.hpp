#include "../field.hpp"

namespace ArduinoNDEF
{
namespace Field
{

class Type : public Field<uint8_t>
{
  public:
    using Field::Field;

    /**
     * @brief NFC Record Type Definition
     */
    enum RTD
    {
      RTD_TEXT = 0x54,
      RTD_URI = 0x55
    };

    /**
     * @brief Create a NDEF record type field based on a Record Type Definition
     */
    Type(RTD type)
    {
      auto data = new uint8_t[1];
      if (data == nullptr)
      {
        _data = nullptr;
        _length = 0;
        _ownership = OwnershipShared;
        return;
      }
      data[0] = type;
      _data = data;
      _length = 1;
      _ownership = OwnershipUnique;
    };
};

} // namespace Field
} // namespace ArduinoNDEF