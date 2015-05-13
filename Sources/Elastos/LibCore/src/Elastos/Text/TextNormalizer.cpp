
#include "unicode/normlzr.h"
#include "TextNormalizer.h"
#include "ElStringByteSink.h"

namespace Elastos {
namespace Text {

static Int32 ToUNormalizationMode(
    /* [in] */ NormalizerForm form)
{
    //Translate Java enum constants to ICU int constants.
    //See UNormalizationMode in "unicode.unorm.h".stable API since ICU 2.0.
/*    switch(form) {
        case NFC: return 4;
        case NFD: return 2;
        case NFKC: return 5;
        case NFKD: return 3;
    }*/
    //throw new AssertionError("unknown Normalizer.Form " + form);
    return NOERROR;
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode icu4_error(UErrorCode errorCode)
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

static String Native_normalizeImpl(
    /* [in] */ const String& src,
    /* [in] */ Int32 form)
{
    UNormalizationMode mode = static_cast<UNormalizationMode>(form);
    UErrorCode errorCode = U_ZERO_ERROR;
    UnicodeString dst;
    Normalizer::normalize(src.string(), mode, 0, dst, errorCode);
    icu4_error(errorCode);
    return dst.isBogus() ? String(NULL) : String((const char*)(dst.getBuffer()) );
}

static Boolean Native_isNormalizedImpl(
    /* [in] */ const String& src,
    /* [in] */ Int32 form)
{
    UNormalizationMode mode = static_cast<UNormalizationMode>(form);
    UErrorCode errorCode = U_ZERO_ERROR;
    UBool result = Normalizer::isNormalized(src.string(), mode, errorCode);
    icu4_error(errorCode);
    return (Boolean)result;
}

Boolean TextNormalizer::IsNormalized(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form)
{
    String str = String("");
    src->ToString(&str);
    return Native_isNormalizedImpl(str, ToUNormalizationMode(form));
}

String TextNormalizer::Normalize(
    /* [in] */ ICharSequence* src,
    /* [in] */ NormalizerForm form)
{
    String str = String("");
    src->ToString(&str);
    return Native_normalizeImpl(str, ToUNormalizationMode(form));
}

TextNormalizer::TextNormalizer(){}

} // namespace Text
} // namespace Elastos
