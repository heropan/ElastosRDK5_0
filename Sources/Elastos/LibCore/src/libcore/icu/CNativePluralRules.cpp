
#include "CNativePluralRules.h"
#include <unicode/plurrule.h>

namespace Libcore {
namespace ICU {

CNativePluralRules::~CNativePluralRules()
{
    FinalizeImpl(mAddress);
}

CNativePluralRules::Init(
    /* [in] */ Int32 address)
{
    mAddress = address;
}

AutoPtr<INativePluralRules> CNativePluralRules::ForLocale(
    /* [in] */ ILocale* locale)
{
    String s;
    locale->ToString(&s);
    AutoPtr<INativePluralRules> cres;
    CNativePluralRules::New((INativePluralRules **)&cres);
    ((CNativePluralRules *)cres.Get())->Init(ForLocaleImpl(s));
    return cres;
}

ECode CNativePluralRules::QuantityForInt(
    /* [in] */ Int32 value,
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result)

    *result = QuantityForIntImpl(mAddress, value);
    return NOERROR;
}

static PluralRules* toPluralRules(
    /* [in] */ Int32 address)
{
    return reinterpret_cast<PluralRules*>(static_cast<uintptr_t>(address));
}

static ECode maybeThrowIcuException(UErrorCode errorCode)
{
//    const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
        case U_ILLEGAL_ARGUMENT_ERROR:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        case U_INDEX_OUTOFBOUNDS_ERROR:
        case U_BUFFER_OVERFLOW_ERROR:
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        case U_UNSUPPORTED_ERROR:
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        default:
            return E_RUNTIME_EXCEPTION;
    }
}

void CNativePluralRules::FinalizeImpl(
    /* [in] */ Int32 address)
{
    delete toPluralRules(address);
}

Int32 CNativePluralRules::ForLocaleImpl(
    /* [in] */ const String& localeName)
{
    Locale locale = Locale::createFromName(localeName.string());
    UErrorCode status = U_ZERO_ERROR;
    PluralRules* result = PluralRules::forLocale(locale, status);
    ASSERT_SUCCEEDED(maybeThrowIcuException(status));
    return reinterpret_cast<uintptr_t>(result);
}

Int32 CNativePluralRules::QuantityForIntImpl(
    /* [in] */ Int32 address,
    /* [in] */ Int32 value)
{
    UnicodeString keyword = toPluralRules(address)->select(value);
    if (keyword == "zero") {
        return 0;
    }
    else if (keyword == "one") {
        return 1;
    }
    else if (keyword == "two") {
        return 2;
    }
    else if (keyword == "few") {
        return 3;
    }
    else if (keyword == "many") {
        return 4;
    }
    else {
        return 5;
    }
}

} // namespace ICU
} // namespace Libcore
