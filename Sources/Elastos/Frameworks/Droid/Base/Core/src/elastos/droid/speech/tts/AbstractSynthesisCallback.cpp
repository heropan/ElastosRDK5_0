#include "elastos/droid/speech/tts/AbstractSynthesisCallback.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Tts {

/******************
 * AbstractSynthesisCallback
 *******************************************************************************************************/

CAR_OBJECT_IMPL(AbstractSynthesisCallback, Object, ISynthesisCallback);

AbstractSynthesisCallback::AbstractSynthesisCallback()
{}

AbstractSynthesisCallback::~AbstractSynthesisCallback()
{}

ECode AbstractSynthesisCallback::constructor()
{
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
