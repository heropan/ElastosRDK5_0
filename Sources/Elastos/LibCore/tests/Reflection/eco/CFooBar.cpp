
#include "CFooBar.h"

namespace Reflection {

CAR_INTERFACE_IMPL(CFooBar, Object, IFooBar)

CAR_OBJECT_IMPL(CFooBar)

CFooBar::CFooBar()
    : mValue0(0)
    , mString("")
    , mValue1(1)
{
}

ECode CFooBar::GetValue(
    /* [out] */ String* str)
{
    assert(str != NULL);
    *str = mString;
    return NOERROR;
}

ECode CFooBar::SetValue(
    /* [in] */ const String& str)
{
    mString = str;
    return NOERROR;
}

ECode CFooBar::SetValue(
    /* [in] */ Int32 value)
{
    mValue0 = value;
    return NOERROR;
}

ECode CFooBar::GetValue(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = mValue0;
    return NOERROR;
}

ECode CFooBar::SetValue(
    /* [in] */ Int64 value)
{
    mValue1 = value;
    return NOERROR;
}

ECode CFooBar::GetValue(
    /* [out] */ Int64* value)
{
    assert(value != NULL);
    *value = mValue1;
    return NOERROR;
}

ECode CFooBar::SetValue(
    /* [in] */ Int32 value0,
    /* [in] */ const String& str,
    /* [in] */ Int64 value1)
{
    mValue0 = value0;
    mString = str;
    mValue1 = value1;
    return NOERROR;
}

ECode CFooBar::GetValue(
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

} // Reflection