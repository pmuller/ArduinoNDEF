#include "../field.hpp"

namespace ArduinoNDEF
{
namespace Field
{

class Id : public Field<uint8_t>
{
  public:
    using Field::Field;
};

} // namespace Field
} // namespace ArduinoNDEF