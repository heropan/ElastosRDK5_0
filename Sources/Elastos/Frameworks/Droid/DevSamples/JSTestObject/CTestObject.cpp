
#include "CTestObject.h"

#include <cutils/log.h>

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace JSTestObject {

ECode CTestObject::Test_NULL()
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    return NOERROR;
}

ECode CTestObject::Test_Int16(
    /* [in] */ Int16 inValue,
    /* [out] */ Int16* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %d ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Int32(
    /* [in] */ Int32 inValue,
    /* [out] */ Int32* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %d ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Int64(
    /* [in] */ Int64 inValue,
    /* [out] */ Int64* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %lld ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Byte(
    /* [in] */ Byte inValue,
    /* [out] */ Byte* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %d ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Float(
    /* [in] */ Float inValue,
    /* [out] */ Float* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %f ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Double(
    /* [in] */ Double inValue,
    /* [out] */ Double* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %f ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Char8(
    /* [in] */ Char8 inValue,
    /* [out] */ Char8* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %c ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_Char16(
    /* [in] */ Char16 inValue,
    /* [out] */ Char16* outValue)
{
    assert(outValue != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %c ====", __FILE__, __FUNCTION__, inValue);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_CString(
    /* [in] */ CString inCStr,
    /* [out] */ CString* outCStr)
{
    assert(outCStr != NULL);
    ALOGD("==== File: %s, Function: %s, inCStr: %s ====", __FILE__, __FUNCTION__, (const char*)inCStr);
    *outCStr = inCStr;
    return NOERROR;
}

ECode CTestObject::Test_String(
    /* [in] */ const String& inStr,
    /* [out] */ String* outStr)
{
    assert(outStr != NULL);
    ALOGD("==== File: %s, Function: %s, inStr: %s ====", __FILE__, __FUNCTION__, inStr.string());
    *outStr = inStr;
    return NOERROR;
}

ECode CTestObject::Test_Int32_String(
    /* [in] */ Int32 inValue,
    /* [out] */ String* outStr)
{
    assert(outStr != NULL);
    ALOGD("==== File: %s, Function: %s, inValue: %d ====", __FILE__, __FUNCTION__, inValue);
    *outStr = "Test_Int32_String";
    return NOERROR;
}

ECode CTestObject::Test_String_Int32_Int32_Int32_Int32_String_String_String_String(
    /* [in] */ const String& inStr,
    /* [in] */ Int32 inValue1,
    /* [in] */ Int32 inValue2,
    /* [in] */ Int32 inValue3,
    /* [in] */ Int32 inValue4,
    /* [out] */ String* outStr1,
    /* [out] */ String* outStr2,
    /* [out] */ String* outStr3,
    /* [out] */ String* outStr4)
{
    assert(outStr1 != NULL);
    assert(outStr2 != NULL);
    assert(outStr3 != NULL);
    assert(outStr4 != NULL);
    ALOGD("==== File: %s, Function: %s, inStr: %s ====", __FILE__, __FUNCTION__, inStr.string());
    *outStr1 = "Test_String_Int32_Int32_Int32_Int32_String_String_String_String";
    return NOERROR;
}

ECode CTestObject::Test_Boolean(
    /* [in] */ Boolean inValue,
    /* [out] */ Boolean* outValue)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outValue != NULL);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_EMuid(
    /* [in] */ const EMuid& inId,
    /* [out] */ EMuid* outId)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outId != NULL);
    *outId = inId;
    return NOERROR;
}

ECode CTestObject::Test_EGuid(
    /* [in] */ const EGuid& inId,
    /* [out] */ EGuid* outId)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outId != NULL);
    *outId = inId;
    return NOERROR;
}

ECode CTestObject::Test_ECode(
    /* [in] */ ECode inValue,
    /* [out] */ ECode* outValue)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outValue != NULL);
    *outValue = inValue;
    return NOERROR;
}

ECode CTestObject::Test_ArrayOfInt32(
    /* [in] */ const ArrayOf<Int32>& inArray1,
    /* [in] */ ArrayOf<Int32>* inArray2,
    /* [out, callee] */ ArrayOf<Int32>** outArray)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outArray != NULL);
    *outArray = inArray2;
    if (*outArray != NULL) (*outArray)->AddRef();
    return NOERROR;
}

ECode CTestObject::Test_Interface(
    /* [in] */ IInterface* inObject,
    /* [out] */ IInterface** outObject)
{
    ALOGD("==== File: %s, Function: %s ====", __FILE__, __FUNCTION__);
    assert(outObject != NULL);
    *outObject = inObject;
    if (*outObject != NULL) (*outObject)->AddRef();
    return NOERROR;
}

}
}
}
}

