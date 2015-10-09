#include "speech/tts/AbstractSynthesisCallback.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

PInterface AbstractSynthesisCallback::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ISynthesisCallback*)this;
    }
    else if (riid == EIID_ISynthesisCallback) {
        return (ISynthesisCallback*)this;
    }
    return NULL;
}

UInt32 AbstractSynthesisCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AbstractSynthesisCallback::Release()
{
    return ElRefBase::Release();
}

ECode AbstractSynthesisCallback::GetInterfaceID(
    /* [in] */ IInterface* Object,
    /* [out] */ InterfaceID* iID)
{
    VALIDATE_NOT_NULL(iID);
    if (iID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (Object == (IInterface*)(ISynthesisCallback*)this) {
        *iID = EIID_ISynthesisCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode AbstractSynthesisCallback::GetMaxBufferSize(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisCallback::GetMaxBufferSize();
    return NOERROR;
}

ECode AbstractSynthesisCallback::Start(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 channelCount,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisCallback::Start(sampleRateInHz, audioFormat, channelCount);
    return NOERROR;
}

ECode AbstractSynthesisCallback::AudioAvailable(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisCallback::AudioAvailable(buffer, offset, length);
    return NOERROR;
}

ECode AbstractSynthesisCallback::Done(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = SynthesisCallback::Done();
    return NOERROR;
}

ECode AbstractSynthesisCallback::Error()
{
    return SynthesisCallback::Error();
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos