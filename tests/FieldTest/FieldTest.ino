#line 2 "FieldTest.ino"

#include <AUnitVerbose.h>
#include <MFRC522v2-NDEF.h>

test(constructor_empty)
{
  NdefRecordField<uint8_t> field;
  assertEqual(field.data(), nullptr);
  assertEqual(field.length(), static_cast<uint8_t>(0));
}

test(constructor_data_pointer_null)
{
  NdefRecordField<uint8_t> data_pointer_null_field(nullptr, 0);
  NdefRecordField<uint8_t> empty_field;
  assertTrue(data_pointer_null_field == empty_field);
}

test(constructor_c_string_copy)
{
  NdefRecordField<uint8_t> field("Hello", NdefRecordField<uint8_t>::OwnershipCopy);
  assertEqual(field.data()[0], static_cast<uint8_t>('H'));
  assertEqual(field.data()[1], static_cast<uint8_t>('e'));
  assertEqual(field.data()[2], static_cast<uint8_t>('l'));
  assertEqual(field.data()[3], static_cast<uint8_t>('l'));
  assertEqual(field.data()[4], static_cast<uint8_t>('o'));
  assertEqual(field.length(), 5);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipUnique);
}

test(constructor_c_string_shared)
{
  NdefRecordField<uint8_t> field("Hello");
  assertEqual(field.data()[0], static_cast<uint8_t>('H'));
  assertEqual(field.data()[1], static_cast<uint8_t>('e'));
  assertEqual(field.data()[2], static_cast<uint8_t>('l'));
  assertEqual(field.data()[3], static_cast<uint8_t>('l'));
  assertEqual(field.data()[4], static_cast<uint8_t>('o'));
  assertEqual(field.length(), 5);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_unique)
{
  NdefRecordField<uint8_t> foo("foo", NdefRecordField<uint8_t>::OwnershipCopy);
  NdefRecordField<uint8_t> copy(foo);
  assertEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_shared)
{
  NdefRecordField<uint8_t> foo("foo");
  NdefRecordField<uint8_t> copy(foo);
  assertEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
}

test(constructor_copy_other_unique_copied)
{
  NdefRecordField<uint8_t> foo("foo", NdefRecordField<uint8_t>::OwnershipCopy);
  NdefRecordField<uint8_t> copy(foo, NdefRecordField<uint8_t>::OwnershipCopy);
  assertNotEqual(copy.data(), foo.data());
  assertEqual(copy.data()[0], static_cast<uint8_t>('f'));
  assertEqual(copy.data()[1], static_cast<uint8_t>('o'));
  assertEqual(copy.data()[2], static_cast<uint8_t>('o'));
  assertEqual(copy.length(), 3);
  assertEqual(copy.ownership(), NdefRecordField<uint8_t>::OwnershipUnique);
}

test(ownership_unique)
{
  auto data = new uint8_t[3]{1, 2, 3};
  NdefRecordField<uint8_t> field(data, 3, NdefRecordField<uint8_t>::OwnershipUnique);
  assertEqual(field.data(), data);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipUnique);
  // No need to delete data, it is owned by the field
}

test(ownership_shared)
{
  auto data = new uint8_t[3]{1, 2, 3};
  NdefRecordField<uint8_t> field(data, 3, NdefRecordField<uint8_t>::OwnershipShared);
  assertEqual(field.data(), data);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
  // Data is shared so we need to delete it
  delete[] data;
}

test(ownership_copy)
{
  auto data = new uint8_t[3]{1, 2, 3};
  NdefRecordField<uint8_t> field(data, 3, NdefRecordField<uint8_t>::OwnershipCopy);
  assertNotEqual(field.data(), data);
  assertEqual(field.data()[0], 1);
  assertEqual(field.data()[1], 2);
  assertEqual(field.data()[2], 3);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipUnique);
  // Data is copied so we need to delete it
  delete[] data;
}

test(ownership_copy_implicit)
{
  auto data = new uint8_t[3]{1, 2, 3};
  NdefRecordField<uint8_t> field(data, 3);
  assertNotEqual(field.data(), data);
  assertEqual(field.data()[0], 1);
  assertEqual(field.data()[1], 2);
  assertEqual(field.data()[2], 3);
  assertEqual(field.length(), 3);
  assertEqual(field.ownership(), NdefRecordField<uint8_t>::OwnershipUnique);
  // Data is copied so we need to delete it
  delete[] data;
}

test(assignment_other_shared_self_shared)
{
  NdefRecordField<uint8_t> foo("foo");
  NdefRecordField<uint8_t> bar("bar");
  bar = foo;
  assertEqual(bar.data(), foo.data());
  assertEqual(bar.data()[0], static_cast<uint8_t>('f'));
  assertEqual(bar.data()[1], static_cast<uint8_t>('o'));
  assertEqual(bar.data()[2], static_cast<uint8_t>('o'));
  assertEqual(bar.length(), 3);
  assertEqual(bar.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
}

test(assignment_other_shared_self_unique)
{
  NdefRecordField<uint8_t> foo("foo");
  NdefRecordField<uint8_t> bar("bar", NdefRecordField<uint8_t>::OwnershipCopy);
  bar = foo;
  assertEqual(bar.data(), foo.data());
  assertEqual(bar.data()[0], static_cast<uint8_t>('f'));
  assertEqual(bar.data()[1], static_cast<uint8_t>('o'));
  assertEqual(bar.data()[2], static_cast<uint8_t>('o'));
  assertEqual(bar.length(), 3);
  assertEqual(bar.ownership(), NdefRecordField<uint8_t>::OwnershipShared);
}

test(equality)
{
  NdefRecordField<uint8_t> foo("foo");
  NdefRecordField<uint8_t> bar("bar");
  NdefRecordField<uint8_t> foo2("foo");
  assertTrue(foo == foo2);
  assertFalse(foo == bar);
}

test(type)
{
  NdefRecordType text(NdefRecordType::RTD_TEXT);
  assertEqual(text.data()[0], static_cast<uint8_t>('T'));
  assertEqual(text.length(), 1);
  assertEqual(text.ownership(), NdefRecordType::OwnershipUnique);
  NdefRecordType uri(NdefRecordType::RTD_URI);
  assertEqual(uri.data()[0], static_cast<uint8_t>('U'));
  assertEqual(uri.length(), 1);
  assertEqual(uri.ownership(), NdefRecordType::OwnershipUnique);
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