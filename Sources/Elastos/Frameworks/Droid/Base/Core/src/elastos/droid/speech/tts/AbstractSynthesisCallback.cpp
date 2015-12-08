#include "elastos/droid/speech/tts/AbstractSynthesisCallback.h"

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

ECode AbstractSynthesisCallback::constructor(
    /* [in] */ Boolean clientIsUsingV2)
{
    mClientIsUsingV2 = clientIsUsingV2;

    return NOERROR;
}

ECode AbstractSynthesisCallback::ErrorCodeOnStop(
    /* [out] */ Int32* ret);
{
    *ret = mClientIsUsingV2 ? TextToSpeech.STOPPED : TextToSpeech.ERROR;

    return NOERROR;
}

} // namespace Tts
} // namespace Speech
} // namepsace Droid
} // namespace Elastos
