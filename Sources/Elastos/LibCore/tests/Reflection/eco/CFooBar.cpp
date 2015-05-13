
#include "CFooBar.h"

CFooBar::CFooBar()
    : mValue0(0)
    , mString("")
    , mValue1(1)
{
}

ECode CFooBar::GetString(
    /* [out] */ String* str)
{
    assert(str != NULL);
    *str = mString;
    return NOERROR;
}

ECode CFooBar::SetString(
    /* [in] */ const String& str)
{
    mString = str;
    return NOERROR;
}

ECode CFooBar::SetValue0(
    /* [in] */ Int32 value)
{
    mValue0 = value;
    return NOERROR;
}

ECode CFooBar::GetValue0(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = mValue0;
    return NOERROR;
}

ECode CFooBar::SetValue1(
    /* [in] */ Int64 value)
{
    mValue1 = value;
    return NOERROR;
}

ECode CFooBar::GetValue1(
    /* [out] */ Int64* value)
{
    assert(value != NULL);
    *value = mValue1;
    return NOERROR;
}

ECode CFooBar::SetAllValues(
    /* [in] */ Int32 value0,
    /* [in] */ const String& str,
    /* [in] */ Int64 value1)
{
    mValue0 = value0;
    mString = str;
    mValue1 = value1;
    return NOERROR;
}

ECode CFooBar::GetAllValues(
    /* [out] */ Int32* value0,
    /* [out] */ String* str,
    /* [out] */ Int64* value1)
{
    assert(str != NULL);
    assert(value0 != NULL);
    assert(value1 != NULL);
    *str = mString;
    *value0 = mValue0;
    *value1 = mValue1;
    return NOERROR;
}
