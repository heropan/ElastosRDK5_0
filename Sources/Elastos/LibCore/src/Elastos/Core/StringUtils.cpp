#include "StringUtils.h"
#include <cmdef.h>
#include "StringToIntegral.h"
#include "StringToReal.h"
#include "IntegralToString.h"
#include "RealToString.h"
#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#include "CStringWrapper.h"
#include "CPatternHelper.h"
#include "CSplitter.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::ISplitter;
using Elastos::Utility::Regex::CSplitter;
using Elastos::Utility::Regex::IPattern;
using Elastos::Utility::Regex::IMatcher;

namespace Elastos {
namespace Core {

Int16 StringUtils::ParseInt16(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int16 defValue)
{
    Int16 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        printf("Warnning: StringUtils::ParseInt16 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Int32 StringUtils::ParseInt32(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int32 defValue)
{
    Int32 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        printf("Warnning: StringUtils::ParseInt32 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Int64 StringUtils::ParseInt64(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int64 defValue)
{
    Int64 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        printf("Warnning: StringUtils::ParseInt64 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Float StringUtils::ParseFloat(
    /* [in] */ const String& input,
    /* [in] */ Float defValue)
{
    Float value;
    ECode ec = StringToReal::Parse(input, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        printf("Warnning: StringUtils::ParseFloat NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Double StringUtils::ParseDouble(
    /* [in] */ const String& input,
    /* [in] */ Double defValue)
{
    Double value;
    ECode ec = StringToReal::Parse(input, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        printf("Warnning: StringUtils::ParseDouble NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

ECode StringUtils::ParseInt16(
    /* [in] */ const String& input,
    /* [out] */ Int16* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::ParseInt32(
    /* [in] */ const String& input,
    /* [out] */ Int32* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::ParseInt64(
    /* [in] */ const String& input,
    /* [out] */ Int64* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::ParseInt16(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int16* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::ParseInt32(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int32* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::ParseInt64(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int64* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::ParseFloat(
    /* [in] */ const String& input,
    /* [out] */ Float* result)
{
    return StringToReal::Parse(input, result);
}

ECode StringUtils::ParseDouble(
    /* [in] */ const String& input,
    /* [out] */ Double* result)
{
    return StringToReal::Parse(input, result);
}

String StringUtils::BooleanToString(
    /* [in] */ Boolean b)
{
    return b ? String("TRUE", 4) : String("FALSE", 5);
}

/**
 * Equivalent to Integer.toString(i, radix).
 */
String StringUtils::Int32ToString(
    /* [in] */ Int32 i,
    /* [in] */ Int32 radix)
{
    return IntegralToString::Int32ToString(i, radix);
}

/**
 * Equivalent to Long.toString(v, radix).
 */
String StringUtils::Int64ToString(
    /* [in] */ Int64 v,
    /* [in] */ Int32 radix)
{
    return IntegralToString::Int64ToString(v, radix);
}

String StringUtils::Int32ToBinaryString(
    /* [in] */ Int32 i)
{
    return IntegralToString::Int32ToBinaryString(i);
}

String StringUtils::Int64ToBinaryString(
    /* [in] */ Int64 v)
{
    return IntegralToString::Int64ToBinaryString(v);
}

String StringUtils::ByteToHexString(
    /* [in] */ Byte b,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::ByteToHexString(b, upperCase);
}

String StringUtils::BytesToHexString(
    /* [in] */ ArrayOf<Byte>& bytes,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::BytesToHexString(bytes, upperCase);
}

String StringUtils::Int32ToHexString(
    /* [in] */ Int32 i,
    /* [in] */ Boolean upperCase,
    /* [in] */ Int32 minWidth)
{
    return IntegralToString::Int32ToHexString(i, upperCase, minWidth);
}

String StringUtils::Int64ToHexString(
    /* [in] */ Int64 v,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::Int64ToHexString(v, upperCase);
}

String StringUtils::Int32ToOctalString(
    /* [in] */ Int32 i)
{
    return IntegralToString::Int32ToOctalString(i);
}

String StringUtils::Int64ToOctalString(
    /* [in] */ Int64 v)
{
    return IntegralToString::Int64ToOctalString(v);
}

String StringUtils::DoubleToString(
    /* [in] */ Double d)
{
    return RealToString::GetInstance()->DoubleToString(d);
}

String StringUtils::FloatToString(
    /* [in] */ Float f)
{
    return RealToString::GetInstance()->FloatToString(f);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [out, callee] */ ArrayOf<String>** array)
{
    return Split(input, regularExpression, 0, array);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    AutoPtr<ISplitter> splitter;
    CSplitter::AcquireSingleton((ISplitter**)&splitter);
    splitter->FastSplit(regularExpression, input, limit, array);
    if (*array != NULL){
        return NOERROR;
    }
    else{
        AutoPtr<IPatternHelper> helper;
        FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
        AutoPtr<IPattern> pattern;
        FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));
        AutoPtr<ICharSequence> seq;
        FAIL_RETURN(CStringWrapper::New(input, (ICharSequence**)&seq));
        return pattern->SplitEx2(seq, limit, array);
    }
}

ECode StringUtils::ReplaceFirst(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    AutoPtr<IPattern> pattern;
    FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));

    AutoPtr<IMatcher> matcher;
    FAIL_RETURN(pattern->Matcher(input, (IMatcher**)&matcher));
    return matcher->ReplaceFirst(replacement, result);
}

ECode StringUtils::ReplaceAll(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    AutoPtr<IPattern> pattern;
    FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));

    AutoPtr<IMatcher> matcher;
    FAIL_RETURN(pattern->Matcher(input, (IMatcher**)&matcher));
    return matcher->ReplaceAll(replacement, result);
}

ECode StringUtils::Matches(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /*[out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    return helper->Matches(regularExpression, input, result);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;
    VALIDATE_NOT_NULL(regularExpression);

    return Split(input, String(regularExpression), 0, array);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;
    VALIDATE_NOT_NULL(regularExpression);

    return Split(input, String(regularExpression), limit, array);
}

ECode StringUtils::ReplaceFirst(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ const char* replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);
    VALIDATE_NOT_NULL(regularExpression);
    VALIDATE_NOT_NULL(replacement);

    return ReplaceFirst(input, String(regularExpression), String(replacement), result);
}

ECode StringUtils::ReplaceAll(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ const char* replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);
    VALIDATE_NOT_NULL(regularExpression);
    VALIDATE_NOT_NULL(replacement);

    return ReplaceAll(input, String(regularExpression), String(replacement), result);
}

ECode StringUtils::Matches(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /*[out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(regularExpression);

    return Matches(input, String(regularExpression), result);
}

} // namespace Core
} // namespace Elastos
