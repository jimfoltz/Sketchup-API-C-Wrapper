#include "SketchUpAPITests.hpp"
#include "gtest/gtest.h"

#include <vector>

#include <SketchUpAPI/sketchup.h>

#include "SUAPI-CppWrapper/model/TypedValue.hpp"
#include "SUAPI-CppWrapper/String.hpp"


TEST(TypedValue, bool_value_true)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetBool(su_typed_value, true));

  CW::TypedValue typed_value(su_typed_value);
  auto value = typed_value.bool_value();

  ASSERT_TRUE(value);
}

TEST(TypedValue, bool_value_false)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetBool(su_typed_value, false));

  CW::TypedValue typed_value(su_typed_value);
  auto value = typed_value.bool_value();

  ASSERT_FALSE(value);
}

TEST(DISABLED_TypedValue, bool_value_wrong_type)
{
  SUColor color{ 255, 128, 64, 32 };

  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  SU(SUTypedValueSetColor(su_typed_value, &color));

  CW::TypedValue typed_value(su_typed_value);
  // TODO(thomthom): Should be a custom exception type:
  ASSERT_THROW(typed_value.bool_value(), std::exception);
}


TEST(TypedValue, typed_value_array_GetInts)
{
  SUTypedValueRef su_typed_value = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value));
  std::vector<SUTypedValueRef> su_values;
  for (int32_t i = 0; i < 3; ++i)
  {
    SUTypedValueRef su_value = SU_INVALID;
    SU(SUTypedValueCreate(&su_value));
    SU(SUTypedValueSetInt32(su_value, i));
    su_values.emplace_back(su_value);
  }
  SU(SUTypedValueSetArrayItems(su_typed_value, su_values.size(), su_values.data()));

  CW::TypedValue typed_value(su_typed_value);
  auto values = typed_value.typed_value_array();

  ASSERT_EQ(3, values.size());

  ASSERT_EQ(0, values.at(0).int32_value());
  ASSERT_EQ(1, values.at(1).int32_value());
  ASSERT_EQ(2, values.at(2).int32_value());
}

TEST(TypedValue, typed_value_array_GetMixed)
{
  // Create the array container
  SUTypedValueRef su_typed_value_array = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value_array));


  int32_t i = 1;
  SUTypedValueRef su_typed_value_int = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value_int));
  SU(SUTypedValueSetInt32(su_typed_value_int, i));

  double d = 2.0;
  SUTypedValueRef su_typed_value_double = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value_double));
  SU(SUTypedValueSetDouble(su_typed_value_double, d));

  const char str[] = "three";
  SUTypedValueRef su_typed_value_string = SU_INVALID;
  SU(SUTypedValueCreate(&su_typed_value_string));
  SU(SUTypedValueSetString(su_typed_value_string, str));

  std::vector<SUTypedValueRef> su_values;

  su_values.emplace_back(su_typed_value_int);
  su_values.emplace_back(su_typed_value_double);
  su_values.emplace_back(su_typed_value_string);

  SU(SUTypedValueSetArrayItems(su_typed_value_array, su_values.size(), su_values.data()));

  CW::TypedValue typed_value(su_typed_value_array);
  auto values = typed_value.typed_value_array();

  ASSERT_EQ(3, values.size());

  ASSERT_EQ(1, values.at(0).int32_value());
  ASSERT_EQ(2.0, values.at(1).double_value());
  ASSERT_EQ("three", values.at(2).string_value());
}
