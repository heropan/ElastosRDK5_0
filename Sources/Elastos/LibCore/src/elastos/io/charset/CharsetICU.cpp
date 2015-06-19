#include "CharsetICU.h"
#include "CharsetEncoderICU.h"
#include "CharsetDecoderICU.h"
// #include "NativeConverter.h"

namespace Elastos {
namespace IO {
namespace Charset {

CharsetICU::CharsetICU()
{}

ECode CharsetICU::Init(
    /* [in] */ const String& canonicalName,
    /* [in] */ const String& icuCanonName,
    /* [in] */ ArrayOf<String>* aliases)
{
    mIcuCanonicalName = icuCanonName;
    return Charset::Init(canonicalName, aliases);
}

ECode CharsetICU::NewEncoder(
    /* [out] */ ICharsetEncoder** charsetEncoder)
{
    VALIDATE_NOT_NULL(charsetEncoder);
    AutoPtr<CharsetEncoderICU> encoderICU;
    FAIL_RETURN(CharsetEncoderICU::NewInstance((ICharset*)this, mIcuCanonicalName, (CharsetEncoderICU**)&encoderICU));
    *charsetEncoder = ICharsetEncoder::Probe(encoderICU.Get());
    REFCOUNT_ADD(*charsetEncoder);
    return NOERROR;
}

ECode CharsetICU::NewDecoder(
    /* [out] */ ICharsetDecoder** charsetDecoder)
{
    VALIDATE_NOT_NULL(charsetDecoder);
    AutoPtr<CharsetDecoderICU> decoderICU;
    FAIL_RETURN(CharsetDecoderICU::NewInstance((ICharset*)this, mIcuCanonicalName, (CharsetDecoderICU**)&decoderICU));
    *charsetDecoder = ICharsetDecoder::Probe(decoderICU.Get());
    REFCOUNT_ADD(*charsetDecoder);
    return NOERROR;
}

ECode CharsetICU::Contains(
    /* [in] */ ICharset* charset,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (charset == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    else if (_CObject_Compare((ICharset*)this, charset)) {
        *result = TRUE;
        return NOERROR;
    }

    String temp;
    charset->GetName(&temp);
    // *result = NativeConverter::Contains(mIcuCanonicalName, temp);
    return NOERROR;
}

ECode CharsetICU::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mCanonicalName;
    return NOERROR;
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
