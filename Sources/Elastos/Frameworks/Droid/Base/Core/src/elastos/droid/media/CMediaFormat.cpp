
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/CMediaFormat.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::IO::IByteBuffer;
using Elastos::Utility::CObjectStringMap;
using Elastos::Core::IFloat;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::CInteger32;
using Elastos::Core::CInteger64;
using Elastos::Core::CFloat;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Media {

CMediaFormat::CMediaFormat()
{
}

ECode CMediaFormat::constructor()
{
    return CObjectStringMap:: New((IObjectStringMap**)&mMap);
}

ECode CMediaFormat::constructor(
    /* [in] */ IObjectStringMap* map)
{
    mMap = map;
    return NOERROR;
}

ECode CMediaFormat::ContainsKey(
    /* [in] */ const String& name,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mMap->ContainsKey(name, result);
}

ECode CMediaFormat::GetInt32(
    /* [in] */ const String& name,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInterface> tmpObj;
    mMap->Get(name, (IInterface**)&tmpObj);
    IInteger32* obj = IInteger32::Probe(tmpObj.Get());
    if (obj) {
        return obj->GetValue(result);
    }
    else {
        *result = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CMediaFormat::GetInt64(
    /* [in] */ const String& name,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInterface> tmpObj;
    mMap->Get(name, (IInterface**)&tmpObj);
    IInteger64* obj = IInteger64::Probe(tmpObj.Get());
    if (obj) {
        return obj->GetValue(result);
    }
    else {
        *result = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CMediaFormat::GetFloat(
    /* [in] */ const String& name,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInterface> tmpObj;
    mMap->Get(name, (IInterface**)&tmpObj);
    IFloat* obj = IFloat::Probe(tmpObj.Get());
    if (obj) {
        return obj->GetValue(result);
    }
    else {
        *result = 0;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CMediaFormat::GetString(
    /* [in] */ const String& name,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInterface> tmpObj;
    mMap->Get(name, (IInterface**)&tmpObj);
    ICharSequence* obj = ICharSequence::Probe(tmpObj.Get());
    if (obj) {
        return obj->ToString(result);
    }
    else {
        *result = String(NULL);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

ECode CMediaFormat::GetByteBuffer(
    /* [in] */ const String& name,
    /* [out] */ IByteBuffer** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IInterface> tmpObj;
    mMap->Get(name, (IInterface**)&tmpObj);
    *result = IByteBuffer::Probe(tmpObj.Get());
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaFormat::SetInt32(
    /* [in] */ const String& name,
    /* [in] */ Int32 value)
{
    AutoPtr<IInteger32> temp;
    CInteger32::New(value, (IInteger32**)&temp);
    return mMap->Put(name, temp);
}

ECode CMediaFormat::SetInt64(
    /* [in] */ const String& name,
    /* [in] */ Int64 value)
{
    AutoPtr<IInteger64> temp;
    CInteger64::New(value, (IInteger64**)&temp);
    return mMap->Put(name,temp);
}

ECode CMediaFormat::SetFloat(
    /* [in] */ const String& name,
    /* [in] */ Float value)
{
    AutoPtr<IFloat> temp;
    CFloat::New(value, (IFloat**)&temp);
    return mMap->Put(name,temp);
}

ECode CMediaFormat::SetString(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    AutoPtr<ICharSequence> temp;
    CString::New(value, (ICharSequence**)&temp);
    return mMap->Put(name,temp);
}

ECode CMediaFormat::SetByteBuffer(
    /* [in] */ const String& name,
    /* [in] */ IByteBuffer* bytes)
{
    return mMap->Put(name, bytes);
}

ECode CMediaFormat::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    // return mMap.toString();
    *result = String("TODO:CMediaFormat:E_NOT_IMPLEMENTED");
    return E_NOT_IMPLEMENTED;
}

ECode CMediaFormat::CreateAudioFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channelCount,
    /* [out] */ IMediaFormat** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<CMediaFormat> format;
    FAIL_RETURN(CMediaFormat::NewByFriend((CMediaFormat**)&format));

    FAIL_RETURN(format->SetString(KEY_MIME, mime));
    FAIL_RETURN(format->SetInt32(KEY_SAMPLE_RATE, sampleRate));
    FAIL_RETURN(format->SetInt32(KEY_CHANNEL_COUNT, channelCount));

    *result = (IMediaFormat*)format.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaFormat::CreateVideoFormat(
    /* [in] */ const String& mime,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IMediaFormat** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<CMediaFormat> format;
    FAIL_RETURN(CMediaFormat::NewByFriend((CMediaFormat**)&format));

    FAIL_RETURN(format->SetString(KEY_MIME, mime));
    FAIL_RETURN(format->SetInt32(KEY_WIDTH, width));
    FAIL_RETURN(format->SetInt32(KEY_HEIGHT, height));

    *result = (IMediaFormat*)format.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaFormat::GetMap(
    /* [out] */ IObjectStringMap** result)
{
    VALIDATE_NOT_NULL(result);
     *result = mMap;
     REFCOUNT_ADD(*result);
     return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
