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

// NFCForum-TS-RTD_URI_1.0.pdf - A.1
test(uri_payload__http_www)
{
  auto payload = ArduinoNDEF::Field::UriPayload::from_uri("http://www.nfc.com");
  assertEqual(payload->length(), static_cast<uint32_t>(8));
  assertEqual(payload->data()[0], 0x01); // http://www.
  assertEqual(payload->data()[1], static_cast<uint8_t>('n'));
  assertEqual(payload->data()[2], static_cast<uint8_t>('f'));
  assertEqual(payload->data()[3], static_cast<uint8_t>('c'));
  assertEqual(payload->data()[4], static_cast<uint8_t>('.'));
  assertEqual(payload->data()[5], static_cast<uint8_t>('c'));
  assertEqual(payload->data()[6], static_cast<uint8_t>('o'));
  assertEqual(payload->data()[7], static_cast<uint8_t>('m'));
  auto uri = payload->to_uri();
  assertEqual(uri, "http://www.nfc.com");
  delete[] uri;
  delete payload;
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.2
test(uri_payload__phone)
{
  auto payload = ArduinoNDEF::Field::UriPayload::from_uri("tel:+35891234567");
  assertEqual(payload->length(), static_cast<uint32_t>(13));
  assertEqual(payload->data()[0], 0x05); // tel:
  assertEqual(payload->data()[1], static_cast<uint8_t>('+'));
  assertEqual(payload->data()[2], static_cast<uint8_t>('3'));
  assertEqual(payload->data()[3], static_cast<uint8_t>('5'));
  assertEqual(payload->data()[4], static_cast<uint8_t>('8'));
  assertEqual(payload->data()[5], static_cast<uint8_t>('9'));
  assertEqual(payload->data()[6], static_cast<uint8_t>('1'));
  assertEqual(payload->data()[7], static_cast<uint8_t>('2'));
  assertEqual(payload->data()[8], static_cast<uint8_t>('3'));
  assertEqual(payload->data()[9], static_cast<uint8_t>('4'));
  assertEqual(payload->data()[10], static_cast<uint8_t>('5'));
  assertEqual(payload->data()[11], static_cast<uint8_t>('6'));
  assertEqual(payload->data()[12], static_cast<uint8_t>('7'));
  auto uri = payload->to_uri();
  assertEqual(uri, "tel:+35891234567");
  delete[] uri;
  delete payload;
}

// NFCForum-TS-RTD_URI_1.0.pdf - A.3
test(uri_payload__no_abbreviation)
{
  auto payload =
      ArduinoNDEF::Field::UriPayload::from_uri("mms://example.com/download.wmv");
  assertEqual(payload->length(), static_cast<uint32_t>(31));
  assertEqual(payload->data()[0], 0x00); // No abbreviation
  assertEqual(payload->data()[1], static_cast<uint8_t>('m'));
  assertEqual(payload->data()[2], static_cast<uint8_t>('m'));
  assertEqual(payload->data()[3], static_cast<uint8_t>('s'));
  assertEqual(payload->data()[4], static_cast<uint8_t>(':'));
  assertEqual(payload->data()[5], static_cast<uint8_t>('/'));
  assertEqual(payload->data()[6], static_cast<uint8_t>('/'));
  assertEqual(payload->data()[7], static_cast<uint8_t>('e'));
  assertEqual(payload->data()[8], static_cast<uint8_t>('x'));
  assertEqual(payload->data()[9], static_cast<uint8_t>('a'));
  assertEqual(payload->data()[10], static_cast<uint8_t>('m'));
  assertEqual(payload->data()[11], static_cast<uint8_t>('p'));
  assertEqual(payload->data()[12], static_cast<uint8_t>('l'));
  assertEqual(payload->data()[13], static_cast<uint8_t>('e'));
  assertEqual(payload->data()[14], static_cast<uint8_t>('.'));
  assertEqual(payload->data()[15], static_cast<uint8_t>('c'));
  assertEqual(payload->data()[16], static_cast<uint8_t>('o'));
  assertEqual(payload->data()[17], static_cast<uint8_t>('m'));
  assertEqual(payload->data()[18], static_cast<uint8_t>('/'));
  assertEqual(payload->data()[19], static_cast<uint8_t>('d'));
  assertEqual(payload->data()[20], static_cast<uint8_t>('o'));
  assertEqual(payload->data()[21], static_cast<uint8_t>('w'));
  assertEqual(payload->data()[22], static_cast<uint8_t>('n'));
  assertEqual(payload->data()[23], static_cast<uint8_t>('l'));
  assertEqual(payload->data()[24], static_cast<uint8_t>('o'));
  assertEqual(payload->data()[25], static_cast<uint8_t>('a'));
  assertEqual(payload->data()[26], static_cast<uint8_t>('d'));
  assertEqual(payload->data()[27], static_cast<uint8_t>('.'));
  assertEqual(payload->data()[28], static_cast<uint8_t>('w'));
  assertEqual(payload->data()[29], static_cast<uint8_t>('m'));
  assertEqual(payload->data()[30], static_cast<uint8_t>('v'));
  auto uri = payload->to_uri();
  assertEqual(uri, "mms://example.com/download.wmv");
  delete[] uri;
  delete payload;
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