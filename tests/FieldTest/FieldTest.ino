#line 2 "FieldTest.ino"

#include <AUnitVerbose.h>
#include <ArduinoNDEF.h>

test(constructor_empty)
{
  ArduinoNDEF::Field::Field<uint8_t> field;
  assertEqual(field.data(), nullptr);
  assertEqual(field.length(), static_cast<uint8_t>(0));
}

test(constructor_data_pointer_null)
{
  ArduinoNDEF::Field::Field<uint8_t> data_pointer_null_field(nullptr, 0);
  ArduinoNDEF::Field::Field<uint8_t> empty_field;
  assertTrue(data_pointer_null_field == empty_field);
}

test(constructor_c_string_copy)
{
  ArduinoNDEF::Field::Field<uint8_t> field(
      "Hello",
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  assertEqual(field.data()[0], static_cast<uint8_t>('H'));
  assertEqual(field.data()[1], static_cast<uint8_t>('e'));
  assertEqual(field.data()[2], static_cast<uint8_t>('l'));
  assertEqual(field.data()[3], static_cast<uint8_t>('l'));
  assertEqual(field.data()[4], static_cast<uint8_t>('o'));
  assertEqual(field.length(), 5);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique);
}

test(constructor_c_string_shared)
{
  ArduinoNDEF::Field::Field<uint8_t> field("Hello");
  assertEqual(field.data()[0], static_cast<uint8_t>('H'));
  assertEqual(field.data()[1], static_cast<uint8_t>('e'));
  assertEqual(field.data()[2], static_cast<uint8_t>('l'));
  assertEqual(field.data()[3], static_cast<uint8_t>('l'));
  assertEqual(field.data()[4], static_cast<uint8_t>('o'));
  assertEqual(field.length(), 5);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_unique)
{
  ArduinoNDEF::Field::Field<uint8_t> foo(
      "foo",
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  ArduinoNDEF::Field::Field<uint8_t> copy(foo);
  assertEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_shared)
{
  ArduinoNDEF::Field::Field<uint8_t> foo("foo");
  ArduinoNDEF::Field::Field<uint8_t> copy(foo);
  assertEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_unique_copied)
{
  ArduinoNDEF::Field::Field<uint8_t> foo(
      "foo",
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  ArduinoNDEF::Field::Field<uint8_t> copy(
      foo,
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  assertNotEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique);
}

test(ownership_unique)
{
  auto data = new uint8_t[3]{1, 2, 3};
  ArduinoNDEF::Field::Field<uint8_t> field(
      data,
      3,
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique
  );
  assertEqual(field.data(), data);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique);
  // No need to delete data, it is owned by the field
}

test(ownership_shared)
{
  auto data = new uint8_t[3]{1, 2, 3};
  ArduinoNDEF::Field::Field<uint8_t> field(
      data,
      3,
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared
  );
  assertEqual(field.data(), data);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
  // Data is shared so we need to delete it
  delete[] data;
}

test(ownership_copy)
{
  auto data = new uint8_t[3]{1, 2, 3};
  ArduinoNDEF::Field::Field<uint8_t> field(
      data,
      3,
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  assertNotEqual(field.data(), data);
  assertEqual(field.data()[0], 1);
  assertEqual(field.data()[1], 2);
  assertEqual(field.data()[2], 3);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique);
  // Data is copied so we need to delete it
  delete[] data;
}

test(ownership_copy_implicit)
{
  auto data = new uint8_t[3]{1, 2, 3};
  ArduinoNDEF::Field::Field<uint8_t> field(data, 3);
  assertNotEqual(field.data(), data);
  assertEqual(field.data()[0], 1);
  assertEqual(field.data()[1], 2);
  assertEqual(field.data()[2], 3);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipUnique);
  // Data is copied so we need to delete it
  delete[] data;
}

test(assignment_other_shared_self_shared)
{
  ArduinoNDEF::Field::Field<uint8_t> foo("foo");
  ArduinoNDEF::Field::Field<uint8_t> bar("bar");
  bar = foo;
  assertEqual(bar.data(), foo.data());
  assertEqual(bar.data()[0], static_cast<uint8_t>('f'));
  assertEqual(bar.data()[1], static_cast<uint8_t>('o'));
  assertEqual(bar.data()[2], static_cast<uint8_t>('o'));
  assertEqual(bar.length(), 3);
  assertEqual(bar.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
}

test(assignment_other_shared_self_unique)
{
  ArduinoNDEF::Field::Field<uint8_t> foo("foo");
  ArduinoNDEF::Field::Field<uint8_t> bar(
      "bar",
      ArduinoNDEF::Field::Field<uint8_t>::OwnershipCopy
  );
  bar = foo;
  assertEqual(bar.data(), foo.data());
  assertEqual(bar.data()[0], static_cast<uint8_t>('f'));
  assertEqual(bar.data()[1], static_cast<uint8_t>('o'));
  assertEqual(bar.data()[2], static_cast<uint8_t>('o'));
  assertEqual(bar.length(), 3);
  assertEqual(bar.ownership(), ArduinoNDEF::Field::Field<uint8_t>::OwnershipShared);
}

test(equality)
{
  ArduinoNDEF::Field::Field<uint8_t> foo("foo");
  ArduinoNDEF::Field::Field<uint8_t> bar("bar");
  ArduinoNDEF::Field::Field<uint8_t> foo2("foo");
  assertTrue(foo == foo2);
  assertFalse(foo == bar);
}

test(type)
{
  ArduinoNDEF::Field::Type text(ArduinoNDEF::Field::Type::RTD_TEXT);
  assertEqual(text.data()[0], static_cast<uint8_t>('T'));
  assertEqual(text.length(), 1);
  assertEqual(text.ownership(), ArduinoNDEF::Field::Type::OwnershipUnique);
  ArduinoNDEF::Field::Type uri(ArduinoNDEF::Field::Type::RTD_URI);
  assertEqual(uri.data()[0], static_cast<uint8_t>('U'));
  assertEqual(uri.length(), 1);
  assertEqual(uri.ownership(), ArduinoNDEF::Field::Type::OwnershipUnique);
}

void setup()
{
#if !defined(EPOXY_DUINO)
  delay(1000); // Wait for stability on some boards to avoid garbage on Serial
#endif
  Serial.begin(115200);
  while (!Serial)
    ;
}
void loop() { aunit::TestRunner::run(); }