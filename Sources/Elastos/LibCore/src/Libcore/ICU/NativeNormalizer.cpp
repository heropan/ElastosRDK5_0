
#include "NativeNormalizer.h"
#include "ElStringByteSink.h"
#include <unicode/normlzr.h>

namespace Libcore {
namespace ICU {

ECode NativeNormalizer::IsNormalized(
    /* [in] */ ICharSequence* src,
    /* [in] */ Form form,
    /* [out] */ Boolean* isNormalized)
{
    VALIDATE_NOT_NULL(isNormalized);
    String s;
    src->ToString(&s);
    return IsNormalizedImpl(s, ToUNormalizationMode(form),isNormalized);
}

ECode NativeNormalizer::Normalize(
    /* [in] */ ICharSequence* src,
    /* [in] */ Form form,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    String s;
    src->ToString(&s);
    return NormalizeImpl(s, ToUNormalizationMode(form),result);
}


Int32 NativeNormalizer::ToUNormalizationMode(
    /* [in] */ Form form)
{
    // Translates Java enum constants to ICU int constants.
    // See UNormalizationMode in "unicode/unorm.h". Stable API since ICU 2.0.
    switch (form) {
        case Form_NFC: return 4;
        case Form_NFD: return 2;
        case Form_NFKC: return 5;
        case Form_NFKD: return 3;
    }
    assert(0);
    return -1;
}

static ECode maybeThrowIcuException(UErrorCode errorCode)
{
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

ECode NativeNormalizer::NormalizeImpl(
    /* [in] */ const String& src,
    /* [in] */ Int32 form,
    /* [out] */ String* result)
{
    UNormalizationMode mode = static_cast<UNormalizationMode>(form);
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString dst;
    Normalizer::normalize(UnicodeString::fromUTF8(src.string()), mode, 0, dst, status);
    FAIL_RETURN(maybeThrowIcuException(status));
    if (dst.isBogus()) {
        *result = NULL;
    }
    else {
        String s("");
        ElStringByteSink sink(&s);
        dst.toUTF8(sink);
        *result = s;
    }
    return NOERROR;
}

ECode NativeNormalizer::IsNormalizedImpl(
    /* [in] */ const String& src,
    /* [in] */ Int32 form,
    /* [out] */ Boolean* isNormalized)
{
    UNormalizationMode mode = static_cast<UNormalizationMode>(form);
    UErrorCode status = U_ZERO_ERROR;
    UBool result = Normalizer::isNormalized(UnicodeString::fromUTF8(src.string()), mode, status);
    FAIL_RETURN(maybeThrowIcuException(status));
    *isNormalized = result;
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
