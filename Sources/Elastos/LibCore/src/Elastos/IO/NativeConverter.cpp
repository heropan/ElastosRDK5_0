
#include "cmdef.h"
#include "NativeConverter.h"
#include "CharsetICU.h"
#include "elastos/UniquePtr.h"
#include <unicode/ucnv.h>
#include <unicode/ucnv_cb.h>
#include <unicode/uniset.h>
#include <unicode/utypes.h>
#include "CCodingErrorAction.h"

using Elastos::Core::UniquePtr;
using Elastos::IO::Charset::CCodingErrorAction;
using Elastos::IO::Charset::CharsetICU;

namespace Elastos {
namespace IO {

#define NativeConverter_REPORT 0
#define NativeConverter_IGNORE 1
#define NativeConverter_REPLACE 2

#define MAX_REPLACEMENT_LENGTH 32 // equivalent to UCNV_ERROR_BUFFER_LENGTH

static ECode maybeThrowIcuException(
    /* [in] */ UErrorCode errorCode)
{
    if (errorCode <= U_ZERO_ERROR) {
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

struct DecoderCallbackContext {
    UChar replacementChars[MAX_REPLACEMENT_LENGTH];
    size_t replacementCharCount;
    UConverterToUCallback onUnmappableInput;
    UConverterToUCallback onMalformedInput;
};

struct EncoderCallbackContext {
    char replacementBytes[MAX_REPLACEMENT_LENGTH];
    size_t replacementByteCount;
    UConverterFromUCallback onUnmappableInput;
    UConverterFromUCallback onMalformedInput;
};

struct UConverterDeleter {
    void operator()(UConverter* p) const {
        ucnv_close(p);
    }
};

typedef UniquePtr<UConverter, UConverterDeleter> UniqueUConverter;

static UConverter* toUConverter(Int64 address) {
    return reinterpret_cast<UConverter*>(static_cast<uintptr_t>(address));
}

static Boolean shouldCodecThrow(Boolean flush, UErrorCode error) {
    if (flush) {
        return (error != U_BUFFER_OVERFLOW_ERROR && error != U_TRUNCATED_CHAR_FOUND);
    }
    else {
        return (error != U_BUFFER_OVERFLOW_ERROR && error != U_INVALID_CHAR_FOUND
                && error != U_ILLEGAL_CHAR_FOUND);
    }
}

ECode NativeConverter::Decode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ArrayOf<Byte>* input,
    /* [in] */ Int32 inEnd,
    /* [in] */ ArrayOf<Char32>* output,
    /* [in] */ Int32 outEnd,
    /* [in] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL || input == NULL || output == NULL || data == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // maybeThrowIcuException(env, "toUConverter", U_ILLEGAL_ARGUMENT_ERROR);
        // return U_ILLEGAL_ARGUMENT_ERROR;
    }

    // Do the conversion.
    Int32* sourceOffset = &(*data)[0];
    Int32* targetOffset = &(*data)[1];
    const char* mySource = reinterpret_cast<const char*>(input->GetPayload() + *sourceOffset);
    const char* mySourceLimit = reinterpret_cast<const char*>(input->GetPayload() + inEnd);
    AutoPtr<ArrayOf<UChar> > temp = ArrayOf<UChar>::Alloc(output->GetLength());
    UChar* cTarget = temp->GetPayload() + *targetOffset;
    const UChar* cTargetLimit = temp->GetPayload() + outEnd;
    UErrorCode errorCode = U_ZERO_ERROR;
    ucnv_toUnicode(cnv, &cTarget, cTargetLimit, &mySource, mySourceLimit,
            NULL, flush, &errorCode);
    for (Int32 i = *targetOffset; i < temp->GetLength(); ++i) {
        (*output)[i] = (*temp)[i];
    }
    *sourceOffset = mySource - reinterpret_cast<const char*>(input->GetPayload()) - *sourceOffset;
    *targetOffset = cTarget - temp->GetPayload() - *targetOffset;

    // If there was an error, count the problematic bytes.
    if (errorCode == U_ILLEGAL_CHAR_FOUND || errorCode == U_INVALID_CHAR_FOUND) {
        int8_t invalidByteCount = 32;
        char invalidBytes[32] = {'\0'};
        UErrorCode minorErrorCode = U_ZERO_ERROR;
        ucnv_getInvalidChars(cnv, invalidBytes, &invalidByteCount, &minorErrorCode);
        if (U_SUCCESS(minorErrorCode)) {
            (*data)[2] = invalidByteCount;
        }
    }

    // Managed code handles some cases; throw all other errors.
    //if (shouldCodecThrow(flush, errorCode)) {
    //return maybeThrowIcuException(errorCode);
    //}
    //return errorCode;
    *result = errorCode;
    return NOERROR;
}

ECode NativeConverter::Encode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ArrayOf<Char32>* input,
    /* [in] */ Int32 inEnd,
    /* [in] */ ArrayOf<Byte>* output,
    /* [in] */ Int32 outEnd,
    /* [in] */ ArrayOf<Int32>* data,
    /* [in] */ Boolean flush,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL || input == NULL || output == NULL || data == NULL) {
        // maybeThrowIcuException(env, "toUConverter", U_ILLEGAL_ARGUMENT_ERROR);
        *result = E_ILLEGAL_ARGUMENT_EXCEPTION;
        return NOERROR;
    }

    // Do the conversion.
    Int32* sourceOffset = &(*data)[0];
    Int32* targetOffset = &(*data)[1];
    AutoPtr<ArrayOf<UChar> > temp = ArrayOf<UChar>::Alloc(inEnd);
    Int32 count = inEnd < input->GetLength() ? inEnd : input->GetLength();
    for (Int32 i = 0; i < count; ++i) {
        (*temp)[i] = (*input)[i];
    }
    const UChar* mySource = temp->GetPayload() + *sourceOffset;
    const UChar* mySourceLimit= temp->GetPayload() + inEnd;
    char* cTarget = reinterpret_cast<char*>(output->GetPayload() + *targetOffset);
    const char* cTargetLimit = reinterpret_cast<const char*>(output->GetPayload() + outEnd);
    UErrorCode errorCode = U_ZERO_ERROR;
    ucnv_fromUnicode(cnv , &cTarget, cTargetLimit, &mySource, mySourceLimit,
            NULL, (UBool)flush, &errorCode);
    *sourceOffset = (mySource - temp->GetPayload()) - *sourceOffset;
    *targetOffset = (reinterpret_cast<byte*>(cTarget) - output->GetPayload()) - *targetOffset;

    // If there was an error, count the problematic characters.
    if (errorCode == U_ILLEGAL_CHAR_FOUND || errorCode == U_INVALID_CHAR_FOUND) {
        int8_t invalidUCharCount = 32;
        UChar invalidUChars[32];
        UErrorCode minorErrorCode = U_ZERO_ERROR;
        ucnv_getInvalidUChars(cnv, invalidUChars, &invalidUCharCount, &minorErrorCode);
        if (U_SUCCESS(minorErrorCode)) {
            // myData[2] = invalidUCharCount;
        }
    }

    // Managed code handles some cases; throw all other errors.
    //if (shouldCodecThrow(flush, errorCode)) {
    //return maybeThrowIcuException(errorCode);
    //}
    *result = errorCode;
    return NOERROR;
}

ECode NativeConverter::OpenConverter(
    /* [in] */ const String& encoding,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    if (encoding.IsNull()) {
        *result = 0;
        return NOERROR;
    }
    UErrorCode status = U_ZERO_ERROR;
    UConverter* cnv = ucnv_open(encoding.string(), &status);
    FAIL_RETURN(maybeThrowIcuException(status));
    *result = reinterpret_cast<uintptr_t>(cnv);
    return NOERROR;
}

void NativeConverter::CloseConverter(
    /* [in] */ Int64 converterHandle)
{
    ucnv_close(toUConverter(converterHandle));
}

void NativeConverter::ResetByteToChar(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv) {
        ucnv_resetToUnicode(cnv);
    }
}

void NativeConverter::ResetCharToByte(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv) {
        ucnv_resetFromUnicode(cnv);
    }
}

AutoPtr<ArrayOf<Byte> > NativeConverter::GetSubstitutionBytes(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL) {
        return NULL;
    }
    UErrorCode status = U_ZERO_ERROR;
    char replacementBytes[MAX_REPLACEMENT_LENGTH];
    int8_t len = sizeof(replacementBytes);
    ucnv_getSubstChars(cnv, replacementBytes, &len, &status);

    AutoPtr<ArrayOf<Byte> > result;
    if (!U_SUCCESS(status)) {
        result = ArrayOf<Byte>::Alloc(0);
        return result;
    }

    result = ArrayOf<Byte>::Alloc(len);
    memcpy(result->GetPayload(), reinterpret_cast<Byte*>(replacementBytes), len);
    return result;
}

Int32 NativeConverter::GetMaxBytesPerChar(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    return (cnv != NULL) ? ucnv_getMaxCharSize(cnv) : -1;
}

Int32 NativeConverter::GetMinBytesPerChar(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    return (cnv != NULL) ? ucnv_getMinCharSize(cnv) : -1;
}

Float NativeConverter::GetAveBytesPerChar(
    /* [in] */ Int64 converterHandle)
{
    UConverter* cnv = toUConverter(converterHandle);
    return (cnv != NULL) ?
            ((ucnv_getMaxCharSize(cnv) + ucnv_getMinCharSize(cnv)) / 2.0) : -1;
}

static Int32 NativeConverter_getMaxBytesPerChar(Int64 address)
{
    UConverter* cnv = toUConverter(address);
    return (cnv != NULL) ? ucnv_getMaxCharSize(cnv) : -1;
}

Float NativeConverter::GetAveCharsPerByte(
    /* [in] */ Int64 converterHandle)
{
    return (1 / (Float) NativeConverter_getMaxBytesPerChar(converterHandle));
}

Boolean NativeConverter::Contains(
    /* [in] */ const String& converterName1,
    /* [in] */ const String& converterName2)
{
    if (converterName1.IsNull() || converterName2.IsNull()) {
        return FALSE;
    }

    UErrorCode errorCode = U_ZERO_ERROR;
    UniqueUConverter converter1(ucnv_open(converterName1.string(), &errorCode));
    UnicodeSet set1;
    ucnv_getUnicodeSet(converter1.get(), set1.toUSet(), UCNV_ROUNDTRIP_SET, &errorCode);

    UniqueUConverter converter2(ucnv_open(converterName2.string(), &errorCode));
    UnicodeSet set2;
    ucnv_getUnicodeSet(converter2.get(), set2.toUSet(), UCNV_ROUNDTRIP_SET, &errorCode);

    return U_SUCCESS(errorCode) && set1.containsAll(set2);
}

Boolean NativeConverter::CanEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 codeUnit)
{
    UErrorCode errorCode = U_ZERO_ERROR;
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL) {
        return FALSE;
    }

    UChar srcBuffer[3];
    const UChar* src = &srcBuffer[0];
    const UChar* srcLimit = (codeUnit < 0x10000) ? &src[1] : &src[2];

    char dstBuffer[5];
    char* dst = &dstBuffer[0];
    const char* dstLimit = &dstBuffer[4];

    Int32 i = 0;
    UTF_APPEND_CHAR(&srcBuffer[0], i, 2, codeUnit);

    ucnv_fromUnicode(cnv, &dst, dstLimit, &src, srcLimit, NULL, TRUE, &errorCode);
    return U_SUCCESS(errorCode);
}

/*
 * If a charset listed in the IANA Charset Registry is supported by an implementation
 * of the Java platform then its canonical name must be the name listed in the registry.
 * Many charsets are given more than one name in the registry, in which case the registry
 * identifies one of the names as MIME-preferred. If a charset has more than one registry
 * name then its canonical name must be the MIME-preferred name and the other names in
 * the registry must be valid aliases. If a supported charset is not listed in the IANA
 * registry then its canonical name must begin with one of the strings "X-" or "x-".
 */
static String getCanonicalName(
    /* [in] */ const char* icuCanonicalName)
{
    UErrorCode status = U_ZERO_ERROR;

    // Check to see if this is a well-known MIME or IANA name.
    const char* cName = NULL;
    if ((cName = ucnv_getStandardName(icuCanonicalName, "MIME", &status)) != NULL) {
        return String(cName);
    }
    else if ((cName = ucnv_getStandardName(icuCanonicalName, "IANA", &status)) != NULL) {
        return String(cName);
    }

    // Check to see if an alias already exists with "x-" prefix, if yes then
    // make that the canonical name.
    int32_t aliasCount = ucnv_countAliases(icuCanonicalName, &status);
    for (int i = 0; i < aliasCount; ++i) {
        const char* name = ucnv_getAlias(icuCanonicalName, i, &status);
        if (name != NULL && name[0] == 'x' && name[1] == '-') {
            return String(name);
        }
    }

    // As a last resort, prepend "x-" to any alias and make that the canonical name.
    status = U_ZERO_ERROR;
    const char* name = ucnv_getStandardName(icuCanonicalName, "UTR22", &status);
    if (name == NULL && strchr(icuCanonicalName, ',') != NULL) {
        name = ucnv_getAlias(icuCanonicalName, 1, &status);
    }
    // If there is no UTR22 canonical name then just return the original name.
    if (name == NULL) {
        name = icuCanonicalName;
    }
    UniquePtr<char[]> result(new char[2 + strlen(name) + 1]);
    strcpy(&result[0], "x-");
    strcat(&result[0], name);
    return String(&result[0]);
}

AutoPtr<ArrayOf<String> > NativeConverter::GetAvailableCharsetNames()
{
    int32_t num = ucnv_countAvailable();
    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(num);
    for (Int32 i = 0; i < num; ++i) {
        const char* name = ucnv_getAvailableName(i);
        String nameString = getCanonicalName(name);
        if (nameString.IsNull()) {
            return NULL;
        }
        (*result)[i] = nameString;
        // if (env->ExceptionCheck()) {
        //     return NULL;
        // }
    }
    return result;
}

static AutoPtr<ArrayOf<String> > getAliases(
    /* [in] */ const char* icuCanonicalName)
{
    // Get an upper bound on the number of aliases...
    const char* myEncName = icuCanonicalName;
    UErrorCode error = U_ZERO_ERROR;
    size_t aliasCount = ucnv_countAliases(myEncName, &error);
    if (aliasCount == 0 && myEncName[0] == 'x' && myEncName[1] == '-') {
        myEncName = myEncName + 2;
        aliasCount = ucnv_countAliases(myEncName, &error);
    }
    if (!U_SUCCESS(error)) {
        return NULL;
    }

    // Collect the aliases we want...
    AutoPtr<ArrayOf<String> > aliases = ArrayOf<String>::Alloc(aliasCount);
    for (size_t i = 0, j = 0; i < aliasCount; ++i) {
        const char* name = ucnv_getAlias(myEncName, i, &error);
        if (!U_SUCCESS(error)) {
            return NULL;
        }
        // TODO: why do we ignore these ones?
        if (strchr(name, '+') == 0 && strchr(name, ',') == 0) {
            (*aliases)[j] = name;
            j++;
        }
    }
    return aliases;
}

static const char* getICUCanonicalName(
    /* [in] */ const char* name)
{
    UErrorCode error = U_ZERO_ERROR;
    const char* canonicalName = NULL;
    if ((canonicalName = ucnv_getCanonicalName(name, "MIME", &error)) != NULL) {
        return canonicalName;
    }
    else if((canonicalName = ucnv_getCanonicalName(name, "IANA", &error)) != NULL) {
        return canonicalName;
    }
    else if((canonicalName = ucnv_getCanonicalName(name, "", &error)) != NULL) {
        return canonicalName;
    }
    else if((canonicalName =  ucnv_getAlias(name, 0, &error)) != NULL) {
        /* we have some aliases in the form x-blah .. match those first */
        return canonicalName;
    }
    else if (strstr(name, "x-") == name) {
        /* check if the converter can be opened with the name given */
        error = U_ZERO_ERROR;
        UniqueUConverter cnv(ucnv_open(name + 2, &error));
        if (cnv.get() != NULL) {
            return name + 2;
        }
    }
    return NULL;
}

ECode NativeConverter::CharsetForName(
    /* [in] */ const String& charsetName,
    /* [out] */ ICharset** charset)
{
    VALIDATE_NOT_NULL(charset);
    if (charsetName.IsNull()) {
        *charset = NULL;
        return NOERROR;
    }
    // Get ICU's canonical name for this charset.
    const char* icuCanonicalName = getICUCanonicalName(charsetName.string());
    if (icuCanonicalName == NULL) {
        *charset = NULL;
        return NOERROR;
    }
    // Get Java's canonical name for this charset.
    String canonicalName = getCanonicalName(icuCanonicalName);
    // if (env->ExceptionOccurred()) {
    //     return NULL;
    // }

    // Check that this charset is supported.
    // ICU doesn't offer any "isSupported", so we just open and immediately close.
    // We ignore the UErrorCode because ucnv_open returning NULL is all the information we need.
    UErrorCode dummy = U_ZERO_ERROR;
    UniqueUConverter cnv(ucnv_open(icuCanonicalName, &dummy));
    if (cnv.get() == NULL) {
        *charset = NULL;
        return NOERROR;
    }
    cnv.reset();

    // Get the aliases for this charset.
    AutoPtr<ArrayOf<String> > aliases = getAliases(icuCanonicalName);
    // if (env->ExceptionOccurred()) {
    //     return NULL;
    // }

//     // Construct the CharsetICU object.
    AutoPtr<CharsetICU> charsetICU = new CharsetICU();
    charsetICU->Init(canonicalName, String(icuCanonicalName), aliases);
    *charset = (ICharset*)charsetICU->Probe(Elastos::IO::Charset::EIID_ICharset);
    INTERFACE_ADDREF(*charset);
    return NOERROR;
}

ECode NativeConverter::TranslateCodingErrorAction(
    /* [in] */ ICodingErrorAction* action,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ecode = NOERROR;
    AutoPtr<ICodingErrorAction> codingErrorAction;
    ASSERT_SUCCEEDED(CCodingErrorAction::New((ICodingErrorAction** )&codingErrorAction));
    AutoPtr<ICodingErrorAction> reportAction, ignoreAction, replaceAction;
    codingErrorAction->GetREPORT((ICodingErrorAction**)&reportAction);
    codingErrorAction->GetIGNORE((ICodingErrorAction**)&ignoreAction);
    codingErrorAction->GetREPLACE((ICodingErrorAction**)&replaceAction);
    if (action == reportAction.Get()) {
        *result = 0;
    } else if (action == ignoreAction.Get()) {
        *result = 1;
    } else if (action == replaceAction.Get()) {
        *result = 2;
    } else {
        ecode = E_ASSERTION_ERROR;
    }
    return ecode;
}

ECode NativeConverter::SetCallbackDecode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetDecoder* decoder)
{
    AutoPtr<ICodingErrorAction> malformedAction, unmappableAction;
    decoder->MalformedInputAction((ICodingErrorAction**)&malformedAction);
    decoder->UnmappableCharacterAction((ICodingErrorAction**)&unmappableAction);
    String replacement;
    decoder->Replacement(&replacement);
    Int32 malformedResult, unmappableResult;
    FAIL_RETURN(TranslateCodingErrorAction(malformedAction, &malformedResult));
    FAIL_RETURN(TranslateCodingErrorAction(unmappableAction, &unmappableResult));
    return SetCallbackDecode(converterHandle, malformedResult, unmappableResult, replacement);
}

ECode NativeConverter::SetCallbackEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ ICharsetEncoder* encoder)
{
    AutoPtr<ICodingErrorAction> malformedAction, unmappableAction;
    encoder->GetMalformedInputAction((ICodingErrorAction**)&malformedAction);
    encoder->GetUnmappableCharacterAction((ICodingErrorAction**)&unmappableAction);
    AutoPtr< ArrayOf<Byte> > replacement;
    encoder->GetReplacement((ArrayOf<Byte>**)&replacement);
    Int32 malformedResult, unmappableResult;
    FAIL_RETURN(TranslateCodingErrorAction(malformedAction, &malformedResult));
    FAIL_RETURN(TranslateCodingErrorAction(unmappableAction, &unmappableResult));
    return SetCallbackEncode(converterHandle, malformedResult, unmappableResult, (ArrayOf<Byte>*)replacement);
}

static void decoderIgnoreCallback(
    const void*, UConverterToUnicodeArgs*, const char*, int32_t,
    UConverterCallbackReason, UErrorCode* err)
{
    // The icu4c UCNV_FROM_U_CALLBACK_SKIP callback requires that the context is NULL, which is
    // never true for us.
    *err = U_ZERO_ERROR;
}

static void decoderReplaceCallback(
    const void* rawContext, UConverterToUnicodeArgs* toArgs, const char*,
    int32_t, UConverterCallbackReason, UErrorCode* err)
{
    if (!rawContext) {
        return;
    }
    const DecoderCallbackContext* context = reinterpret_cast<const DecoderCallbackContext*>(rawContext);
    *err = U_ZERO_ERROR;
    ucnv_cbToUWriteUChars(toArgs,context->replacementChars,
            context->replacementCharCount, 0, err);
}

static UConverterToUCallback getToUCallback(
    /* [in] */ int32_t mode) {
    switch (mode) {
        case NativeConverter_IGNORE:
            return decoderIgnoreCallback;
        case NativeConverter_REPLACE:
            return decoderReplaceCallback;
        case NativeConverter_REPORT:
            return UCNV_TO_U_CALLBACK_STOP;
    }
    abort();
}

static void CHARSET_DECODER_CALLBACK(const void* rawContext, UConverterToUnicodeArgs* args,
        const char* codeUnits, Int32 length,
        UConverterCallbackReason reason, UErrorCode* status) {
    if (!rawContext) {
        return;
    }
    const DecoderCallbackContext* ctx = reinterpret_cast<const DecoderCallbackContext*>(rawContext);
    switch(reason) {
    case UCNV_UNASSIGNED:
        ctx->onUnmappableInput(ctx, args, codeUnits, length, reason, status);
        return;
    case UCNV_ILLEGAL:
    case UCNV_IRREGULAR:
        ctx->onMalformedInput(ctx, args, codeUnits, length, reason, status);
        return;
    case UCNV_CLOSE:
        delete ctx;
        return;
    default:
        *status = U_ILLEGAL_ARGUMENT_ERROR;
        return;
    }
}

ECode NativeConverter::SetCallbackDecode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 onMalformedInput,
    /* [in] */ Int32 onUnmappableInput,
    /* [in] */ const String& subChars)
{
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        maybeThrowIcuException(env, "toConverter", U_ILLEGAL_ARGUMENT_ERROR);
    }

    UConverterToUCallback oldCallback;
    const void* oldCallbackContext;
    ucnv_getToUCallBack(cnv, &oldCallback, &oldCallbackContext);

    DecoderCallbackContext* callbackContext = const_cast<DecoderCallbackContext*>(
            reinterpret_cast<const DecoderCallbackContext*>(oldCallbackContext));
    if (callbackContext == NULL) {
        callbackContext = new DecoderCallbackContext;
    }

    callbackContext->onMalformedInput = getToUCallback(onMalformedInput);
    callbackContext->onUnmappableInput = getToUCallback(onUnmappableInput);

    if (subChars.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //maybeThrowIcuException(env, "replacement", U_ILLEGAL_ARGUMENT_ERROR);
    }
    // u_strncpy(callbackContext->replacementChars,
    //         UnicodeString::fromUTF8(subChars.string()).getBuffer(), subChars.GetLength());
    callbackContext->replacementCharCount = subChars.GetLength();

    UErrorCode errorCode = U_ZERO_ERROR;
    ucnv_setToUCallBack(cnv, CHARSET_DECODER_CALLBACK, callbackContext,
            NULL, NULL, &errorCode);
    return maybeThrowIcuException(errorCode);
}

static void encoderReplaceCallback(const void* rawContext,
        UConverterFromUnicodeArgs* fromArgs, const UChar*, Int32, UChar32,
        UConverterCallbackReason, UErrorCode * err) {
    if (rawContext == NULL) {
        return;
    }
    const EncoderCallbackContext* context = reinterpret_cast<const EncoderCallbackContext*>(rawContext);
    *err = U_ZERO_ERROR;
    ucnv_cbFromUWriteBytes(fromArgs, context->replacementBytes, context->replacementByteCount, 0, err);
}

static UConverterFromUCallback getFromUCallback(int32_t mode) {
    switch(mode) {
        case NativeConverter_IGNORE:
            return UCNV_FROM_U_CALLBACK_SKIP;
        case NativeConverter_REPLACE:
            return encoderReplaceCallback;
        case NativeConverter_REPORT:
            return UCNV_FROM_U_CALLBACK_STOP;
    }
    abort();
}

static void CHARSET_ENCODER_CALLBACK(const void* rawContext, UConverterFromUnicodeArgs* args,
        const UChar* codeUnits, Int32 length, UChar32 codePoint, UConverterCallbackReason reason,
        UErrorCode* status) {
    if (!rawContext) {
        return;
    }
    const EncoderCallbackContext* ctx = reinterpret_cast<const EncoderCallbackContext*>(rawContext);
    switch(reason) {
    case UCNV_UNASSIGNED:
        ctx->onUnmappableInput(ctx, args, codeUnits, length, codePoint, reason, status);
        return;
    case UCNV_ILLEGAL:
    case UCNV_IRREGULAR:
        ctx->onMalformedInput(ctx, args, codeUnits, length, codePoint, reason, status);
        return;
    case UCNV_CLOSE:
        delete ctx;
        return;
    default:
        *status = U_ILLEGAL_ARGUMENT_ERROR;
        return;
    }
}

ECode NativeConverter::SetCallbackEncode(
    /* [in] */ Int64 converterHandle,
    /* [in] */ Int32 onMalformedInput,
    /* [in] */ Int32 onUnmappableInput,
    /* [in] */ ArrayOf<Byte>* subBytes)
{
    UConverter* cnv = toUConverter(converterHandle);
    if (cnv == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        maybeThrowIcuException(env, "toUConverter", U_ILLEGAL_ARGUMENT_ERROR);
    }

    UConverterFromUCallback oldCallback = NULL;
    const void* oldCallbackContext = NULL;
    ucnv_getFromUCallBack(cnv, &oldCallback, const_cast<const void**>(&oldCallbackContext));

    EncoderCallbackContext* callbackContext = const_cast<EncoderCallbackContext*>(
            reinterpret_cast<const EncoderCallbackContext*>(oldCallbackContext));
    if (callbackContext == NULL) {
        callbackContext = new EncoderCallbackContext;
    }

    callbackContext->onMalformedInput = getFromUCallback(onMalformedInput);
    callbackContext->onUnmappableInput = getFromUCallback(onUnmappableInput);

    // ScopedByteArrayRO replacementBytes(env, javaReplacement);
    if (subBytes == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        maybeThrowIcuException(env, "replacementBytes", U_ILLEGAL_ARGUMENT_ERROR);
    }
    // memcpy(callbackContext->replacementBytes, subBytes->GetPayload(), subBytes->GetLength());
    callbackContext->replacementByteCount = subBytes->GetLength();

    UErrorCode errorCode = U_ZERO_ERROR;
    ucnv_setFromUCallBack(cnv, CHARSET_ENCODER_CALLBACK, callbackContext,
            NULL, NULL, &errorCode);
    return maybeThrowIcuException(errorCode);
}

} // namespace IO
} // namespace Elastos
