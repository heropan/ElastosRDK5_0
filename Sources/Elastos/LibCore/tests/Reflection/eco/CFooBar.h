
#ifndef __CFOOBAR_H__
#define __CFOOBAR_H__

#include "_CFooBar.h"
#include <elastos.h>

CarClass(CFooBar)
{
public:
    CFooBar();

    CARAPI GetString(
        /* [out] */ String* str);

    CARAPI SetString(
        /* [in] */ const String& str);

    CARAPI GetValue0(
        /* [out] */ Int32* value);

    CARAPI SetValue0(
        /* [in] */ Int32 value);

    CARAPI GetValue1(
        /* [out] */ Int64* value);

    CARAPI SetValue1(
        /* [in] */ Int64 value);

    CARAPI SetAllValues(
        /* [in] */ Int32 value0,
        /* [in] */ const String& str,
        /* [in] */ Int64 value1);

    CARAPI GetAllValues(
        /* [out] */ Int32* value0,
        /* [out] */ String* str,
        /* [out] */ Int64* value1);

private:
    Int32 mValue0;
    String mString;
    Int64 mValue1;
};

#endif //__CFOOBAR_H__
