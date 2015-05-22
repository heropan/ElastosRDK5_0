#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include <elastos.h>
#include <elstring.h>

namespace Elastos {
namespace Core {

class StringUtils
{
public:
    // From String without error code
    //
    static CARAPI_(Int16) ParseInt16(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix = 10,
        /* [in] */ Int16 defValue = 0);

    static CARAPI_(Int32) ParseInt32(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix = 10,
        /* [in] */ Int32 defValue = 0);

    static CARAPI_(Int64) ParseInt64(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix = 10,
        /* [in] */ Int64 defValue = 0);

    static CARAPI_(Float) ParseFloat(
        /* [in] */ const String& input,
        /* [in] */ Float defValue = 0.0);

    static CARAPI_(Double) ParseDouble(
        /* [in] */ const String& input,
        /* [in] */ Double defValue = 0.0);

    // From string with error code
    //
    static CARAPI ParseInt16(
        /* [in] */ const String& input,
        /* [out] */ Int16* result);

    static CARAPI ParseInt32(
        /* [in] */ const String& input,
        /* [out] */ Int32* result);

    static CARAPI ParseInt64(
        /* [in] */ const String& input,
        /* [out] */ Int64* result);

    static CARAPI ParseInt16(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix,
        /* [out] */ Int16* result);

    static CARAPI ParseInt32(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix,
        /* [out] */ Int32* result);

    static CARAPI ParseInt64(
        /* [in] */ const String& input,
        /* [in] */ Int32 radix,
        /* [out] */ Int64* result);

    static CARAPI ParseFloat(
        /* [in] */ const String& input,
        /* [out] */ Float* result);

    static CARAPI ParseDouble(
        /* [in] */ const String& input,
        /* [out] */ Double* result);

    // ToString
    //
    static CARAPI_(String) BooleanToString(
        /* [in] */ Boolean b);

    /**
     * Equivalent to Integer.toString(i, radix).
     */
    static CARAPI_(String) Int32ToString(
        /* [in] */ Int32 i,
        /* [in] */ Int32 radix = 10);

    /**
     * Equivalent to Long.toString(v, radix).
     */
    static CARAPI_(String) Int64ToString(
        /* [in] */ Int64 v,
        /* [in] */ Int32 radix = 10);

    static CARAPI_(String) Int32ToBinaryString(
        /* [in] */ Int32 i);

    static CARAPI_(String) Int64ToBinaryString(
        /* [in] */ Int64 v);

    static CARAPI_(String) ByteToHexString(
        /* [in] */ Byte b,
        /* [in] */ Boolean upperCase = TRUE);

    static CARAPI_(String) BytesToHexString(
        /* [in] */ ArrayOf<Byte>& bytes,
        /* [in] */ Boolean upperCase = TRUE);

    static CARAPI_(String) Int32ToHexString(
        /* [in] */ Int32 i,
        /* [in] */ Boolean upperCase = TRUE,
        /* [in] */ Int32 minWidth = 0);

    static CARAPI_(String) Int64ToHexString(
        /* [in] */ Int64 v,
        /* [in] */ Boolean upperCase = TRUE);

    static CARAPI_(String) Int32ToOctalString(
        /* [in] */ Int32 i);

    static CARAPI_(String) Int64ToOctalString(
        /* [in] */ Int64 v);

    static CARAPI_(String) DoubleToString(
        /* [in] */ Double d);

    static CARAPI_(String) FloatToString(
        /* [in] */ Float f);

    static CARAPI Split(
        /* [in] */ const String& input,
        /* [in] */ const String& regularExpression,
        /* [out, callee] */ ArrayOf<String>** array);

    static CARAPI Split(
        /* [in] */ const String& input,
        /* [in] */ const String& regularExpression,
        /* [in] */ Int32 limit,
        /* [out, callee] */ ArrayOf<String>** array);

    static CARAPI ReplaceFirst(
        /* [in] */ const String& input,
        /* [in] */ const String& regularExpression,
        /* [in] */ const String& replacement,
        /* [out] */ String* result);

    static CARAPI ReplaceAll(
        /* [in] */ const String& input,
        /* [in] */ const String& regularExpression,
        /* [in] */ const String& replacement,
        /* [out] */ String* result);

    static CARAPI Matches(
        /* [in] */ const String& input,
        /* [in] */ const String& regularExpression,
        /* [out] */ Boolean* result);

    static CARAPI Split(
        /* [in] */ const String& input,
        /* [in] */ const char* regularExpression,
        /* [out, callee] */ ArrayOf<String>** array);

    static CARAPI Split(
        /* [in] */ const String& input,
        /* [in] */ const char* regularExpression,
        /* [in] */ Int32 limit,
        /* [out, callee] */ ArrayOf<String>** array);

    static CARAPI ReplaceFirst(
        /* [in] */ const String& input,
        /* [in] */ const char* regularExpression,
        /* [in] */ const char* replacement,
        /* [out] */ String* result);

    static CARAPI ReplaceAll(
        /* [in] */ const String& input,
        /* [in] */ const char* regularExpression,
        /* [in] */ const char* replacement,
        /* [out] */ String* result);

    static CARAPI Matches(
        /* [in] */ const String& input,
        /* [in] */ const char* regularExpression,
        /* [out] */ Boolean* result);

    static CARAPI ParsePositiveInt64(
        /* [in] */ const String& input,
        /* [out] */ Int64* result);

    static CARAPI ParsePositiveInt64(
        /* [in] */ const String& input,
        /* [in] */ const Int32 radix,
        /* [out] */ Int64* result);

private:
    StringUtils();
    ~StringUtils();
};

} // namespace Core
} // namespace Elastos

#endif // __STRINGUTILS_H__
