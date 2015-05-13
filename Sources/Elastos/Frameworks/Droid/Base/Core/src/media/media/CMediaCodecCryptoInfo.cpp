
#include "ext/frameworkext.h"
#include "media/CMediaCodecCryptoInfo.h"

using Elastos::Droid::Media::IMediaCodecCryptoInfo;

namespace Elastos {
namespace Droid {
namespace Media {

CMediaCodecCryptoInfo::CMediaCodecCryptoInfo()
    : mNumSubSamples(0)
    , mMode(0)
{
}

ECode CMediaCodecCryptoInfo::Set(
    /* [in] */ Int32 newNumSubSamples,
    /* [in] */ ArrayOf<Int32>* newNumBytesOfClearData,
    /* [in] */ ArrayOf<Int32>* newNumBytesOfEncryptedData,
    /* [in] */ ArrayOf<Byte>* newKey,
    /* [in] */ ArrayOf<Byte>* newIV,
    /* [in] */ Int32 newMode)
{
    mNumSubSamples = newNumSubSamples;
    mNumBytesOfClearData = newNumBytesOfClearData ? newNumBytesOfClearData->Clone() : NULL;
    mNumBytesOfEncryptedData = newNumBytesOfEncryptedData ? newNumBytesOfEncryptedData->Clone() : NULL;
    mKey = newKey ? newKey->Clone() : NULL;
    mInitVector = newIV ? newIV->Clone() : NULL;
    mMode = newMode;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetNumSubSamples(
    /* [out] */ Int32 *result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNumSubSamples;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetNumBytesOfClearData(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNumBytesOfClearData;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetNumBytesOfEncryptedData(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mNumBytesOfEncryptedData;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetKey(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mKey;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetInitVector(
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInitVector;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::GetMode(
    /* [out] */ Int32 *result)
{
    VALIDATE_NOT_NULL(result);
    *result = mMode;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetNumSubSamples(
    /* [in] */ Int32 samples)
{
    mNumSubSamples = samples;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetNumBytesOfClearData(
    /* [in] */ ArrayOf<Int32>* num)
{
    mNumBytesOfClearData = num ? num->Clone() : NULL;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetNumBytesOfEncryptedData(
    /* [in] */ ArrayOf<Int32>* num)
{
    mNumBytesOfEncryptedData = num ? num->Clone() : NULL;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetKey(
    /* [in] */ ArrayOf<Byte>* key)
{
    mKey = key ? key->Clone() : NULL;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetInitVector(
    /* [in] */ ArrayOf<Byte>* iv)
{
    mInitVector = iv ? iv->Clone() : NULL;
    return NOERROR;
}

ECode CMediaCodecCryptoInfo::SetMode(
    /* [in] */ Int32 mode)
{
    mMode = mode;
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
