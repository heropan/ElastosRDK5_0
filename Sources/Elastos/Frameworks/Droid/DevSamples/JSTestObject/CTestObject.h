
#ifndef __CTESTOBJECT_H__
#define __CTESTOBJECT_H__

#include "_CTestObject.h"

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace JSTestObject {

CarClass(CTestObject)
{
public:
    CARAPI Test_NULL();

    CARAPI Test_Int16(
        /* [in] */ Int16 inValue,
        /* [out] */ Int16* outValue);

    CARAPI Test_Int32(
        /* [in] */ Int32 inValue,
        /* [out] */ Int32* outValue);

    CARAPI Test_Int64(
        /* [in] */ Int64 inValue,
        /* [out] */ Int64* outValue);

    CARAPI Test_Byte(
        /* [in] */ Byte inValue,
        /* [out] */ Byte* outValue);

    CARAPI Test_Float(
        /* [in] */ Float inValue,
        /* [out] */ Float* outValue);

    CARAPI Test_Double(
        /* [in] */ Double inValue,
        /* [out] */ Double* outValue);

    CARAPI Test_Char8(
        /* [in] */ Char8 inValue,
        /* [out] */ Char8* outValue);

    CARAPI Test_Char16(
        /* [in] */ Char16 inValue,
        /* [out] */ Char16* outValue);

    CARAPI Test_CString(
        /* [in] */ CString inCStr,
        /* [out] */ CString* outCStr);

    CARAPI Test_String(
        /* [in] */ const String& inStr,
        /* [out] */ String* outStr);

    CARAPI Test_Int32_String(
        /* [in] */ Int32 inValue,
        /* [out] */ String* outStr);

    CARAPI Test_String_Int32_Int32_Int32_Int32_String_String_String_String(
        /* [in] */ const String& inStr,
        /* [in] */ Int32 inValue1,
        /* [in] */ Int32 inValue2,
        /* [in] */ Int32 inValue3,
        /* [in] */ Int32 inValue4,
        /* [out] */ String* outStr1,
        /* [out] */ String* outStr2,
        /* [out] */ String* outStr3,
        /* [out] */ String* outStr4);

    CARAPI Test_Boolean(
        /* [in] */ Boolean inValue,
        /* [out] */ Boolean* outValue);

    CARAPI Test_EMuid(
        /* [in] */ const EMuid& inId,
        /* [out] */ EMuid* outId);

    CARAPI Test_EGuid(
        /* [in] */ const EGuid& inId,
        /* [out] */ EGuid* outId);

    CARAPI Test_ECode(
        /* [in] */ ECode inValue,
        /* [out] */ ECode* outValue);

    CARAPI Test_ArrayOfInt32(
        /* [in] */ const ArrayOf<Int32>& inArray1,
        /* [in] */ ArrayOf<Int32>* inArray2,
        /* [out, callee] */ ArrayOf<Int32>** outArray);

    CARAPI Test_Interface(
        /* [in] */ IInterface* inObject,
        /* [out] */ IInterface** outObject);
};

}
}
}
}

#endif // __CTESTOBJECT_H__
