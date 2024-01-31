#pragma once

#include <stdint.h>
#include <string.h>

namespace ArduinoNDEF
{
namespace Field
{

/**
 * @brief A NDEF record field
 */
template <typename LENGTH> class Field
{
  public:
    /**
     * @brief Ownership of the data
     */
    enum Ownership
    {
      // The data is owned by the NDEF record field
      // and will be deleted when the NDEF record field is destroyed
      OwnershipUnique,
      // The data is shared with other objects
      // and will not be deleted when the NDEF record field is destroyed
      OwnershipShared,
      // The data will be copied when the NDEF record field is created
      // and will be deleted when the NDEF record field is destroyed
      OwnershipCopy
    };

    /**
     * @brief Create an empty NDEF record field
     */
    Field() : _data(nullptr), _length(0), _ownership(OwnershipShared){};

    /**
     * @brief Create a NDEF record field
     *
     * @param data The data
     * @param length The data length
     * @param ownership The ownership of the data
     */
    Field(const uint8_t *data, LENGTH length, Ownership ownership = OwnershipCopy) :
        _data(nullptr), _length(0), _ownership(OwnershipShared)
    {
      // Empty data
      if (data == nullptr)
        return;
      // Create a NDEF record field by making a copy of data
      if (ownership == OwnershipCopy)
      {
        auto data_ = new uint8_t[length];
        if (data_ == nullptr)
          return;
        _ownership = OwnershipUnique;
        _length = length;
        memcpy(data_, data, length);
        _data = data_;
        return;
      }
      // OwnershipShared or OwnershipUnique
      this->_ownership = ownership;
      this->_length = length;
      this->_data = data;
    };

    /**
     * @brief Create a NDEF record field from a C-string
     *
     * @param data The string
     * @param ownership The ownership of the data
     */
    Field(const char *string, Ownership ownership = OwnershipShared) :
        Field(reinterpret_cast<const uint8_t *>(string), strlen(string), ownership){};

    /**
     * @brief Copy constructor
     *
     * @param other The other NDEF record field
     */
    Field(const Field &other, Ownership ownership = OwnershipShared) :
        _data(nullptr), _length(0), _ownership(OwnershipShared)
    {
      if (other._data == nullptr)
        return;

      if (ownership == OwnershipShared || other._ownership == OwnershipShared)
      {
        _data = other._data;
        _length = other._length;
        _ownership = OwnershipShared;
        return;
      }

      auto data = new uint8_t[other._length];

      if (data == nullptr)
        return;

      _ownership = OwnershipUnique;
      _length = other._length;
      memcpy(data, other._data, _length);
      _data = data;
    };

    /**
     * @brief Destroy the NDEF record field
     */
    ~Field()
    {
      if (_ownership == OwnershipUnique)
        delete[] _data;
    };

    /**
     * @brief Get the data
     *
     * @return const uint8_t* The data
     */
    const uint8_t *data() const { return _data; };

    /**
     * @brief Get the data length
     *
     * @return LENGTH The data length
     */
    LENGTH length() const { return _length; };

    /**
     * @brief Get the ownership of the data
     *
     * @return Ownership The ownership of the data
     */
    Ownership ownership() const { return _ownership; };

    Field &operator=(const Field &other)
    {
      // Self-assignment check
      if (this == &other)
        return *this;
      // Other field is empty or its data are shared
      if (other._data == nullptr || other._ownership == OwnershipShared)
      {
        if (_ownership == OwnershipUnique)
          delete[] _data;
        _data = other._data;
        _length = other._length;
        _ownership = OwnershipShared;
        return *this;
      }
      // Other field owns its data, let's copy it
      auto data = new uint8_t[other._length];
      if (data == nullptr)
        // XXX: Not great to silence the error, but not sure what to
        // do as we do not have exceptions
        return *this;
      memcpy(data, other._data, _length);
      _data = data;
      _ownership = OwnershipUnique;
      _length = other._length;
      return *this;
    };

    bool operator==(const Field &other) const
    {
      if (this == &other)
        return true;
      if (_length != other._length)
        return false;
      if (_data == nullptr && other._data == nullptr)
        return true;
      if (_data == nullptr || other._data == nullptr)
        return false;
      return memcmp(_data, other._data, _length) == 0;
    };

  protected:
    const uint8_t *_data;
    LENGTH _length;
    Ownership _ownership;
};

class Id : public Field<uint8_t>
{
  public:
    using Field::Field;
};

class Payload : public Field<uint32_t>
{
  public:
    using Field::Field;
};

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