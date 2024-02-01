#pragma once

#include "../field.hpp"

namespace ArduinoNDEF
{
namespace Field
{

class Payload : public Field<uint32_t>
{
  public:
    using Field::Field;
};

} // namespace Field
} // namespace ArduinoNDEF