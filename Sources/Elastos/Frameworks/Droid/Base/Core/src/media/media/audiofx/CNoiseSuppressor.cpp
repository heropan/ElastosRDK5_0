
#include "ext/frameworkext.h"
#include "CNoiseSuppressor.h"
#include "CAudioEffectHelper.h"
#include "AudioEffect.h"

using Elastos::Utility::IUUIDHelper;
using Elastos::Utility::CUUIDHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

const String CNoiseSuppressor::TAG("CNoiseSuppressor");

IAUDIOEFFECT_METHODS_IMPL(CNoiseSuppressor, AudioEffect)

/**
 * Class constructor.
 * <p> The constructor is not guarantied to succeed and throws the following exceptions:
 * <ul>
 *  <li>IllegalArgumentException is thrown if the device does not implement an NS</li>
 *  <li>UnsupportedOperationException is thrown is the resources allocated to audio
 *  pre-procesing are currently exceeded.</li>
 *  <li>RuntimeException is thrown if a memory allocation error occurs.</li>
 * </ul>
 *
 * @param audioSession system wide unique audio session identifier. The NoiseSuppressor
 * will be applied to the AudioRecord with the same audio session.
 *
 * @throws java.lang.IllegalArgumentException
 * @throws java.lang.UnsupportedOperationException
 * @throws java.lang.RuntimeException
 */
ECode CNoiseSuppressor::constructor(
    /* [in] */ Int32 audioSession)
{
    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeNS;
    AutoPtr<IUUID> typeNULL;
    helper->FromString(IAudioEffect::EFFECT_TYPE_NS, (IUUID**)&typeNS);
    helper->FromString(IAudioEffect::EFFECT_TYPE_NULL, (IUUID**)&typeNULL);
    Init(typeNS, typeNULL, 0, audioSession);
}

/**
 * Checks if the device implements acoustic echo cancellation.
 * @return true if the device implements acoustic echo cancellation, false otherwise.
 */
ECode CNoiseSuppressor::IsAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeNS;
    helper->FromString(IAudioEffect::EFFECT_TYPE_NS, (IUUID**)&typeNS);
    AutoPtr<IAudioEffectHelper> aeHelper;
    CAudioEffectHelper::AcquireSingleton((IAudioEffectHelper**)&aeHelper);
    return aeHelper->IsEffectTypeAvailable(typeNS, result);
}

PInterface CNoiseSuppressor::Probe(
    /* [in] */ REIID riid)
{
    return _CNoiseSuppressor::Probe(riid);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
